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

// 汎用エンコーダー処理関数
int Encoder::runEncoder(
    int initialValue,
    int minValue,
    int maxValue,
    void (*onValueChange)(int value),
    bool (*isBreak)())
{
  setCount(initialValue);
  encoderEnabled = true;

  while (true)
  {
    // エンコーダーの値が変わった場合
    if (updateEncoder(minValue, maxValue))
    {
      onValueChange(currentEncoderValue);
    }

    // 終了条件をチェック
    if (isBreak())
    {
      encoderEnabled = false;
      break;
    }

    delay(1);
  }
  return currentEncoderValue;
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