#include <Arduino.h>

#include "config/pinConfig.h"
#include "interface/encoder.h"

volatile bool encoderEnabled = false;
int currentEncoderValue = 0;
int oldEncoderValue = 0;

// エンコーダ用変数
Encoder::Encoder()
{
  pinMode(SELECT_SW_PIN, INPUT_PULLUP);
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  esp32Encoder.attachSingleEdge(ENCODER_A_PIN, ENCODER_B_PIN);
}

void Encoder::setCount(int value)
{
  esp32Encoder.setCount(value);
}

// エンコーダ用汎用関数
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

// エンコーダサンプル
// encoder.setCount(初期値);
// encoderEnabled = true;
// while (true)
// {
//   if (updateEncoder(最小値, 最大値))
//   {
//     Serial.println(currentEncoderValue);
//   }
//   if (エンコーダを抜ける条件)
//   {
//     encoderEnabled = false;
//     break;
//   }
// }