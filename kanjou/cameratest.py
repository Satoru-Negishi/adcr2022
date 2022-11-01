import cv2

# Video
frameWidth = 640
frameHeight = 480

#Video Source
#cap = cv2.VideoCapture('videos/traffic.mp4') #自分のmp4のpathを入力
cap = cv2.VideoCapture(0)



while True:
    ret, img = cap.read()
    img = cv2.resize(img, (frameWidth, frameHeight))
    cv2.imshow('Video', img)
    print('ret=', ret)

    # qを押すと止まる。
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break