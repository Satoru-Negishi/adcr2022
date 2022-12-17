#!/usr/bin/env python
#共通
import cv2 as cv
import numpy as np
from std_msgs.msg import String, Int32MultiArray,MultiArrayDimension
import sys
import rospy
import mediapipe as mp
from std_srvs.srv import Empty, EmptyResponse

#mediapipe_pose
import copy
import argparse
from utils import CvFpsCalc
from test.msg import pose as MsgPose

#medeapipe_facemesh
import math

#numpyをメッセージ型のmultiarryに変換 #################################################
def _numpy2multiarray(multiarray_type, np_array):
    """Convert numpy.ndarray to multiarray"""
    multiarray = multiarray_type()
    multiarray.layout.dim = [MultiArrayDimension("dim%d" % i, np_array.shape[i], np_array.shape[i] * np_array.dtype.itemsize) for i in range(np_array.ndim)]
    multiarray.data = np_array.reshape(1, -1)[0].tolist()
    return multiarray

def _multiarray2numpy(pytype, dtype, multiarray):
    """Convert multiarray to numpy.ndarray"""
    dims = map(lambda x: x.size, multiarray.layout.dim)
    return np.array(multiarray.data, dtype=pytype).reshape(dims).astype(dtype)

#pyfeatのモデル読み込み完了のサービス(サーバー) ##################################
def handle_service(req):
    rospy.loginfo('ros_mediapipe: called')
    return EmptyResponse()

def service_server():
    s = rospy.Service('emotion_ready', Empty, handle_service)
    print('ros_mediapipe: Ready to Serve')
    
