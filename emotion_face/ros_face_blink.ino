/*眉毛修正版*/
/*ros関連*/
#include <ros.h>
#include <std_msgs/String.h>
#include <string>

/*M5stack関連*/
#include <M5Core2.h>
#define LGFX_AUTODETECT // 自動認識(D-duino-32 XS, PyBadgeはパネルID読取れないため自動認識の対象から外れている)
#define LGFX_USE_V1     // v1.0.0を有効に(v0からの移行期間の特別措置とのこと。書かない場合は旧v0系で動作)
#include <LovyanGFX.hpp>          // lovyanGFXのヘッダを準備
#include <LGFX_AUTODETECT.hpp>    // クラス"LGFX"を準備
static LGFX lcd;                  // LGFXのインスタンスを作成（クラスLGFXを使ってlcdコマンドでいろいろできるようにする）
static LGFX_Sprite canvas(&lcd);  // スプライトを使う場合はLGFX_Spriteのインスタンスを作成

String emo_str = String("neutral");    //neutral
String blink_str = String("no_blink");  //no_blink

int move_x = -40, move_y = 50;
int right_eye_x = 200+move_x, right_eye_y = 50+move_y; 
int left_eye_x = 60+move_x, left_eye_y = 50+move_y;
int nose_x = 123+move_x, nose_y = 130+move_y;
int mouth_x = 98+move_x, mouth_y = 150+move_y;

ros::NodeHandle  nh;

std_msgs::String reemo_msg;
std_msgs::String reface_msg;
std_msgs::String reinfo_msg;
ros::Publisher emo("emo", &reemo_msg);
ros::Publisher face("face", &reface_msg);
ros::Publisher info("info", &reinfo_msg);

void blink_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){
//右目
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 40, 33, BLACK);
  canvas.fillEllipse(31+right_eye_x, 25+right_eye_y, 53, 33, canvas.color332(250,128,114));
//左目
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 40, 33, BLACK);
  canvas.fillEllipse(29+left_eye_x, 25+left_eye_y, 53, 33, canvas.color332(250,128,114)); 
}

void anger_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){
//右目
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 10, WHITE);
  //上淵
  for(int i=-10;i<0;i++){
    canvas.drawBezier(7+right_eye_x, 50+right_eye_y, 45+right_eye_x, i+right_eye_y, 53+right_eye_x, 50+right_eye_y, BLACK);
    }
  for(int i=20;i<50;i++){
  canvas.drawBezier(7+right_eye_x, i+right_eye_y, 45+right_eye_x, -10+right_eye_y, 53+right_eye_x, 50+right_eye_y, canvas.color332(250,128,114));
  }
  //下淵
  canvas.fillEllipse(42+right_eye_x, 112+right_eye_y, 31, 34, BLACK);
  canvas.fillEllipse(50+right_eye_x, 114+right_eye_y, 41, 35, canvas.color332(250,128,114));
//左目
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE); 
   //上淵
  for(int i=-10;i<0;i++){
    canvas.drawBezier(53+left_eye_x, 50+left_eye_y, 15+left_eye_x, i+left_eye_y, 7+left_eye_x, 50+left_eye_y, BLACK);
    }
  for(int i=20;i<50;i++){
    canvas.drawBezier(53+left_eye_x, i+left_eye_y, 15+left_eye_x, -10+left_eye_y, 7+left_eye_x, 50+left_eye_y, canvas.color332(250,128,114));
    }
  //下淵
  canvas.fillEllipse(18+left_eye_x, 112+left_eye_y, 31, 34, BLACK);
  canvas.fillEllipse(10+left_eye_x, 114+left_eye_y, 41, 35, canvas.color332(250,128,114));
}

void disgust_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){
//右目 
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 10, WHITE);
  //下淵
  canvas.fillEllipse(30+right_eye_x, 91+right_eye_y, 25, 23, BLACK);
  canvas.fillEllipse(31+right_eye_x, 106+right_eye_y, 39, 33, canvas.color332(250,128,114));   
//左目
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE);
  //下淵
  canvas.fillEllipse(30+left_eye_x, 91+left_eye_y, 25, 23, BLACK);
  canvas.fillEllipse(31+left_eye_x, 106+left_eye_y, 39, 33, canvas.color332(250,128,114)); 
  }

void fear_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){ 
//右目
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 15, 15, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 5, WHITE);
  //下淵
  for(int i=76;i<80;i++){
    canvas.drawBezier(6+right_eye_x, 80+right_eye_y, 21+right_eye_x, i+right_eye_y, 40+right_eye_x, 90+right_eye_y,BLACK);
    }
