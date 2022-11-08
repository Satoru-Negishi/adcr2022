## py-featを使用したリアルタイム顔感情認識

import cv2
import numpy as np
from feat import Detector

#モデル指定
face_model = "retinaface"
landmark_model = "mobilenet"
au_model = "rf"
emotion_model = "resmasknet"
detector = Detector(
    face_model="retinaface",
    landmark_model="mobilefacenet",
    au_model='svm',
    emotion_model="fer",
    facepose_model="img2pose",
)

#ビデオ表示関係
cap = cv2.VideoCapture(0)
frameWidth = 640
frameHeight = 480

#感情リスト
emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]

while True:
    #画像取得
    ret, img = cap.read()
    show_img = cv2.resize(img, (frameWidth, frameHeight))
    cv2.imshow('Video', show_img)
    img = np.expand_dims(img,0)

    #特徴抽出，感情推定
    faces = detector.detect_faces(img)
    landmarks = detector.detect_landmarks(img, faces)
    emo_pred = detector.emotion_model.detect_emo(img, landmarks)
    
    #結果表示
    pred_index = np.argmax(emo_pred[0])
    print(emolist[pred_index])