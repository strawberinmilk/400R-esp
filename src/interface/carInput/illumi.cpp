#include "illumi.h"
#include "config/pinConfig.h"
#include "output/PwmLeds/footLight.h"
#include "output/PwmLeds/heartLight.h"

extern FootLight footLight;
extern HeartLight heartLight;

// 初期化
void Illumi::init()
{
  pinMode(ILLUMI_INPUT_PIN, INPUT);
  illumiState = digitalRead(ILLUMI_INPUT_PIN);
}

// ループ処理
void Illumi::update()
{
  bool currentIllumiState = digitalRead(ILLUMI_INPUT_PIN);
  if (currentIllumiState != illumiState)
  {
    illumiState = currentIllumiState;
    footLight.lighting();
    heartLight.lighting();
  }
}

// イルミ線の状態を取得
bool Illumi::getIllumiState() const
{
  return illumiState;
}
