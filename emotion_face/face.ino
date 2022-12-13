//emolist = ["anger", "disgust", "fear", "happiness", "sadness", "surprise", "neutral"]

#include <M5Core2.h>

#define LGFX_AUTODETECT // 自動認識(D-duino-32 XS, PyBadgeはパネルID読取れないため自動認識の対象から外れているそうです)
#define LGFX_USE_V1     // v1.0.0を有効に(v0からの移行期間の特別措置とのこと。書かない場合は旧v0系で動作)

#include <LovyanGFX.hpp>          // lovyanGFXのヘッダを準備
#include <LGFX_AUTODETECT.hpp>    // クラス"LGFX"を準備

static LGFX lcd;                  // LGFXのインスタンスを作成（クラスLGFXを使ってlcdコマンドでいろいろできるようにする）
static LGFX_Sprite canvas(&lcd);  // スプライトを使う場合はLGFX_Spriteのインスタンスを作成

enum emotion {
  anger, disgust, fear, happiness, sadness, surprise, neutral
};

void anger_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
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
  //眉毛
  for(int i=-20;i<-10;i++){
    canvas.drawBezier(-10+right_eye_x, 50+right_eye_y, 48+right_eye_x, i+right_eye_y, 60+right_eye_x, 18+right_eye_y,BLACK);
    }
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
//表示
  canvas.pushSprite(0, 0); 
  }

void disgust_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
//右目
  //眉毛
  canvas.fillEllipse(30+right_eye_x, -20+right_eye_y, 40, 33, BLACK);
  canvas.fillEllipse(31+right_eye_x, -25+right_eye_y, 53, 33, canvas.color332(250,128,114));  
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
  //眉毛
  canvas.fillEllipse(30+left_eye_x, -20+left_eye_y, 40, 33, BLACK);
  canvas.fillEllipse(29+left_eye_x, -25+left_eye_y, 53, 33, canvas.color332(250,128,114));
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE);
  //下淵
  canvas.fillEllipse(30+left_eye_x, 91+left_eye_y, 25, 23, BLACK);
  canvas.fillEllipse(31+left_eye_x, 106+left_eye_y, 39, 33, canvas.color332(250,128,114)); 
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
//表示
  canvas.pushSprite(0, 0); 
  }

void fear_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
//右目
  //眉毛
  canvas.fillEllipse(45+right_eye_x, -20+right_eye_y, 40, 35, BLACK);
  canvas.fillEllipse(55+right_eye_x, -22+right_eye_y, 55, 35, canvas.color332(250,128,114)); 
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
  //眉毛
  canvas.fillEllipse(15+left_eye_x, -20+left_eye_y, 40, 35, BLACK);
  canvas.fillEllipse(5+left_eye_x, -22+left_eye_y, 55, 35, canvas.color332(250,128,114));
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
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114));
  //口
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 33, canvas.color332(205,92,92));
  canvas.fillEllipse(60+mouth_x, 43+mouth_y, 35, 29, WHITE);
  canvas.fillEllipse(60+mouth_x, 45+mouth_y, 35, 22, canvas.color332(152,81,75)); 
//表示
  canvas.pushSprite(0, 0); 
  }

void happiness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
//右目
  //眉毛
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+right_eye_x, 30+right_eye_y,60,28, canvas.color332(250,128,114));  
  //目
  canvas.fillEllipse(30+right_eye_x, 70+right_eye_y, 30, 35, BLACK);
  canvas.fillEllipse(30+right_eye_x, 74+right_eye_y, 32, 36, canvas.color332(250,128,114)); 
//左目
  //眉毛
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+left_eye_x, 30+left_eye_y,60,28, canvas.color332(250,128,114));  
  //目
  canvas.fillEllipse(30+left_eye_x, 70+left_eye_y, 30, 35, BLACK);
  canvas.fillEllipse(30+left_eye_x, 74+left_eye_y, 32, 36, canvas.color332(250,128,114));
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
 //表示
  canvas.pushSprite(0, 0); 
  }
  
