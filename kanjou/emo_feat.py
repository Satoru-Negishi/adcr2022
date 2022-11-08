## py-featを使用したリアルタイム顔感情認識

import cv2
import numpy as np
from feat import Detector
frameWidth = 640
frameHeight = 480

face_model = "retinaface"
landmark_model = "mobilenet"
au_model = "rf"
emotion_model = "resmasknet"
detector = Detector(
    face_model="retinaface",
    landmark_model="mobilefacenet",
    au_model='svm',
    emotion_model="resmasknet",
    facepose_model="img2pose",
)

cap = cv2.VideoCapture(0)
emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]

while True:
    ret, img = cap.read()
    show_img = cv2.resize(img, (frameWidth, frameHeight))
    cv2.imshow('Video', show_img)
    img = np.expand_dims(img,0)
    faces = detector.detect_faces(img)
    landmarks = detector.detect_landmarks(img, faces)
    emo_pred = detector.detect_emotions(frame=img, facebox=faces, landmarks=landmarks)
    
    pred_index = np.argmax(emo_pred[0])
    print(emolist[pred_index])