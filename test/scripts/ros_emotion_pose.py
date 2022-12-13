#!/usr/bin/env python
# -*- coding: utf-8 -*-

#共通
import cv2 as cv
import numpy as np
from std_msgs.msg import String, Float32MultiArray
from test.msg import pose as MsgPose

#mediapipe_pose
import copy
import argparse
import mediapipe as mp
from utils import CvFpsCalc

#emotion_detect
import rospy
import cv2
from feat import Detector

class Emotion_Pose(object):
    def __init__(self):
        #モデル指定 #################################################################
        face_model = "retinaface"
        landmark_model = "mobilenet"
        au_model = "rf"
        emotion_model = "resmasknet"
        self.detector = Detector(
            face_model="retinaface",
            landmark_model="mobilefacenet",
            au_model='jaanet',
            emotion_model="fer",
            facepose_model="img2pose",
        )

        #ビデオ表示関係 #################################################################
        # self.cap = cv2.VideoCapture(0)
        # self.frameWidth = 640
        # self.frameHeight = 480

        #感情リスト #################################################################
        self.emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]
        self.emotion = String()

        #ROS_PUB_SUB #################################################################
        self._emotion_pub = rospy.Publisher("emo_feat", String, queue_size=1)
        self._pose_pub = rospy.Publisher("pose_mediapipe", MsgPose, queue_size=1)  #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        
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
    
    def emotion_detect(self,image):
        #特徴抽出，感情推定
        faces = self.detector.detect_faces(image)
        landmarks = self.detector.detect_landmarks(image, faces)
        emo_pred = self.detector.emotion_model.detect_emo(image, landmarks)
        poses = self.detector.detect_facepose(image, faces, landmarks)
        pred_index = np.argmax(emo_pred[0])        
        #結果表示
        # print(self.emolist[pred_index])
        
        return self.emolist[pred_index], poses
        
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
        # FPS計測モジュール ########################################################
        cvFpsCalc = CvFpsCalc(buffer_len=10)
        
        #画像取得
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            display_fps = cvFpsCalc.get()

            # カメラキャプチャ #####################################################
            ret, image = cap.read()
            if not ret:
                break
            Pimage = cv.flip(image, 1)  # ミラー表示
            debug_Pimage = copy.deepcopy(Pimage)
            
            Eimage = np.expand_dims(image,0)
            
            # 検出実施 #############################################################
            Pimage = cv.cvtColor(image, cv.COLOR_BGR2RGB)
            results = pose.process(Pimage)
                                 
            emotion.data, head_pose = self.emotion_detect(Eimage)
            
            # 座標取得 #############################################################
            if results.pose_world_landmarks is not None:
                LMlist = list(results.pose_world_landmarks.landmark) 
                LM_Lshoulder = [LMlist[11].x,LMlist[11].y,LMlist[11].z]
                LM_Larm = [LMlist[15].x,LMlist[15].y,LMlist[15].z]
                LM_Rshoulder = [LMlist[12].x,LMlist[12].y,LMlist[12].z]
                LM_Rarm = [LMlist[16].x,LMlist[16].y,LMlist[16].z]
            
            if head_pose != None:
                LM_head = [head_pose[0][0][0][0], head_pose[0][0][0][2]]    
            
            # 結果表示 ############################################################
            print("========================================================")
            print("Emotion:" + emotion.data)
            if results.pose_world_landmarks is not None:
                array_forPublish = MsgPose(head=LM_head,Larm=LM_Larm,Rarm=LM_Rarm)
                print("Landmark:",array_forPublish)
                self._pose_pub.publish(array_forPublish)

            else:
                print("Landmark: None")
            print("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^")
            
            #Publish ##############################################################
            self._emotion_pub.publish(self.emotion)    #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

            # show_img = cv2.resize(img, (self.frameWidth, self.frameHeight))
            # cv2.imshow('Video', show_img)


            rate.sleep()
        cap.release()
        cv.destroyAllWindows()

#プログラム起動時実行 #############################################################
rospy.init_node("emotion_detect")
emotion = Emotion_Pose()
emotion.main()

while not rospy.is_shutdown():
    rospy.spin()

# if __name__ == "__main__":
#     rospy.init_node("emotion_detect")
#     emotion = Emo_feat()
#     emotion.emotion_detect()
#     # try:
#     #     rospy.spin()
#     # except KeyboardInterrupt:
#     #     pass    

# if __name__ == '__main__':
#     main()