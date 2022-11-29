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

void happiness(){
  
  }
  
void sadness(){
  
  }
  
void surprise(){
  
  canvas.createSprite(80,60);
  canvas.fillEllipse(30, 30, 30, 20, 0xFFFF);
  canvas.fillCircle(30, 30, 15, 0x0000);
  canvas.pushSprite(80, 60);

  canvas.createSprite(80,60);
  canvas.fillEllipse(30, 30, 30, 20, 0xFFFF);
  canvas.fillCircle(30, 30, 15, 0x0000);
  canvas.pushSprite(80+100, 60);    
  }
  
void neutral(){
  canvas.createSprite(80,60);
  canvas.fillEllipse(30, 30, 30, 20, 0xFFFF);
  canvas.fillCircle(30, 30, 15, 0x0000);
  canvas.pushSprite(80, 60);

  canvas.createSprite(80,60);
  canvas.fillEllipse(30, 30, 30, 20, 0xFFFF);
  canvas.fillCircle(30, 30, 15, 0x0000);
  canvas.pushSprite(80+100, 60); 
  
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
  
//  surprise();
//  delay(100);  
  
}

void loop(){
  M5.update();                      // 本体ボタン状態更新
  // LCD表示処理（canvas.で指定してメモリ内の仮想画面に描画していく）
  canvas.fillScreen(BLACK);         // 背景塗り潰し

  swith(emo){
    
    case angry:
    case disgust:
    case fear:
      netral();
      break;

     case happiness:
      happiness();
      break;
      
     case sadness:
      sadness();
      break;
      
     case surprise:
      surprise();
      break;
      
     case neutral:
      neutral();
      break;
    
    }
  
  delay(100); 
  
  
  }
