#!/usr/bin/env python

import rospy
import numpy as np
from test.msg import pose as MsgPose
from std_msgs.msg import UInt16

# poseのlandmarkの座標について ###################################################
#   x座標 0 - 960 （左 - 右）
#   y座標 0 - 540 （上 - 下）

CENTER = 90

NECK_MAX = 115
NECK_MIN = 65
ARM_MAX = 180
ARM_MIN = 0
WAIST_MAX = 180
WAIST_MIN = 0

#首サーボモータ　65-115

class Landmark2Servo(object):
    def __init__(self):
        #servo制御角度用変数 #####################################################
            # N : 現在の角度
        self.Larm_Nservo = 0
        self.Rarm_Nservo = 0
        self.waist_Nservo = 0
        self.neck_Nservo = 0
        
            # P : 過去の角度
        self.Larm_Pservo = 0
        self.Rarm_Pservo = 0
        self.waist_Pservo = 0
        self.neck_Pservo = 0
        
        # I : ユーザの初期姿勢
        self.ini_head_landmark = []
        self.ini_Larm_landmark = []
        self.ini_Rarm_landmark = []
        
        self.initialize = True #初期化実行管理
        
        # ROS_PUB_SUB ############################################################
        self._servo_pub = rospy.Publisher("servo", UInt16, queue_size=1)
        
    def callback(self,msg): #pose_mediapipe受信時実行関数
        # rospy.loginfo(msg)
        if self.initialize:
            self.position_initialize(msg)
        
    def position_initialize(self,msg):
        self.initialize = False
        self.ini_head_landmark = list(msg.head)
        self.ini_Larm_landmark = list(msg.Larm)
        self.ini_Rarm_landmark = list(msg.Rarm)
        print(self.ini_head_landmark)
        print(self.ini_Larm_landmark)
        print(self.ini_Rarm_landmark)
    
    def main(self):
        self._servo_sub = rospy.Subscriber("pose_mediapipe", MsgPose, self.callback)

        


# main() ########################################################################   
rospy.init_node('pose2servo')
motors = Landmark2Servo()
motors.main()
while not rospy.is_shutdown():
    rospy.spin()