//左目
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 15, 15, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 5, WHITE);
  //下淵
  for(int i=76;i<80;i++){
    canvas.drawBezier(54+left_eye_x, 80+left_eye_y, 35+left_eye_x, i+left_eye_y, 16+left_eye_x, 90+left_eye_y,BLACK);
    } 
  }

void happiness_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){
//右目
  canvas.fillEllipse(30+right_eye_x, 70+right_eye_y, 30, 35, BLACK);
  canvas.fillEllipse(30+right_eye_x, 74+right_eye_y, 32, 36, canvas.color332(250,128,114)); 
//左目
  canvas.fillEllipse(30+left_eye_x, 70+left_eye_y, 30, 35, BLACK);
  canvas.fillEllipse(30+left_eye_x, 74+left_eye_y, 32, 36, canvas.color332(250,128,114));
  }
  
void sadness_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){ 
//右目
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 10, WHITE);
  //上淵
  for(int i=-15;i<-5;i++){
    canvas.drawBezier(7+right_eye_x, 50+right_eye_y, 15+right_eye_x, i+right_eye_y, 62+right_eye_x, 70+right_eye_y,BLACK);
    }
  for(int i=-15;i<70;i++){
    canvas.drawBezier(7+right_eye_x, 50+right_eye_y, 15+right_eye_x, -15+right_eye_y, 62+right_eye_x, i+right_eye_y,canvas.color332(250,128,114));
    }
  //下淵
  for(int i=76;i<80;i++){
    canvas.drawBezier(6+right_eye_x, 80+right_eye_y, 21+right_eye_x, i+right_eye_y, 40+right_eye_x, 90+right_eye_y,BLACK);
    }
//左目
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE); 
  //上淵
  for(int i=-15;i<-5;i++){
    canvas.drawBezier(53+left_eye_x, 50+left_eye_y, 45+left_eye_x, i+left_eye_y, -3+left_eye_x, 70+left_eye_y,BLACK);
    }
  for(int i=-15;i<70;i++){
    canvas.drawBezier(53+left_eye_x, 50+left_eye_y, 45+left_eye_x, -15+left_eye_y, -3+left_eye_x, i+left_eye_y,canvas.color332(250,128,114));
    }
  //下淵
  for(int i=76;i<80;i++){
    canvas.drawBezier(54+left_eye_x, 80+left_eye_y, 35+left_eye_x, i+left_eye_y, 16+left_eye_x, 90+left_eye_y,BLACK);
    }
  }

void surprise_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){
//右目  
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23-5, 28+5, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18-5, 21-7, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5-2, 10-7, WHITE);
//左目  
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23-5, 28+5, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18-5, 21-7, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5-2, 10-7, WHITE);
  }
   
void neutral_eye(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y){
//右目
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 10, WHITE);
//左目
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE);
  }


void anger_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){ 
//右目
  //眉毛
  for(int i=-20;i<-10;i++){
    canvas.drawBezier(-10+right_eye_x, 50+right_eye_y, 48+right_eye_x, i+right_eye_y, 60+right_eye_x, 18+right_eye_y,BLACK);
    }
//左目
  //眉毛
  for(int i=-20;i<-10;i++){
    canvas.drawBezier(70+left_eye_x, 50+left_eye_y, 12+left_eye_x, i+left_eye_y, 0+left_eye_x, 18+left_eye_y,BLACK);
    }
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114)); 
  //口
  //唇間の影
  canvas.fillEllipse(60+mouth_x, 44+mouth_y, 50, 32, BLACK);
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 52, 30, canvas.color332(250,128,114));
  //下唇影
  canvas.fillEllipse(60+mouth_x, 61+mouth_y, 21, 32, BLACK);
  canvas.fillEllipse(60+mouth_x, 68+mouth_y, 44, 32, canvas.color332(250,128,114));
  }

void disgust_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
//右目
  //眉毛
  canvas.fillEllipse(30+right_eye_x, -20+right_eye_y, 40, 33, BLACK);
  canvas.fillEllipse(31+right_eye_x, -25+right_eye_y, 53, 33, canvas.color332(250,128,114));   
//左目
  //眉毛
  canvas.fillEllipse(30+left_eye_x, -20+left_eye_y, 40, 33, BLACK);
  canvas.fillEllipse(29+left_eye_x, -25+left_eye_y, 53, 33, canvas.color332(250,128,114));
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114)); 
  //口
  //唇間の影
  canvas.fillEllipse(60+mouth_x, 44+mouth_y, 50, 32, BLACK);
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 52, 30, canvas.color332(250,128,114));
  //下唇影
  canvas.fillEllipse(60+mouth_x, 61+mouth_y, 21, 32, BLACK);
  canvas.fillEllipse(60+mouth_x, 68+mouth_y, 44, 32, canvas.color332(250,128,114));
  }

