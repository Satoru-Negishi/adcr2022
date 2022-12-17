#!/usr/bin/env python

import rospy
import numpy as np
from test.msg import pose as MsgPose
from std_msgs.msg import UInt16MultiArray
from std_srvs.srv import Empty, EmptyResponse


# poseのworld_landmarkの座標について ###################################################
#   x座標 -1.0 , 1.0 （左 - 右）
#   y座標 -1.0 , 0.0 , 1.0 （下, 中心[腰] , 上）

CENTER = 90

NECK_MAX = 115
NECK_MIN = 65
ARM_MAX = 180
ARM_MIN = 0
WAIST_MAX = 180
WAIST_MIN = 0

#首サーボモータ　65-115

#pyfeatのモデル読み込み完了のサービス(サーバー) ##################################
def handle_service(req):
    rospy.loginfo('ros_motor: called')
    return EmptyResponse()

def service_server():
    s = rospy.Service('emotion_ready', Empty, handle_service)
    print('ros_motor: Ready to Serve')

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
        
        #     # I : ユーザの初期姿勢
        # self.ini_head_landmark = []
        # self.ini_Larm_landmark = []
        # self.ini_Rarm_landmark = []
        
        # self.initialize = True #初期化実行管理
        
        # ROS_PUB_SUB ############################################################
        self._servo_pub = rospy.Publisher("servo", UInt16MultiArray, queue_size=1)
   
    # def position_initialize(self,msg):
    #     self.initialize = False
    #     self.ini_head_landmark = list(msg.head)
    #     self.ini_Larm_landmark = list(msg.Larm)
    #     self.ini_Rarm_landmark = list(msg.Rarm)
    #     rospy.loginfo("landmark initialize")
    #     rospy.loginfo("[ini_head]"+self.ini_head_landmark)
    #     rospy.loginfo("[ini_Larm]"+self.ini_Larm_landmark)
    #     rospy.loginfo("[ini_Rarm]"+self.ini_Rarm_landmark)
       
    # def head_landmark2servo(self):
        
    
    def arm_landmark2servo(self,landmark,LR):
        if LR == 'left':
            self.Larm_Pservo = self.Larm_Nservo
            self.Larm_Nservo = round(landmark[1] * 180)
            if self.Larm_Nservo < 0:
                self.Larm_Nservo = 0
            if self.Larm_Nservo > 180:
                self.Larm_Nservo = 180
            rospy.loginfo("L_landmark"+landmark[1]+"[Larm]: "+self.Larm_Nservo)
        elif LR == 'right':
            self.Rarm_Pservo = self.Rarm_Nservo
            self.Rarm_Nservo = round(landmark[1]*180)
            if self.Rarm_Nservo < 0:
                self.Rarm_Nservo = 0
            if self.Rarm_Nservo > 180:
                self.Rarm_Nservo = 180
            rospy.loginfo("R_landmark"+landmark[1]+"[Rarm]: "+self.Larm_Nservo)
        else:
            pass 
        return

    def waist_landmark2servo(self,landmark):
        pass
               
        
         
        
    def callback(self,msg): #pose_mediapipe受信時実行関数
        self.head_landmark = list(msg.head)
        self.Larm_landmark = list(msg.Larm)
        self.Rarm_landmark = list(msg.Rarm)

        # rospy.loginfo(msg)
        # if self.initialize:
        #     self.position_initialize(msg)
        # if 
        self.arm_landmark2servo(self.Larm_landmark,'left')
        self.arm_landmark2servo(self.Rarm_landmark,'right')
        
        array_forPublish = UInt16MultiArray([90,self.Larm_Nservo,self.Rarm_Nservo,90])
        self._servo_pub.publish(array_forPublish)
        
    def main(self):
        self._servo_sub = rospy.Subscriber("pose_mediapipe", MsgPose, self.callback)

        


# main() ########################################################################   
rospy.init_node('pose2servo')
motors = Landmark2Servo()

# service_server() # モデルロード完了サービス受信

motors.main()
while not rospy.is_shutdown():
    rospy.spin()