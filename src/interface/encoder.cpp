#include <Arduino.h>

#include "config/pinConfig.h"
#include "interface/encoder.h"

volatile bool encoderEnabled = false;
int currentEncoderValue = 0;
int oldEncoderValue = 0;

// エンコーダ用変数
Encoder::Encoder()
{
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  esp32Encoder.attachSingleEdge(ENCODER_A_PIN, ENCODER_B_PIN);
}

/**
 * カウント値を設定
 * @param value 設定するカウント値
 */
void Encoder::setCount(int value)
{
  esp32Encoder.setCount(value);
}

/**
 * エンコーダの値を更新
 * @param min 最小値
 * @param max 最大値
 * @return 値が更新された場合はtrue、そうでない場合はfalse
 */
boolean Encoder::updateEncoder(int min, int max)
{
  if (!encoderEnabled)
    return false; // 停止中は何もしない

  int count = esp32Encoder.getCount();
  // 値が変わっていない場合は何もしない
  if (count == oldEncoderValue)
    return false;

  int fixedCount = count;
  if (count < min)
  {
    fixedCount = max;
    esp32Encoder.setCount(fixedCount);
  }
  else if (max < count)
  {
    fixedCount = min;
    esp32Encoder.setCount(fixedCount);
  }
  Serial.println(String(count) + " -> " + String(fixedCount));
  currentEncoderValue = fixedCount;
  oldEncoderValue = count;
  return true;
}

// // 汎用エンコーダー処理関数
// int Encoder::runEncoder(
//     int initialValue,
//     int minValue,
//     int maxValue,
//     void (*onValueChange)(int value),
//     bool (*isBreak)())
// {
//   setCount(initialValue);
//   encoderEnabled = true;

//   while (true)
//   {
//     // エンコーダーの値が変わった場合
//     if (updateEncoder(minValue, maxValue))
//     {
//       onValueChange(currentEncoderValue);
//     }

//     // 終了条件をチェック
//     if (isBreak())
//     {
//       encoderEnabled = false;
//       break;
//     }

//     delay(1);
//   }
//   return currentEncoderValue;
// }

/**
 * 非ブロッキング版エンコーダー処理関数
 */
void Encoder::startEncoderV2(int initialValue, int minValue, int maxValue)
{
  // 既に実行中の場合は停止
  if (isRunningV2)
  {
    stopEncoderV2();
  }

  // パラメータ設定
  minValueV2 = minValue;
  maxValueV2 = maxValue;

  // エンコーダー初期化
  setCount(initialValue);
  encoderEnabled = true;
  isRunningV2 = true;

  Serial.println("Encoder V2 started");
}

bool Encoder::updateEncoderV2()
{
  // 実行中でない場合は何もしない
  if (!isRunningV2)
  {
    return false;
  }

  // エンコーダーの値が変わった場合
  if (updateEncoder(minValueV2, maxValueV2))
  {
    return true; // 値が変更された
  }

  return false; // 値は変更されていない
}

void Encoder::stopEncoderV2()
{
  if (isRunningV2)
  {
    encoderEnabled = false;
    isRunningV2 = false;
    Serial.println("Encoder V2 stopped");
  }
}