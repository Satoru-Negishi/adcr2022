import cv2
import math
import mediapipe as mp

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_pose = mp.solutions.pose
pre_angle_s = 0 # 肩の角度の変数

cap = cv2.VideoCapture(0)
with mp_pose.Pose(
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as pose:
  while cap.isOpened():
    success, image = cap.read()
    if not success:
      print("Ignoring empty camera frame.")
      continue

    image.flags.writeable = False
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = pose.process(image)

    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    mp_drawing.draw_landmarks(
        image,
        results.pose_landmarks,
        mp_pose.POSE_CONNECTIONS,
        landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style())
    if results.pose_landmarks:
      z1=results.pose_landmarks.landmark[12].z #z1,y1 = shoulder
      z2=results.pose_landmarks.landmark[14].z #z2,y2 = elbow
      y1=results.pose_landmarks.landmark[12].y
      y2=results.pose_landmarks.landmark[14].y
      
      if z2<z1:
        angle_s = int(math.degrees(math.atan((y2-y1)/(z1-z2))))
      else:# z2>=z1
        angle_s = int(180+math.degrees(math.atan((y2-y1)/(z1-z2))))

      if angle_s < 0:
        angle_s = 0
      elif angle_s > 180:
        angle_s = 180

      print("\r"+str(angle_s))  #腕を前に出すと0度になる

    cv2.imshow('mediapipe pose', cv2.flip(image, 1))
    if cv2.waitKey(5) & 0xFF == 27: #ESCで終了
      break
cap.release()
