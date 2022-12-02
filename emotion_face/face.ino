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

void happiness_face(){
  canvas.fillScreen(canvas.color332(250,128,114));   
//右目
  //pushSprite
  int right_eye_x = 200;
  int right_eye_y = 50;
  //眉毛
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+right_eye_x, 30+right_eye_y,60,28, canvas.color332(250,128,114));  
  //目
  canvas.fillEllipse(30+right_eye_x, 70+right_eye_y, 30, 35, BLACK);
  canvas.fillEllipse(30+right_eye_x, 74+right_eye_y, 32, 36, canvas.color332(250,128,114));
  
//左目
  //pushSprite
  int left_eye_x = 60;
  int left_eye_y = 50;
  //眉毛
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+left_eye_x, 30+left_eye_y,60,28, canvas.color332(250,128,114));  
  //目
  canvas.fillEllipse(30+left_eye_x, 70+left_eye_y, 30, 35, BLACK);
  canvas.fillEllipse(30+left_eye_x, 74+left_eye_y, 32, 36, canvas.color332(250,128,114));

//口、鼻
  //pushSprite
  int nose_x = 130;
  int nose_y = 130;
  int mouth_x = 105;
  int mouth_y = 150;
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
  
void sadness_face(){
  
  }
  
void surprise_face(){
  canvas.fillScreen(canvas.color332(250,128,114));   
//右目
  //pushSprite
  int right_eye_x = 200;
  int right_eye_y = 50;
  //眉毛
  canvas.fillEllipse(30+right_eye_x, 30-10+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30-10+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+right_eye_x, 30-10+right_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 28-5, 35+5, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 58+right_eye_y, 18-5, 22-7, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5-2, 10-7, WHITE);
  
//左目
  //pushSprite
  int left_eye_x = 60;
  int left_eye_y = 50;
  //眉毛
  canvas.fillEllipse(30+left_eye_x, 30-10+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30-10+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+left_eye_x, 30-10+left_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 28-5, 35+5, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 58+left_eye_y, 18-5, 22-7, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5-2, 10-7, WHITE);

//口、鼻
  //pushSprite
  int nose_x = 130;
  int nose_y = 130;
  int mouth_x = 105;
  int mouth_y = 150; 
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
  
void neutral_face(){
  canvas.fillScreen(canvas.color332(250,128,114));   
//右目
  //pushSprite
  int right_eye_x = 200;
  int right_eye_y = 50;
  //眉毛
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+right_eye_x, 30+right_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+right_eye_x, 30+right_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+right_eye_x, 50+right_eye_y, 28, 35, WHITE);
  //黒目
  canvas.fillEllipse(30+right_eye_x, 58+right_eye_y, 18, 22, BLACK);
  //光
  canvas.fillEllipse(23+right_eye_x, 51+right_eye_y, 5, 10, WHITE);
  
//左目
  //pushSprite
  int left_eye_x = 60;
  int left_eye_y = 50;
  //眉毛
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 28, BLACK);
  canvas.fillEllipse(30+left_eye_x, 30+left_eye_y, 30, 21, canvas.color332(250,128,114));
  canvas.fillRect(30-30+left_eye_x, 30+left_eye_y,60,28, canvas.color332(250,128,114));  
  //白目
  canvas.fillEllipse(30+left_eye_x, 50+left_eye_y, 28, 35, WHITE);
  //黒目
  canvas.fillEllipse(30+left_eye_x, 58+left_eye_y, 18, 22, BLACK);
  //光
  canvas.fillEllipse(23+left_eye_x, 51+left_eye_y, 5, 10, WHITE);

//口、鼻
  //pushSprite
  int nose_x = 130;
  int nose_y = 130;
  int mouth_x = 105;
  int mouth_y = 150;
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
  lcd.setRotation(1);         // 画面向き設定（0～3で設定、4～7は反転)
  canvas.setColorDepth(8);    // カラーモード設定（書かなければ初期値16bit。24bit（パネル性能によっては18bit）は対応していれば選択可）
                              // CORE2 GRAY、のスプライトは16bit以上で表示されないため8bitに設定
  canvas.setTextWrap(false);  // 改行をしない（画面をはみ出す時自動改行する場合はtrue）
  canvas.setTextSize(1);      // 文字サイズ（倍率）
  canvas.createSprite(lcd.width(), lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）

  enum emotion emo;
  emo = neutral;//取得された感情
  
}

void loop(){
  M5.update();                      // 本体ボタン状態更新
  // LCD表示処理（canvas.で指定してメモリ内の仮想画面に描画していく）
  canvas.setBaseColor(canvas.color332(250,128,114));
  canvas.fillScreen(canvas.color332(250,128,114));         // 背景塗り潰し
  enum emotion emo;
  emo = neutral;//取得された感情

 switch(emo){
   
   case angry:
   case disgust:
   case fear:
     neutral_face();
     break;

    case happiness:
     happiness_face();
     break;
     
    case sadness:
     sadness_face();
     break;
     
    case surprise:
     surprise_face();
     break;
     
    case neutral:
     neutral_face();
     break;
   
   }
 
 delay(100); 
  
  
  }
