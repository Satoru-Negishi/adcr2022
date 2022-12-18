#!/usr/bin/env python
import rospy
import random
import numpy as np
from std_msgs.msg import String,UInt16MultiArray,MultiArrayDimension
from std_srvs.srv import Empty
import tkinter as tk
import sys

#numpyをメッセージ型のmultiarryに変換 #################################################
def _numpy2multiarray(multiarray_type, np_array):
    """Convert numpy.ndarray to multiarray"""
    multiarray = multiarray_type()
    multiarray.layout.dim = [MultiArrayDimension("dim%d" % i, np_array.shape[i], np_array.shape[i] * np_array.dtype.itemsize) for i in range(np_array.ndim)]
    multiarray.data = np_array.reshape(1, -1)[0].tolist()
    return multiarray

def _multiarray2numpy(pytype, dtype, multiarray):
    """Convert multiarray to numpy.ndarray"""
    # dims = map(lambda x: x.size, multiarray.layout.dim)
    dims = [4]
    return np.array(multiarray.data, dtype=pytype).reshape(dims).astype(dtype)

#キー入力読み込み完了のサービス(クライアント) ##################################
def call_service():
    rospy.loginfo('waiting service')
    rospy.wait_for_service('EnterKeys_ready')
    try:
        service = rospy.ServiceProxy('EnterKeys_ready', Empty)
        response = service()
    except rospy.ServiceException as  e:
        print("service call failed: %s" % e)
    
class main_infomation(object):
    def __init__(self):
        #########################################################################
        self.blink = String()
        self.emotion = String()
        #ROS_PUB_SUB #################################################################
        self._mian_pose_pub = rospy.Publisher("main_pose", UInt16MultiArray, queue_size=1)  
        self._main_blink_pub = rospy.Publisher("main_blink", String, queue_size=1)
        self._main_emotion_pub = rospy.Publisher("main_emotion", String, queue_size=1)
        
    def key_reception(self):
        """
        キーボード入力を受け付ける
        """
        #表情表出フィルタの有無の選択
        rospy.loginfo('Please select whether or not there is an facial expression filter. [yes or no] ')
        try:
            while True:
                exp_fil = input()
                if (exp_fil == 'yes') or (exp_fil== 'no'):
                    break
            rospy.loginfo("facial expression filter: %s", exp_fil)
        except KeyboardInterrupt:
            sys.exit()
        #使用するモータの選択
        rospy.loginfo('Enter all the motors to be used from [head,waist,arm], or enter ALL if all motors are used.  (ex)When using the head and waist => head,waist')
        try:
            while True:
                Inapplicable_characters = False
                motor_sel = input()
                motor_sel_l = motor_sel.split(',')
                for word in motor_sel_l:
                    if not word in ['head','waist','arm','ALL']:
                        Inapplicable_characters = True
                if Inapplicable_characters == False:
                    break
            rospy.loginfo("motor used: %s", motor_sel_l)
            unused = list(set(['head','waist','arm'])^ set(motor_sel_l))
            rospy.loginfo("%s",unused)
        except KeyboardInterrupt:
            sys.exit()
        #瞬き検出の有無の選択
        rospy.loginfo('Use blink detection? [yes or no]')
        try:
            while True:
                blink_detc = input()
                if (blink_detc == 'yes') or (blink_detc== 'no'):
                    break
            rospy.loginfo("Use blink detection?: %s", blink_detc)
        except KeyboardInterrupt:
            sys.exit()
        return exp_fil,motor_sel_l,blink_detc
  
    def pose_messageCb(self,msg,motor_sel_l):
        """
        モータ
        """
        if (('ALL' in motor_sel_l) or (len(motor_sel_l) == 3)):
            self._mian_pose_pub.publish(msg)
        else:
            unused = list(set(['head','waist','arm'])^ set(motor_sel_l))
            print(unused)
            nparray_msg=_multiarray2numpy(int, np.uint16, msg)
            for site in unused:
                if site == 'head':
                    nparray_msg[0] = 90
                elif site == 'waist':
                    nparray_msg[3] = 90
                elif site == 'arm':
                    nparray_msg[1] = 180
                    nparray_msg[2] = 0
            f_msg = _numpy2multiarray(UInt16MultiArray,nparray_msg)
            self._mian_pose_pub.publish(f_msg)

    def blink_messageCb(self,msg,blink_detc):
        """
        瞬き
        """
        if blink_detc == 'yes':
            self.blink.data = msg.data #no_blink,blink
            self._main_blink_pub.publish(self.blink)
        else:
            self.blink.data =random.choice(['no_blink','no_blink','no_blink','blink'])  
            self._main_blink_pub.publish(self.blink)

    def emo_messageCb(self,msg,exp_fil):
        """
        顔表情
        """
        if exp_fil == 'yes': #フィルタあり
            if msg.data in ['anger', 'disgust','fear']:
                self.emotion.data = 'neutral'
            else:
                self.emotion.data = msg.data
            self._main_emotion_pub.publish(self.emotion)  
        else:
            self.emotion.data = msg.data
            self._main_emotion_pub.publish(self.emotion)       
    
    def main(self, exp_fil, motor_sel_l, blink_detc):
        self._mian_pose_sub = rospy.Subscriber("servo", UInt16MultiArray, self.pose_messageCb, motor_sel_l)  
        self._main_blink_sub = rospy.Subscriber("face_mediapipe", String, self.blink_messageCb, blink_detc)
        self._main_emotion_sub = rospy.Subscriber("emo_feat", String, self.emo_messageCb, exp_fil)
        
                    
#プログラム起動時実行 #############################################################
rospy.init_node("main_infomation")
# root = tk.Tk()
# root.mainloop()
main_plogram = main_infomation()
exp_fil, motor_sel_l, blink_detc = main_plogram.key_reception()
# call_service()#キー入力完了サービス送信
main_plogram.main(exp_fil, motor_sel_l, blink_detc)
while not rospy.is_shutdown():
    rospy.spin()