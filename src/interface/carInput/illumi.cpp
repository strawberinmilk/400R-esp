#include "illumi.h"
#include "config/pinConfig.h"
#include "output/footLight.h"

// volatile bool illumiChanged = false;
// static bool lastIllumiState = false;

// Illumi *Illumi::instance = nullptr;
extern FootLight footLight;

// 初期化
void Illumi::init()
{
  pinMode(ILLUMI_INPUT_PIN, INPUT);
  illumiState = digitalRead(ILLUMI_INPUT_PIN);
  // 割り込み用にインスタンスを定義
  // instance = this;
  // ピン状態変化時にisrHandlerを呼び出す設定
  // attachInterrupt(digitalPinToInterrupt(ILLUMI_INPUT_PIN), isrHandler, CHANGE);
}

// ループ処理
void Illumi::update()
{
  // if (illumiChanged)
  // {
  //   if (illumiState != lastIllumiState)
  //   {
  //     lastIllumiState = illumiState;
  //     footLight.lighting();
  //   }
  //   illumiChanged = false;
  // }
  bool currentIllumiState = digitalRead(ILLUMI_INPUT_PIN);
  if (currentIllumiState != illumiState)
  {
    illumiState = currentIllumiState;
    footLight.lighting();
  }
}

// 割り込み
// void IRAM_ATTR Illumi::isrHandler()
// {
// if (!instance)
//   return;
// bool currentState = digitalRead(ILLUMI_INPUT_PIN);
// if (instance->illumiState != currentState)
// {
//   instance->illumiState = currentState;
//   illumiChanged = true;
// }
// }

// イルミ線の状態を取得
bool Illumi::getIllumiState() const
{
  return illumiState;
}