void fear_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
//右目
  //眉毛
  canvas.fillEllipse(45+right_eye_x, -20+right_eye_y, 40, 35, BLACK);
  canvas.fillEllipse(55+right_eye_x, -22+right_eye_y, 55, 35, canvas.color332(250,128,114)); 
//左目
  //眉毛
  canvas.fillEllipse(15+left_eye_x, -20+left_eye_y, 40, 35, BLACK);
  canvas.fillEllipse(5+left_eye_x, -22+left_eye_y, 55, 35, canvas.color332(250,128,114));
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114));
  //口
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 33, canvas.color332(205,92,92));
  canvas.fillEllipse(60+mouth_x, 43+mouth_y, 35, 29, WHITE);
  canvas.fillEllipse(60+mouth_x, 45+mouth_y, 35, 22, canvas.color332(152,81,75)); 
  }

void happiness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
//右眉毛
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillEllipse(30+right_eye_x, 36+right_eye_y, 35, 23, canvas.color332(250,128,114));
//左眉毛
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillEllipse(30+left_eye_x, 36+left_eye_y, 35, 23, canvas.color332(250,128,114));  
//口、鼻
  //口
  //下唇影
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 21, 10, BLACK);
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 21, 5, canvas.color332(250,128,114));
  canvas.fillRect(60-21+mouth_x,48-10+mouth_y,42,10, canvas.color332(250,128,114));
  //唇間の影
  canvas.fillEllipse(60+mouth_x, 18+mouth_y, 52, 30, BLACK);
  canvas.fillEllipse(60+mouth_x, 18+mouth_y, 52, 25, canvas.color332(250,128,114));
  canvas.fillRect(60-52+mouth_x,18-30+mouth_y,104,30,canvas.color332(250,128,114));  
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114)); 
  }
  
void sadness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
//右目
  //眉毛
  for(int i=-20;i<-10;i++){
    canvas.drawBezier(0+right_eye_x, 31+right_eye_y, 10+right_eye_x, i+right_eye_y, 68+right_eye_x, 48+right_eye_y,BLACK);
    }
//左目
  //眉毛
  for(int i=-20;i<-10;i++){
    canvas.drawBezier(60+left_eye_x, 31+left_eye_y, 50+left_eye_x, i+left_eye_y, -8+left_eye_x, 48+left_eye_y,BLACK);
    }
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114)); 
  //口
  //唇間の影
  canvas.fillEllipse(60+mouth_x, 50+mouth_y, 55, 32, BLACK);
  canvas.fillEllipse(60+mouth_x, 55+mouth_y, 75, 32, canvas.color332(250,128,114)); 
  //下唇影
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 21, 10, BLACK);
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 21, 5, canvas.color332(250,128,114));
  canvas.fillRect(60-21+mouth_x,48-10+mouth_y,42,10, canvas.color332(250,128,114));
  }
  
void surprise_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
//右眉毛
  canvas.fillEllipse(30+right_eye_x, 30-10+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30-10+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillEllipse(30+right_eye_x, 36-10+right_eye_y, 35, 23, canvas.color332(250,128,114)); 
//左眉毛
  canvas.fillEllipse(30+left_eye_x, 30-10+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30-10+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillEllipse(30+left_eye_x, 36-10+left_eye_y, 35, 23, canvas.color332(250,128,114)); 
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114));
  //口
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 45, canvas.color332(205,92,92));
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 37, WHITE);
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 28, canvas.color332(152,81,75));  
  }
  
void neutral_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){ 
//右眉毛
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillEllipse(30+right_eye_x, 36+right_eye_y, 35, 23, canvas.color332(250,128,114)); 
//左眉毛
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 21, canvas.color332(250,128,114)); 
  canvas.fillEllipse(30+left_eye_x, 36+left_eye_y, 35, 23, canvas.color332(250,128,114)); 
