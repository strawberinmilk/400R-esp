#include "setMode.h"
#include "output/footLight.h"
#include "interface/display.h"
#include "interface/encoder.h"
#include "config/pinConfig.h"

extern Display display;
extern FootLight footLight;
extern Encoder encoder;

// フットライト明るさ調整
void SetMode::footLightVolume()
{
  display.print("Foot Light Volume", "");
  delay(1000);

  encoder.setCount(footLight.getVolume());
  encoder.encoderEnabled = true;
  while (true)
  {
    if (encoder.updateEncoder(0, 255))
    {
      display.print("Foot Light Volume", String(encoder.currentEncoderValue).c_str());
      footLight.setVolume(encoder.currentEncoderValue);
    }
    if (digitalRead(SELECT_SW_PIN) == LOW)
    {
      encoder.encoderEnabled = false;
      break;
    }
  }
}

// フットライトモード選択
void SetMode::footLightMode()
{
  display.print("Foot Light Mode", "");
  delay(1000);

  encoder.setCount(footLight.isLighting() ? 1 : 0);
  encoder.encoderEnabled = true;
  while (true)
  {
    if (encoder.updateEncoder(0, 1))
    {
      display.print("Foot Light Mode", encoder.currentEncoderValue == 1 ? "ON" : "OFF");
      footLight.setIsLighting(encoder.currentEncoderValue == 1);
    }
    if (digitalRead(SELECT_SW_PIN) == LOW)
    {
      encoder.encoderEnabled = false;
      break;
    }
  }
}

// 各設定モードを配列に格納
void (SetMode::*modes[])() = {
    &SetMode::footLightVolume,
    &SetMode::footLightMode,
};
// モード表示名の配列
const char *modeNames[] = {
    "Foot Light Vol",
    "Foot Light Mode",
};
// モードの数を計算し定数化
size_t modeLength = sizeof(modes) / sizeof(modes[0]);

// 初期化関数
void SetMode::init()
{
}

// メイン関数
void SetMode::select()
{
  display.print("Mode Selected", "please select");
  delay(1000);
  encoder.setCount(0);
  encoder.encoderEnabled = true;
  while (true)
  {
    if (encoder.updateEncoder(0, modeLength - 1))
    {
      display.print("Mode Selected", modeNames[encoder.currentEncoderValue]);
    }
    if (digitalRead(SELECT_SW_PIN) == LOW)
    {
      encoder.encoderEnabled = false;
      (this->*modes[encoder.currentEncoderValue])();
      display.print("success!", "");
      delay(1000);
      break;
    }
  } // while
}