class ros_mediapipe(object):
    def __init__(self):
        #ビデオ表示関係 #################################################################
        # self.cap = cv2.VideoCapture(0)
        # self.frameWidth = 640
        # self.frameHeight = 480

        #瞬き########################################################################
        self.blink = String()

        #ROS_PUB_SUB #################################################################
        self._CamImage_pub = rospy.Publisher("cam_image", Int32MultiArray, queue_size=1)
        self._pose_pub = rospy.Publisher("pose_mediapipe", MsgPose, queue_size=1)  
        self._face_pub = rospy.Publisher("face_mediapipe", String, queue_size=1)
        
        #引数解析 #################################################################
        args = self.get_args()
        self.cap_device = args.device
        self.cap_width = args.width
        self.cap_height = args.height
        # upper_body_only = args.upper_body_only
        self.model_complexity = args.model_complexity
        self.min_detection_confidence = args.min_detection_confidence
        self.min_tracking_confidence = args.min_tracking_confidence
        self.enable_segmentation = args.enable_segmentation
        self.segmentation_score_th = args.segmentation_score_th
        self.use_brect = args.use_brect
        self.plot_world_landmark = args.plot_world_landmark
        
    def get_args(self):
        parser = argparse.ArgumentParser()

        parser.add_argument("--device", type=int, default=0)
        parser.add_argument("--width", help='cap width', type=int, default=960) #emotion_default= 640
        parser.add_argument("--height", help='cap height', type=int, default=540) #emotion_default=480

        # parser.add_argument('--upper_body_only', action='store_true')  # 0.8.3 or less
        parser.add_argument("--model_complexity",
                            help='model_complexity(0,1(default),2)',
                            type=int,
                            default=1)
        parser.add_argument("--min_detection_confidence",
                            help='min_detection_confidence',
                            type=float,
                            default=0.5)
        parser.add_argument("--min_tracking_confidence",
                            help='min_tracking_confidence',
                            type=float,
                            default=0.5)
        parser.add_argument('--enable_segmentation', action='store_true')
        parser.add_argument("--segmentation_score_th",
                            help='segmentation_score_threshold',
                            type=float,
                            default=0.5)

        parser.add_argument('--use_brect', action='store_true')
        parser.add_argument('--plot_world_landmark', action='store_true')

        args = parser.parse_args()

        return args
    
    def distance(self,v1,v2):
        """
        距離を計算,getEyeOpen関連の関数
        """
        return math.sqrt(math.pow(v1.x - v2.x, 2) + math.pow(v1.y - v2.y, 2))

    def eyeLidRatio(self,eyeOuterCorner, eyeInnerCorner, eyeOuterUpperLid, eyeMidUpperLid, eyeInnerUpperLid, eyeOuterLowerLid, eyeMidLowerLid, eyeInnerLowerLid):
        """
        瞼の比率を計算、getEyeOpen関連の関，getEyeOpen関連の関数
        """
        eyeWidth = self.distance(eyeOuterCorner, eyeInnerCorner)
        eyeOuterLidDistance  = self.distance(eyeOuterUpperLid, eyeOuterLowerLid)
        eyeMidLidDistance  = self.distance(eyeMidUpperLid, eyeMidLowerLid)
        eyeInnerLidDistance  = self.distance(eyeInnerUpperLid, eyeInnerLowerLid)
        eyeLidAvg =  (eyeOuterLidDistance + eyeMidLidDistance + eyeInnerLidDistance) / 3
        ratio = eyeLidAvg / eyeWidth
        return ratio

    def eyeLidRatio_left(self, results):
        """
        左瞼の比率を計算，getEyeOpen関連の関数
        """
        eyeOuterCorner = results.multi_face_landmarks[0].landmark[130]
        eyeInnerCorner = results.multi_face_landmarks[0].landmark[133]
        eyeOuterUpperLid = results.multi_face_landmarks[0].landmark[160]
        eyeMidUpperLid  = results.multi_face_landmarks[0].landmark[159]
        eyeInnerUpperLid = results.multi_face_landmarks[0].landmark[158]
        eyeOuterLowerLid = results.multi_face_landmarks[0].landmark[144]
        eyeMidLowerLid = results.multi_face_landmarks[0].landmark[145]
        eyeInnerLowerLid = results.multi_face_landmarks[0].landmark[153]

        return self.eyeLidRatio(eyeOuterCorner, eyeInnerCorner, eyeOuterUpperLid, eyeMidUpperLid, eyeInnerUpperLid, eyeOuterLowerLid, eyeMidLowerLid, eyeInnerLowerLid)

    def eyeLidRatio_right(self,results):
        """
        右瞼の比率を計算，getEyeOpen関連の関数
        """
        eyeOuterCorner = results.multi_face_landmarks[0].landmark[359]#263
        eyeInnerCorner = results.multi_face_landmarks[0].landmark[362]
        eyeOuterUpperLid = results.multi_face_landmarks[0].landmark[387]
        eyeMidUpperLid  = results.multi_face_landmarks[0].landmark[386]
        eyeInnerUpperLid = results.multi_face_landmarks[0].landmark[385]
        eyeOuterLowerLid = results.multi_face_landmarks[0].landmark[373]
        eyeMidLowerLid = results.multi_face_landmarks[0].landmark[374]
        eyeInnerLowerLid = results.multi_face_landmarks[0].landmark[380]

        return self.eyeLidRatio(eyeOuterCorner, eyeInnerCorner, eyeOuterUpperLid, eyeMidUpperLid, eyeInnerUpperLid, eyeOuterLowerLid, eyeMidLowerLid, eyeInnerLowerLid)

    def clamp(self,val,min_num,max_num):
        """
        比率と最大比率を比較，getEyeOpen関連の関数
        """
        return max(min(val, max_num), min_num)

    def remap(self,ratio,low,high):
        """
        目の開き具合を再マッピングして感度を高める，getEyeOpen関連の関数
        """
        return (self.clamp(ratio,low,high) - low) / (high - low)

    def getEyeOpen(self,results,side):
        """
        目の空き具合を計算
        参考：https://github.com/yeemachine/kalidokit/blob/main/src/FaceSolver/calcEyes.ts
        """
        high = 0.85 #目が開いていると見なされる確率
        low = 0.55  #目が閉じていると見なされる確率
        if side == 'right':
            eyeDistance =  self.eyeLidRatio_right(results)
        elif side == 'left':
            eyeDistance =  self.eyeLidRatio_left(results)
        maxRatio  =  0.285 #人間の目の幅と高さの比率はおよそ .3 
        ratio = self.clamp(eyeDistance / maxRatio, 0, 2)
        eyeOpenRatio = self.remap(ratio, low, high) 
        return eyeOpenRatio,ratio
            
    def main(self):
        cap = cv.VideoCapture(self.cap_device)
        cap.set(cv.CAP_PROP_FRAME_WIDTH, self.cap_width)
        cap.set(cv.CAP_PROP_FRAME_HEIGHT, self.cap_height)

        # モデルロード #############################################################
        mp_pose = mp.solutions.pose
        pose = mp_pose.Pose(
            # upper_body_only=upper_body_only,
            model_complexity = self.model_complexity,
            #enable_segmentation=enable_segmentation,   ###
            min_detection_confidence = self.min_detection_confidence,
            min_tracking_confidence = self.min_tracking_confidence,
        )
        #facemesh
        mp_face_mesh = mp.solutions.face_mesh # MLソリューションの顔メッシュインスタンス
        face_mesh = mp_face_mesh.FaceMesh(
            min_detection_confidence=0.5, min_tracking_confidence=0.5)  
        
        # FPS計測モジュール ########################################################
        # cvFpsCalc = CvFpsCalc(buffer_len=10)
        
        #画像取得
        while not rospy.is_shutdown():
            # display_fps = cvFpsCalc.get()

            # カメラキャプチャ #####################################################
            ret, image = cap.read()
            if not ret:
                break
            Pimage = cv.flip(image, 1)  # ミラー表示
            # debug_Pimage = copy.deepcopy(Pimage)

                #カメライメージのPublish ##########################################
            CamImage_forPublish = _numpy2multiarray(Int32MultiArray,image)
            self._CamImage_pub.publish(CamImage_forPublish)
            
            # 検出実施 #############################################################
            #mediapipe_pose
            Pimage = cv.cvtColor(image, cv.COLOR_BGR2RGB)
            results = pose.process(Pimage)
            #mediapipe_face
            face_results = face_mesh.process(Pimage)
            
            # 座標取得 #############################################################
            if results.pose_world_landmarks is not None:
                LMlist = list(results.pose_world_landmarks.landmark) 
                LM_Larm = [LMlist[15].x,LMlist[15].y,LMlist[15].z]
                LM_Rarm = [LMlist[16].x,LMlist[16].y,LMlist[16].z]
                LM_head = [LMlist[0].x,LMlist[0].y,LMlist[0].z]
                
            if results.pose_landmarks is not None:
                LMlist2 = list(results.pose_landmarks.landmark) 
                LM_Larm2 = [LMlist2[15].x,LMlist2[15].y,LMlist2[15].z]
                LM_Rarm2 = [LMlist2[16].x,LMlist2[16].y,LMlist2[16].z]
                LM_head2 = [LMlist2[0].x,LMlist2[0].y,LMlist2[0].z]


            if face_results.multi_face_landmarks:
                #開眼率
                r_eyeOpenRatio,r_ratio = self.getEyeOpen(face_results,'right')
                l_eyeOpenRatio,l_ratio = self.getEyeOpen(face_results,'left')
            if(r_eyeOpenRatio<1):
                self.blink.data = 'blink'
            elif(l_eyeOpenRatio<1):
                self.blink.data = 'blink' 
            else:
                self.blink.data = 'no_blink' 
            
            # 結果表示 ############################################################
            print("========================================================")
            if results.pose_world_landmarks is not None:
                array_forPublish = MsgPose(head=LM_head,Larm=LM_Larm,Rarm=LM_Rarm)
                array_forPublish2 = MsgPose(head=LM_head2,Larm=LM_Larm2,Rarm=LM_Rarm2)

                print("Landmark:",array_forPublish)
                print("Landmark2:",array_forPublish2)
                
                self._pose_pub.publish(array_forPublish)
            else:
                print("Landmark: None")
            
            if face_results.multi_face_landmarks:
                print("blink:" + self.blink.data)
                self._face_pub.publish(self.blink)
            else:
                print("blink:face_Landmark None")
            
            print("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^")
                        
        cap.release()
        cv.destroyAllWindows()
        
        
    # # World座標プロット ########################################################
    # if plot_world_landmark:
    #     import matplotlib.pyplot as plt
        
    #     fig = plt.figure()
    #     ax = fig.add_subplot(111, projection="3d")
    #     fig.subplots_adjust(left=0.0, right=1, bottom=0, top=1)

    # World座標プロット ###################################################
    # if plot_world_landmark:
    #     if results.pose_world_landmarks is not None:
    #         plot_world_landmarks(
    #             plt,
    #             ax,
    #             results.pose_world_landmarks,
    #         )



