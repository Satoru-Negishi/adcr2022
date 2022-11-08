##DeepFaceを使用した顔感情認識

from deepface import DeepFace
import cv2


# Video
frameWidth = 640
frameHeight = 480

cap = cv2.VideoCapture(0)



while True:
    ret, img = cap.read()
    img = cv2.resize(img, (frameWidth, frameHeight))
    cv2.imshow('Video', img)
    demo = DeepFace.analyze(img, actions = ['emotion'])
    print(demo['dominant_emotion'])
    # qを押すと止まる。
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break