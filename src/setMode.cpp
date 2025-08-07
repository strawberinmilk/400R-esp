#include "setMode.h"
#include "output/footLight.h"
#include "interface/display.h"
#include "interface/button.h"
#include "interface/encoder.h"
#include "config/pinConfig.h"

extern Display display;
extern FootLight footLight;
extern Encoder encoder;
extern Button button;

// フットライト明るさ調整
void SetMode::footLightVolume()
{
  display.print("Foot Light Volume", "");

  encoder.runEncoder(
      footLight.getVolume(),
      0,
      255,
      [](int value)
      {
        display.print("Foot Light Volume", String(value).c_str());
        footLight.setVolume(value);
      },
      []()
      {
        return !!button.isPushAwait(SELECT_SW_PIN);
      });
}

// フットライトモード選択
void SetMode::footLightMode()
{
  display.print("Foot Light Mode", "");

  encoder.runEncoder(
      footLight.isLighting() ? 1 : 0,
      0,
      1,
      [](int value)
      {
        display.print("Foot Light Mode", encoder.currentEncoderValue == 1 ? "ON" : "OFF");
        footLight.setIsLighting(encoder.currentEncoderValue == 1);
      },
      []()
      {
        return !!button.isPushAwait(SELECT_SW_PIN);
      });
}

// 各設定モードを配列に格納
void (SetMode::*modes[])() = {
    &SetMode::footLightVolume,
    &SetMode::footLightMode,
};
// モード表示名の配列
const char *modeNames[] = {
    "Foot Light Vol",  // 14文字
    "Foot Light Mode", // 15文字
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
  // delay(1000);
  // encoder.setCount(0);
  // encoder.encoderEnabled = true;
  // while (true)
  // {
  //   if (encoder.updateEncoder(0, modeLength - 1))
  //   {
  //     display.print("Mode Selected", modeNames[encoder.currentEncoderValue]);
  //   }
  //   if (button.isPushAwait(SELECT_SW_PIN))
  //   {
  //     encoder.encoderEnabled = false;
  //     (this->*modes[encoder.currentEncoderValue])();
  //     display.print("success!", "");
  //     delay(1000);
  //     break;
  //   }
  // }

  int selectNum = encoder.runEncoder(
      0,
      0,
      modeLength - 1,
      [](int value)
      {
        display.print("Mode Selected", modeNames[encoder.currentEncoderValue]);
      },
      []()
      {
        return !!button.isPushAwait(SELECT_SW_PIN);
      });

  (this->*modes[selectNum])();
  display.print("success!", "");
  delay(1000);
}
