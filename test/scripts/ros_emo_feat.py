#!/usr/bin/env python
import rospy
import random
import numpy as np
from std_msgs.msg import String,UInt8
from std_srvs.srv import Empty,EmptyResponse
import sys
   
#キー読み込み完了のサービス(サーバー) ##################################
def handle_service(req):
    # rospy.loginfo('[KEY][SERVER] ros_emo_feat: called')
    return EmptyResponse()

def service_server():
    s = rospy.Service('EnterKeys_ready', Empty, handle_service)
    # rospy.loginfo('[KEY][SERVER] ros_emo_feat: Ready to Serve')    
    
class main_infomation(object):
    def __init__(self):
        #########################################################################
        self.blink = String()
        self.emotion = String()
        #ROS_PUB_SUB #################################################################
        #motor
        self._mian_Larm_pub = rospy.Publisher("main_Larm_servo", UInt8, queue_size=1)
        self._mian_Rarm_pub = rospy.Publisher("main_Rarm_servo", UInt8, queue_size=1)
        self._mian_head_pub = rospy.Publisher("main_head_servo", UInt8, queue_size=1)
        self._mian_waist_pub = rospy.Publisher("main_waist_servo", UInt8, queue_size=1)
        #other 
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
                exp_fil = input(">> ")
                if (exp_fil == 'yes') or (exp_fil== 'no'):
                    break
                else:
                    rospy.loginfo('Please select whether or not there is an facial expression filter. [yes or no] ')
        except KeyboardInterrupt:
            sys.exit()
        rospy.loginfo("facial expression filter: %s", exp_fil)
    
        #使用するモータの選択
        rospy.loginfo('Enter all the motors to be used from [head,waist,arm], or enter [ALL] if all motors are used.  (ex)When using the head and waist => head,waist')
        try:
            while True:
                Inapplicable_characters = False
                motor_sel = input(">> ")
                motor_sel_l = motor_sel.split(',')
                for word in motor_sel_l:
                    if not word in ['head','waist','arm','ALL']:
                        Inapplicable_characters = True
                if Inapplicable_characters == False:
                    break
                else:
                    rospy.loginfo('Enter all the motors to be used from [head,waist,arm], or enter [ALL] if all motors are used.  (ex)When using the head and waist => head,waist')

        except KeyboardInterrupt:
            sys.exit()

        rospy.loginfo("motor used: %s", motor_sel_l)
        unused = list(set(['head','waist','arm'])^ set(motor_sel_l))
        rospy.loginfo("%s",unused)
        
        #瞬き検出の有無の選択
        rospy.loginfo('Use blink detection filter? [yes or no]')
        try:
            while True:
                blink_detc = input(">> ")
                if (blink_detc == 'yes') or (blink_detc== 'no'):
                    break
                else:
                    rospy.loginfo('Use blink detection filter? [yes or no]')
        except KeyboardInterrupt:
            sys.exit()

        rospy.loginfo("Use blink detection filter?: %s", blink_detc)
        return exp_fil,motor_sel_l,blink_detc
  
    def Larms_messageCb(self,msg,motor_sel_l):
        if (('ALL' in motor_sel_l) or (len(motor_sel_l) == 3) or ('arm' in motor_sel_l)):
            self._mian_Larm_pub.publish(msg)
        else:
            f_msg = UInt8(180)
            self._mian_Larm_pub.publish(f_msg)
    
    def Rarm_messageCb(self,msg,motor_sel_l):
        if (('ALL' in motor_sel_l) or (len(motor_sel_l) == 3) or ('arm' in motor_sel_l)):
            self._mian_Rarm_pub.publish(msg)
        else:
            f_msg = UInt8(0)
            self._mian_Rarm_pub.publish(f_msg)
    
    def head_messageCb(self,msg,motor_sel_l):
        if (('ALL' in motor_sel_l) or (len(motor_sel_l) == 3) or ('head' in motor_sel_l)):
            self._mian_head_pub.publish(msg)
        else:
            f_msg = UInt8(90)
            self._mian_head_pub.publish(f_msg)
    
    def waist_messageCb(self,msg,motor_sel_l):
        if (('ALL' in motor_sel_l) or (len(motor_sel_l) == 3) or ('waist' in motor_sel_l)):
            self._mian_waist_pub.publish(msg)
        else:
            f_msg = UInt8(90)
            self._mian_waist_pub.publish(f_msg)

    def blink_messageCb(self,msg,blink_detc):
        """
        瞬き
        """
        if blink_detc != 'yes':
            self.blink.data = msg.data #no_blink,blink
            self._main_blink_pub.publish(self.blink)
        else:
            self.blink.data =random.choice(['no_blink','no_blink','no_blink','no_blink','no_blink','no_blink','no_blink','no_blink','no_blink','blink'])  
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
        #motor
        self._mian_Larms_sub = rospy.Subscriber("Larm_servo", UInt8, self.Larms_messageCb, motor_sel_l)
        self._mian_Rarm_sub = rospy.Subscriber("Rarm_servo", UInt8, self.Rarm_messageCb, motor_sel_l)
        self._mian_head_sub = rospy.Subscriber("head_servo", UInt8, self.head_messageCb, motor_sel_l)
        self._mian_waist_sub = rospy.Subscriber("waist_servo", UInt8, self.waist_messageCb, motor_sel_l)
        #other 
        self._main_blink_sub = rospy.Subscriber("face_mediapipe", String, self.blink_messageCb, blink_detc)
        self._main_emotion_sub = rospy.Subscriber("emo_feat", String, self.emo_messageCb, exp_fil)
        
                    
#プログラム起動時実行 #############################################################
rospy.init_node("main_infomation")
main_plogram = main_infomation()
exp_fil, motor_sel_l, blink_detc = main_plogram.key_reception()
# call_service()#キー入力完了サービス送信
service_server()#キー入力完了レスポンス送信
main_plogram.main(exp_fil, motor_sel_l, blink_detc)
while not rospy.is_shutdown():
    rospy.spin()