#!/usr/bin/env python
## py-featを使用したリアルタイム顔感情認識
import rospy
import cv2
import numpy as np
from feat import Detector
from std_msgs.msg import String, Int32MultiArray, MultiArrayDimension
from std_srvs.srv import Empty,EmptyResponse
from torchvision import transforms
from PIL import Image
import torch

import time

#メッセージ型のmultiarryをnumpyに変換 #################################################
def _multiarray2numpy(pytype, dtype, multiarray):
    """Convert multiarray to numpy.ndarray"""
    dims = [540,960,3]
    # dims = map(lambda x: x.size, multiarray.layout.dim)
    # print(multiarray.layout.dim)
    return np.array(multiarray.data, dtype=pytype).reshape(dims).astype(dtype)

#キー読み込み完了のサービス(サーバー) ##################################
def handle_service(req):
    rospy.loginfo('ros_emotion: called')
    return EmptyResponse()

def service_server():
    s = rospy.Service('EnterKeys_ready', Empty, handle_service)
    print('ros_emotion: Ready to Serve')


#pyfeatのモデル読み込み完了のサービス(クライアント) ##################################
def call_service():
    rospy.loginfo('waiting service')
    rospy.wait_for_service('emotion_ready')
    try:
        service = rospy.ServiceProxy('emotion_ready', Empty)
        response = service()
    except rospy.ServiceException as  e:
        print("service call failed: %s" % e)
        
def cv2pil(imgCV):
    imgCV_RGB = imgCV[:, :, ::-1] # H方向とW方向はそのままに、BGRを逆順にする
    imgPIL = Image.fromarray(imgCV_RGB)
    return imgPIL

class Emo_feat(object):
    def __init__(self):
        #モデル指定 ###################################################################################
        face_model = "retinaface"
        landmark_model = "mobilenet"
        au_model = "rf"
        emotion_model = "resmasknet"
        self.detector = Detector(
            face_model="retinaface",
            landmark_model="mobilefacenet",
            au_model='svm',
            emotion_model="resmasknet",
            facepose_model="img2pose",
        )

        #感情リスト ###################################################################################
        self.emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]
        self.emotion = String()

        #ROS_PUB_SUB ###################################################################################
        self._string_pub = rospy.Publisher("emo_feat", String, queue_size=1)
    
    def emotion_detect(self,msg):
        print("5")
        #画像取得 ###################################################################################
        # while not rospy.is_shutdown():
        #     ret, img = self.cap.read()
        #     show_img = cv2.resize(img, (self.frameWidth, self.frameHeight))
        #     cv2.imshow('Video', show_img)
        img = _multiarray2numpy(int,np.uint8, msg)
        img = cv2pil(img)
        img = transforms.PILToTensor()(img)
        img = img.numpy()
        img = np.expand_dims(img,0)
        img = torch.from_numpy(img)

        #特徴抽出，感情推定 ###################################################################################
        faces = self.detector.detect_faces(img)
        # start = time.time()
        if faces == [[]]:
            print("No faces found")
        else:
            landmarks = self.detector.detect_landmarks(img, faces)
            print(landmarks)
            emo_pred = self.detector.emotion_model.detect_emo(img, faces, landmarks)
            # poses = self.detector.detect_facepose(img, faces, landmarks) #実行時負荷増
            # end = time.time()
            # print("time:",end-start)
            #結果表示 ###################################################################################
            pred_index = np.argmax(emo_pred[0])
            print(self.emolist[pred_index])
            # #下向きが負[deg]
            # print("pitch", poses[0][0][0][0])
            # # 左向きが負[deg]
            # print("yaw", poses[0][0][0][2])
            self.emotion.data = self.emolist[pred_index]
            self._string_pub.publish(self.emotion)
    
    def main(self):
        print("4")
        self._CamImage_sub = rospy.Subscriber("cam_image", Int32MultiArray, self.emotion_detect)
print("1")
rospy.init_node("emotion_detect")
print("2")
emotion = Emo_feat()

#service_server() #キー入力完了サービス受信
call_service() # モデルロード完了サービス送信[変更]

print("3")
emotion.main()

while not rospy.is_shutdown():
    rospy.spin()
