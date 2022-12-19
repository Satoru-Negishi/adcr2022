#!/usr/bin/env python
import rospy
import numpy as np
from std_msgs.msg import String
from std_msgs.msg import UInt16

class Test_servo(object):
    def __init__(self):
        self.emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]
        self.servo_angle = np.array([0, 30, 60, 90, 120, 150, 180])
        
        self._servo_sub = rospy.Subscriber("emo_feat", String, self.callback,queue_size=1)
        self._servo_pub = rospy.Publisher("servo", UInt16, queue_size=1)
        
    def callback(self,msg):
        rospy.loginfo("log: %s", msg.data)
        print("index",self.emolist.index(msg.data))
        
        pub_angle = self.servo_angle[self.emolist.index(msg.data)]
        self._servo_pub.publish(pub_angle)
        
rospy.init_node('emo_servo')
motor = Test_servo()       
while not rospy.is_shutdown():
    rospy.spin()

# if __name__ == "__main__":
#     rospy.init_node('emo_servo')
#     motor = Test_servo()
#     try:
#         rospy.spin()
#     except KeyboardInterrupt:
#         pass