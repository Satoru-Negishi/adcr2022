## py-featを使用したリアルタイム顔感情認識
## 顔方向認識
import cv2
import numpy as np
import torch
from PIL import Image
from feat import Detector
from torchvision import transforms

def cv2pil(imgCV):
    imgCV_RGB = imgCV[:, :, ::-1] # H方向とW方向はそのままに、BGRを逆順にする
    imgPIL = Image.fromarray(imgCV_RGB)
    return imgPIL

#モデル指定
detector = Detector(
    face_model="retinaface",
    landmark_model="mobilefacenet",
    au_model='svm',
    emotion_model="resmasknet",
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

    # img = cv2.imread("imtest.jpg")

    img = cv2.resize(img, (frameWidth, frameHeight))
    cv2.imshow('Video', img)
    img = cv2pil(img)
    img = transforms.PILToTensor()(img)
    img = img.numpy()
    img = np.expand_dims(img,0)
    img = torch.from_numpy(img)
    #特徴抽出，感情推定，顔方向推定
    faces = detector.detect_faces(img)

    # 顔検出判定
    if faces == [[]]:
        print("No faces found")
        continue

    landmarks = detector.detect_landmarks(img, faces)
    emo_pred = detector.detect_emotions(img,faces, landmarks)
    poses = detector.detect_facepose(img, landmarks)
    
    #結果表示
    pred_index = np.argmax(emo_pred[0])
    print(emolist[pred_index])

    #下向きが負[deg]
    print("pitch", poses[0][0][0][0])
    # 左向きが負[deg]
    print("yaw", poses[0][0][0][2])