//口、鼻
  //口
  //下唇影
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 21, 10, BLACK);
  canvas.fillEllipse(60+mouth_x, 48+mouth_y, 21, 5, canvas.color332(250,128,114));
  canvas.fillRect(60-21+mouth_x,48-10+mouth_y,42,10, canvas.color332(250,128,114));
  //唇間の影
  canvas.fillEllipse(60+mouth_x, 18+mouth_y, 52, 30, BLACK);
  canvas.fillEllipse(60+mouth_x, 18+mouth_y, 52, 25, canvas.color332(250,128,114));
  canvas.fillRect(60-52+mouth_x,18-30+mouth_y,104,30,canvas.color332(250,128,114));  
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114)); 
  }


void blink_anger_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){ 
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  anger_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  canvas.pushSprite(0, 0); 
  }

void noblink_anger_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){ 
  canvas.fillScreen(canvas.color332(250,128,114)); 
  anger_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  anger_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  canvas.pushSprite(0, 0); 
  }

void blink_disgust_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  disgust_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  canvas.pushSprite(0, 0); 
  }

void noblink_disgust_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  disgust_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  disgust_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  canvas.pushSprite(0, 0); 
  }

void blink_fear_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  fear_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  canvas.pushSprite(0, 0);  
  }

void noblink_fear_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  fear_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  fear_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  canvas.pushSprite(0, 0); 
  }

void blink_happiness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  happiness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  canvas.pushSprite(0, 0); 
  }

void noblink_happiness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  happiness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  happiness_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  canvas.pushSprite(0, 0); 
  }
  
void blink_sadness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  sadness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  canvas.pushSprite(0, 0);  
  }

void noblink_sadness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  sadness_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  sadness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  canvas.pushSprite(0, 0); 
  }
  
void blink_surprise_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  surprise_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  canvas.pushSprite(0, 0); 
  }

void noblink_surprise_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  surprise_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  surprise_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  canvas.pushSprite(0, 0); 
  }
  
void blink_neutral_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  blink_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  canvas.pushSprite(0, 0); 
  }

void noblink_neutral_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
  neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y); 
  neutral_eye(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
  canvas.pushSprite(0, 0); 
  }

/*ros関連*/
void display_expressions(String face_str){

  if ((face_str.equalsIgnoreCase("blink")) || (face_str.equalsIgnoreCase("no_blink"))){
    blink_str= face_str;
  }else{
    emo_str = face_str;
  }

  /*デバック用*/
  reemo_msg.data =  emo_str.c_str();
  reface_msg.data = blink_str.c_str();
  reinfo_msg.data = face_str.c_str();
  emo.publish(&reemo_msg);
  face.publish(&reface_msg);
  info.publish(&reinfo_msg);

  if (blink_str.equalsIgnoreCase("no_blink")){
    if(emo_str.equalsIgnoreCase("neutral")){
      noblink_neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("happiness")){
      noblink_happiness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("surprise")){
      noblink_surprise_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("sadness")){
      noblink_sadness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("anger")){
      noblink_anger_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("disgust")){
      noblink_disgust_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("fear")){
      noblink_fear_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }
  }else if(blink_str.equalsIgnoreCase("blink")){
    if(emo_str.equalsIgnoreCase("neutral")){
      blink_neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("happiness")){
      blink_happiness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("surprise")){
      blink_surprise_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("sadness")){
      blink_sadness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("anger")){
      blink_anger_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("disgust")){
      blink_disgust_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }else if(emo_str.equalsIgnoreCase("fear")){
      blink_fear_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
    }
  }
}

void emo_messageCb( const std_msgs::String& emo_msg){
  display_expressions(emo_msg.data);
}

void blink_messageCb( const std_msgs::String& blink_msg){
  display_expressions(blink_msg.data);
}

ros::Subscriber<std_msgs::String> sub_emo("main_emotion", emo_messageCb );
ros::Subscriber<std_msgs::String> sub_face("main_blink", blink_messageCb );

// 初期設定 -----------------------------------------
void setup(){
  /*ros関連*/
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub_emo);
  nh.subscribe(sub_face);
  
  /*デバック用*/
  nh.advertise(emo);
  nh.advertise(face);
  nh.advertise(info);

  /*Mstack関連*/
  // 本体初期化
  M5.begin();   
  // LCD初期設定
  lcd.init();                 // LCD初期化
  lcd.setRotation(2);         // 画面向き設定（0～3で設定、4～7は反転)
  canvas.setColorDepth(8);    // カラーモード設定，CORE2 GRAY、のスプライトは16bit以上で表示されないため8bitに設定
  canvas.setTextWrap(false);  // 改行をしない（画面をはみ出す時自動改行する場合はtrue）
  canvas.createSprite(lcd.width(), lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）
  noblink_neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);  
}

void loop(){
  /*ros関連*/
  nh.spinOnce();
  delay(30);
}