## py-featを使用したリアルタイム顔感情認識
## 顔方向認識
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
    au_model='jaanet',
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
    img = cv2.resize(img, (frameWidth, frameHeight))
    cv2.imshow('Video', img)
    img = np.expand_dims(img,0)

    #特徴抽出，感情推定，顔方向推定
    faces = detector.detect_faces(img)

    # 顔検出判定
    if faces == [[]]:
        print("No faces found")
        continue

    landmarks = detector.detect_landmarks(img, faces)
    emo_pred = detector.emotion_model.detect_emo(img, landmarks)
    poses = detector.detect_facepose(img, faces, landmarks)
    
    #結果表示
    pred_index = np.argmax(emo_pred[0])
    print(emolist[pred_index])

    #下向きが負[deg]
    print("pitch", poses[0][0][0][0])
    # 左向きが負[deg]
    print("yaw", poses[0][0][0][2])