#!/usr/bin/env python

import rospy
import numpy as np
from test.msg import pose as MsgPose
from std_msgs.msg import UInt8, MultiArrayDimension
from std_msgs.msg import Empty as msg_Empty
from std_srvs.srv import Empty


# poseのworld_landmarkの座標について ###################################################
#   x座標 -1.0 , 1.0 （左 - 右）
#   y座標 -1.0 , 0.0 , 1.0 （下, 中心[腰] , 上）
#   首サーボモータ　65-115(ギリギリ)
CENTER = 90

NECK_MAX = 105
NECK_MIN = 75
ARM_MAX = 180
ARM_MIN = 0
WAIST_MAX = 180
WAIST_MIN = 0

NECK_INI = 90
LARM_INI = 180
RARM_INI = 0
WAIST_INI = 90

def _numpy2multiarray(multiarray_type, np_array):
    """Convert numpy.ndarray to multiarray"""
    multiarray = multiarray_type()
    multiarray.layout.dim = [MultiArrayDimension("dim%d" % i, np_array.shape[i], np_array.shape[i] * np_array.dtype.itemsize) for i in range(np_array.ndim)]
    multiarray.data = np_array.reshape(1, -1)[0].tolist()
    return multiarray

#キー入力完了のサービス(クライアント) ##################################
def key_call_service():
    # rospy.loginfo('[KEY][CLIENT]ros_motor: waiting service')
    rospy.wait_for_service('EnterKeys_ready')
    try:
        service = rospy.ServiceProxy('EnterKeys_ready', Empty)
        response = service()
    except rospy.ServiceException as  e:
        rospy.loginfo("[KEY][CLIENT]ros_motor: service call failed: %s" % e)

#pyfeatのモデル読み込み完了のサービス(クライアント) ##################################
def emo_call_service():
    # rospy.loginfo('[EMO][CLIENT]ros_motor: waiting service')
    rospy.wait_for_service('emotion_ready')
    try:
        service = rospy.ServiceProxy('emotion_ready', Empty)
        response = service()
    except rospy.ServiceException as  e:
        rospy.loginfo("[EMO][CLIENT]ros_motor: service call failed: %s" % e)


class Landmark2Servo(object):
    def __init__(self):
        #servo制御角度用変数 #####################################################
        self.neck_forPublish = UInt8(NECK_INI)
        self.Larm_forPublish = UInt8(LARM_INI)
        self.Rarm_forPublish = UInt8(LARM_INI)
        self.waist_forPublish = UInt8(WAIST_INI)
        
            # N : 現在の角度
        self.neck_Nservo = NECK_INI
        self.Larm_Nservo = LARM_INI
        self.Rarm_Nservo = RARM_INI
        self.waist_Nservo = WAIST_INI
        
            # P : 過去の角度
        self.neck_Pservo = NECK_INI
        self.Larm_Pservo = LARM_INI
        self.Rarm_Pservo = RARM_INI
        self.waist_Pservo = WAIST_INI
        
        # self.array_forPublish = []
        
        # ROS_PUB_SUB ############################################################
        # self._servo_pub = rospy.Publisher("servo", UInt16MultiArray, queue_size=1)
        self._Larm_pub = rospy.Publisher("Larm_servo", UInt8, queue_size=1, latch=True)
        self._Rarm_pub = rospy.Publisher("Rarm_servo", UInt8, queue_size=1, latch=True)
        self._head_pub = rospy.Publisher("head_servo", UInt8, queue_size=1, latch=True)
        self._waist_pub = rospy.Publisher("waist_servo", UInt8, queue_size=1, latch=True)       
       
    def head_landmark2servo(self,landmark):
        self.neck_Pservo = self.neck_Nservo
        self.neck_Nservo = 90 + round((25*(landmark[1]-0.612)*1000)/32)
        if self.neck_Nservo < NECK_MIN:
            self.neck_Nservo = NECK_MIN
        if self.neck_Nservo > NECK_MAX:
            self.neck_Nservo = NECK_MAX
        # print("W_landmark:",landmark[1]," [head]: ",self.neck_Nservo)
        return        
        
    def arm_landmark2servo(self,landmark,LR):
        if LR == 'left':
            self.Larm_Pservo = self.Larm_Nservo
            self.Larm_Nservo = 180 - round(landmark[1] * 180)
            if self.Larm_Nservo < ARM_MIN:
                self.Larm_Nservo = ARM_MIN
            if self.Larm_Nservo > ARM_MAX:
                self.Larm_Nservo = ARM_MAX
            # print("L_landmark:",landmark[1]," [Larm]: ",self.Larm_Nservo)
        elif LR == 'right':
            self.Rarm_Pservo = self.Rarm_Nservo
            self.Rarm_Nservo = round(landmark[1]*180)
            if self.Rarm_Nservo < ARM_MIN:
                self.Rarm_Nservo = ARM_MIN
            if self.Rarm_Nservo > ARM_MAX:
                self.Rarm_Nservo = ARM_MIN
            # print("R_landmark:",landmark[1]," [Rarm]: ",self.Rarm_Nservo)
        else:
            pass 
        return

    def waist_landmark2servo(self,landmark):
        self.waist_Pservo = self.waist_Nservo
        self.waist_Nservo = 90 + round(90*landmark[0]*100 / 12)
        if self.waist_Nservo < WAIST_MIN:
            self.waist_Nservo = WAIST_MIN
        if self.waist_Nservo > WAIST_MAX:
            self.waist_Nservo = WAIST_MAX
        # print("W_landmark:",landmark[0]," [waist]: ",self.waist_Nservo)
        return        
        
    def callback(self,msg): #pose_mediapipe受信時実行関数
        self.head_landmark = list(msg.head)
        self.Larm_landmark = list(msg.Larm)
        self.Rarm_landmark = list(msg.Rarm)

        self.arm_landmark2servo(self.Larm_landmark,'left')
        self.arm_landmark2servo(self.Rarm_landmark,'right')
        self.waist_landmark2servo(self.head_landmark)
        self.head_landmark2servo(self.head_landmark)
        
        # rospy.loginfo("[H]: %d [L]: %d, [R]: %d [W]: %d",self.neck_Nservo,self.Larm_Nservo,self.Rarm_Nservo,self.waist_Nservo)
        
        # np_forservo = np.array([self.neck_Nservo,self.Larm_Nservo,self.Rarm_Nservo,self.waist_Nservo])        
        # self._servo_pub.publish(self.array_forPublish)
        
        self.neck_forPublish = UInt8(self.neck_Nservo)
        self.Larm_forPublish = UInt8(self.Larm_Nservo)
        self.Rarm_forPublish = UInt8(self.Rarm_Nservo)
        self.waist_forPublish = UInt8(self.waist_Nservo)
        
    def motor_server(self,msg):
        # self._servo_pub.publish(self.array_forPublish)
        self._head_pub.publish(self.neck_forPublish)
        self._Larm_pub.publish(self.Larm_forPublish)
        self._Rarm_pub.publish(self.Rarm_forPublish)
        self._waist_pub.publish(self.waist_forPublish)
        
 
    
    def main(self):
        self._servo_sub = rospy.Subscriber("pose_mediapipe", MsgPose, self.callback)
        self._ready_sub = rospy.Subscriber("motor_ready", msg_Empty,self.motor_server ,queue_size=1)

        


# main() ########################################################################   
rospy.init_node('pose2servo')
key_call_service() #キー入力完了をリクエスト

emo_call_service() #モデルロード完了をリクエスト

motors = Landmark2Servo()

motors.main()
while not rospy.is_shutdown():
    rospy.spin()