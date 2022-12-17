#!/usr/bin/env python
import rospy
import numpy as np
from std_msgs.msg import String
from std_msgs.msg import UInt16

class Test_ar(object):
    def __init__(self):
        self._larm_sub = rospy.Subscriber("larm", String, self.cb_larm,queue_size=1)
        self._rarm_sub = rospy.Subscriber("rarm", String, self.cb_rarm,queue_size=1)

        
    def cb_emo(self,msg):
        print("[Larm]: ",msg.data)        
    def cb_face(self,msg):
        print("[Rarm]: ", msg.data)

        
rospy.init_node('m5_debug')
motor = Test_ar()       
while not rospy.is_shutdown():
    rospy.spin()