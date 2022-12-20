//ros系準備
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>

ros::NodeHandle nh;

std_msgs::Empty m_ready;
ros::Publisher serv("motor_ready", &m_ready);

//servo系準備
#include <Servo.h>

Servo s1;
Servo s2;
Servo s3;
Servo s4;

//準備完了パブリッシャ
void pub_def(void) { 
   serv.publish( &m_ready );
}

//サーボ稼働
void head_cb( const std_msgs::UInt8& cmd_msg) {
    s1.write(cmd_msg.data);
}

void larm_cb( const std_msgs::UInt8& cmd_msg) {
    s2.write(cmd_msg.data);
}

void rarm_cb( const std_msgs::UInt8& cmd_msg) {
    s3.write(cmd_msg.data);
}

void waist_cb( const std_msgs::UInt8& cmd_msg) {
    s4.write(cmd_msg.data);
    pub_def();
}

//ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );
//ros::Subscriber<std_msgs::UInt16MultiArray> motor_sub("servo", servo_cb);
ros::Subscriber<std_msgs::UInt8> motor_head_sub("main_head_servo", head_cb);
ros::Subscriber<std_msgs::UInt8> motor_Larm_sub("main_Larm_servo", larm_cb);
ros::Subscriber<std_msgs::UInt8> motor_Rarm_sub("main_Rarm_servo", rarm_cb);
ros::Subscriber<std_msgs::UInt8> motor_waist_sub("main_waist_servo", waist_cb);


void setup()
{
  s1.attach(5);
  s1.write(90);   //head
  s2.attach(6);
  s2.write(180);  //left arm
  s3.attach(9);
  s3.write(0);    //right arm
  s4.attach(10);
  s4.write(90);   //waist

  nh.getHardware()->setBaud(9600);
  nh.initNode();
  nh.advertise(serv);
  nh.subscribe(motor_head_sub);
  nh.subscribe(motor_Larm_sub);
  nh.subscribe(motor_Rarm_sub);
  nh.subscribe(motor_waist_sub);
}

void loop()
{
  nh.spinOnce();
  delay(100);
}
