#!/usr/bin/env python
## py-featを使用したリアルタイム顔感情認識
import rospy
import cv2
import numpy as np
from feat import Detector
from std_msgs.msg import String

class Emo_feat(object):
    def __init__(self):
        #モデル指定
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

        #ビデオ表示関係
        self.cap = cv2.VideoCapture(0)
        self.frameWidth = 640
        self.frameHeight = 480

        #感情リスト
        self.emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]
        self.emotion = String()

        self._string_pub = rospy.Publisher("emo_feat", String, queue_size=1)
    
    def emotion_detect(self):
        #画像取得
        # rate = rospy.Rate()
        while not rospy.is_shutdown():
            ret, img = self.cap.read()
            show_img = cv2.resize(img, (self.frameWidth, self.frameHeight))
            cv2.imshow('Video', show_img)
            img = np.expand_dims(img,0)
            print(img)
            #特徴抽出，感情推定
            faces = self.detector.detect_faces(img)
            landmarks = self.detector.detect_landmarks(img, faces)
            emo_pred = self.detector.emotion_model.detect_emo(img, landmarks)
            
            #結果表示
            pred_index = np.argmax(emo_pred[0])
            # print(self.emolist[pred_index])
            self.emotion.data = self.emolist[pred_index]
            self._string_pub.publish(self.emotion)
            # rate.sleep()

rospy.init_node("emotion_detect")
emotion = Emo_feat()
emotion.emotion_detect()

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