# def plot_world_landmarks(
#     plt,
#     ax,
#     landmarks,
#     visibility_th=0.5,
# ):
#     landmark_point = []

#     for index, landmark in enumerate(landmarks.landmark):
#         landmark_point.append(
#             [landmark.visibility, (landmark.x, landmark.y, landmark.z)])

#     face_index_list = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
#     right_arm_index_list = [11, 13, 15, 17, 19, 21]
#     left_arm_index_list = [12, 14, 16, 18, 20, 22]
#     right_body_side_index_list = [11, 23, 25, 27, 29, 31]
#     left_body_side_index_list = [12, 24, 26, 28, 30, 32]
#     shoulder_index_list = [11, 12]
#     waist_index_list = [23, 24]

#     # 顔
#     face_x, face_y, face_z = [], [], []
#     for index in face_index_list:
#         point = landmark_point[index][1]
#         face_x.append(point[0])
#         face_y.append(point[2])
#         face_z.append(point[1] * (-1))

#     # 右腕
#     right_arm_x, right_arm_y, right_arm_z = [], [], []
#     for index in right_arm_index_list:
#         point = landmark_point[index][1]
#         right_arm_x.append(point[0])
#         right_arm_y.append(point[2])
#         right_arm_z.append(point[1] * (-1))

#     # 左腕
#     left_arm_x, left_arm_y, left_arm_z = [], [], []
#     for index in left_arm_index_list:
#         point = landmark_point[index][1]
#         left_arm_x.append(point[0])
#         left_arm_y.append(point[2])
#         left_arm_z.append(point[1] * (-1))

