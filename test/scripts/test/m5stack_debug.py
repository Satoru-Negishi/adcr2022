#!/usr/bin/env python
import rospy
import numpy as np
from std_msgs.msg import String
from std_msgs.msg import UInt16

class Test_m5(object):
    def __init__(self):
        self._emo_sub = rospy.Subscriber("emo", String, self.cb_emo,queue_size=1)
        self._face_sub = rospy.Subscriber("face", String, self.cb_face,queue_size=1)
        self._info_sub = rospy.Subscriber("info", String, self.cb_info,queue_size=1)

        
    def cb_emo(self,msg):
        rospy.loginfo("[emo]: %s", msg.data)
        
    def cb_face(self,msg):
        rospy.loginfo("[face]: %s", msg.data)

    def cb_info(self,msg):
        rospy.loginfo("[info]: %s", msg.data)
        
rospy.init_node('m5_debug')
motor = Test_m5()       
while not rospy.is_shutdown():
    rospy.spin()