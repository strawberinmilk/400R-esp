#include "handBrake.h"
#include "config/pinConfig.h"
#include "output/PwmLeds/footLight.h"
#include "output/PwmLeds/heartLight.h"

extern FootLight footLight;
extern HeartLight heartLight;

// 初期化
void HandBrake::init()
{
  pinMode(HAND_BRAKE_INPUT_PIN, INPUT);
  handBrakeState = digitalRead(HAND_BRAKE_INPUT_PIN);
}

// ループ処理
void HandBrake::update()
{
  bool currentHandBrakeState = digitalRead(HAND_BRAKE_INPUT_PIN);
  if (currentHandBrakeState != handBrakeState)
  {
    handBrakeState = currentHandBrakeState;
    footLight.lighting();
    heartLight.lighting();
  }
}

// ハンドブレーキの状態を取得
bool HandBrake::getHandBrakeState() const
{
  return handBrakeState;
}