#     # 右半身
#     right_body_side_x, right_body_side_y, right_body_side_z = [], [], []
#     for index in right_body_side_index_list:
#         point = landmark_point[index][1]
#         right_body_side_x.append(point[0])
#         right_body_side_y.append(point[2])
#         right_body_side_z.append(point[1] * (-1))

#     # 左半身
#     left_body_side_x, left_body_side_y, left_body_side_z = [], [], []
#     for index in left_body_side_index_list:
#         point = landmark_point[index][1]
#         left_body_side_x.append(point[0])
#         left_body_side_y.append(point[2])
#         left_body_side_z.append(point[1] * (-1))

#     # 肩
#     shoulder_x, shoulder_y, shoulder_z = [], [], []
#     for index in shoulder_index_list:
#         point = landmark_point[index][1]
#         shoulder_x.append(point[0])
#         shoulder_y.append(point[2])
#         shoulder_z.append(point[1] * (-1))

#     # 腰
#     waist_x, waist_y, waist_z = [], [], []
#     for index in waist_index_list:
#         point = landmark_point[index][1]
#         waist_x.append(point[0])
#         waist_y.append(point[2])
#         waist_z.append(point[1] * (-1))
            
#     ax.cla()
#     ax.set_xlim3d(-1, 1)
#     ax.set_ylim3d(-1, 1)
#     ax.set_zlim3d(-1, 1)

#     ax.scatter(face_x, face_y, face_z)
#     ax.plot(right_arm_x, right_arm_y, right_arm_z)
#     ax.plot(left_arm_x, left_arm_y, left_arm_z)
#     ax.plot(right_body_side_x, right_body_side_y, right_body_side_z)
#     ax.plot(left_body_side_x, left_body_side_y, left_body_side_z)
#     ax.plot(shoulder_x, shoulder_y, shoulder_z)
#     ax.plot(waist_x, waist_y, waist_z)
    
#     plt.pause(.001)

#     return




#プログラム起動時実行 #############################################################
if sys.argv:
    del sys.argv[1:]
rospy.init_node("ros_pose")
main_plogram = ros_mediapipe()

service_server() # モデルロード完了サービス受信

main_plogram.main()
while not rospy.is_shutdown():
    rospy.spin()