void sadness_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114));
  int move_x = -40;
  int move_y = 50;     
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
  //眉毛
  for(int i=-20;i<-10;i++){
    canvas.drawBezier(0+right_eye_x, 31+right_eye_y, 10+right_eye_x, i+right_eye_y, 68+right_eye_x, 48+right_eye_y,BLACK);
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
//表示
  canvas.pushSprite(0, 0); 
  }
  
void surprise_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114));
//右目
  //眉毛
  canvas.fillEllipse(30+right_eye_x, 30-10+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30-10+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+right_eye_x, 30-10+right_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23-5, 28+5, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18-5, 21-7, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5-2, 10-7, WHITE);
//左目
  //眉毛
  canvas.fillEllipse(30+left_eye_x, 30-10+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30-10+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+left_eye_x, 30-10+left_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23-5, 28+5, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18-5, 21-7, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5-2, 10-7, WHITE);
//口、鼻
  //鼻
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 22, BLACK);
  canvas.fillEllipse(35+nose_x, 25+nose_y, 20, 18, canvas.color332(250,128,114));
  canvas.fillRect(35-20+nose_x, 25+nose_y,40,22, canvas.color332(250,128,114));
  //口
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 45, canvas.color332(205,92,92));
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 37, WHITE);
  canvas.fillEllipse(60+mouth_x, 41+mouth_y, 35, 28, canvas.color332(152,81,75));  
//表示
  canvas.pushSprite(0, 0); 
  }
  
void neutral_face(int right_eye_x, int right_eye_y, int left_eye_x, int left_eye_y, int nose_x, int nose_y, int mouth_x, int mouth_y){
  canvas.fillScreen(canvas.color332(250,128,114)); 
//右目
  //眉毛
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+right_eye_x, 30+right_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 55+right_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 10, WHITE);
//左目
  //眉毛
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+left_eye_x, 30+left_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 23, 28, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 55+left_eye_y, 18, 21, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE);
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
//表示
  canvas.pushSprite(0, 0); 
  }



// 初期設定 -----------------------------------------
void setup() {
  M5.begin();   // 本体初期化

  // LCD初期設定
  lcd.init();                 // LCD初期化
  lcd.setRotation(2);         // 画面向き設定（0～3で設定、4～7は反転)
  canvas.setColorDepth(8);    // カラーモード設定（書かなければ初期値16bit。24bit（パネル性能によっては18bit）は対応していれば選択可）
                              // CORE2 GRAY、のスプライトは16bit以上で表示されないため8bitに設定
  canvas.setTextWrap(false);  // 改行をしない（画面をはみ出す時自動改行する場合はtrue）
  canvas.setTextSize(1);      // 文字サイズ（倍率）
  canvas.createSprite(lcd.width(), lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）  
}

void loop(){
  M5.update();                      // 本体ボタン状態更新
  // LCD表示処理（canvas.で指定してメモリ内の仮想画面に描画していく）
  canvas.setBaseColor(canvas.color332(250,128,114));
  canvas.fillScreen(canvas.color332(250,128,114));         // 背景塗り潰し
  
  enum emotion emo;
  emo = neutral;//取得された感情

  int move_x = -40, move_y = 50;
  int right_eye_x = 200+move_x, right_eye_y = 50+move_y; 
  int left_eye_x = 60+move_x, left_eye_y = 50+move_y;
  int nose_x = 123+move_x, nose_y = 130+move_y;
  int mouth_x = 98+move_x, mouth_y = 150+move_y;

  
//  right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y;
  
  neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
  delay(10000);
//  surprise_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
//  
//  neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
//  happiness_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
//  
//  neutral_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
//  anger_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
//  fear_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
//  disgust_face(right_eye_x, right_eye_y, left_eye_x, left_eye_y, nose_x, nose_y, mouth_x, mouth_y);
//  delay(10000);
  

//  switch(emo){
//    
//    case angry:
//    case disgust:
//    case fear:
//      neutral_face();
//      break;
//
//     case happiness:
//      happiness_face();
//      break;
//      
//     case sadness:
//      sadness_face();
//      break;
//      
//     case surprise:
//      surprise_face();
//      break;
//      
//     case neutral:
//      neutral_face();
//      break;
//    
//    }
//  
//  delay(100); 
  
  
  }
