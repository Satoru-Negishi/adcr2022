#!/usr/bin python

import rospy
rospy.init_node("filter")
while not rospy.is_shutdown():
    rospy.spin()