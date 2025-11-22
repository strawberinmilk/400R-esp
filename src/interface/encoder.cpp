#include <Arduino.h>

#include "config/pinConfig.h"
#include "interface/encoder.h"

/**
 * コンストラクタ
 */
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
 * 現在のエンコーダーの値を取得
 */
int Encoder::getCurrentValue()
{
  return currentEncoderValue;
}

/**
 * エンコーダの値を更新
 * @param min 最小値
 * @param max 最大値
 * @return 値が更新された場合はtrue、そうでない場合はfalse
 */
boolean Encoder::updateEncoder(int min, int max)
{
  if (!isRunning)
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
  // Serial.println(String(count) + " -> " + String(fixedCount));
  currentEncoderValue = fixedCount;
  oldEncoderValue = count;
  return true;
}

/**
 * 非ブロッキング版エンコーダー処理関数
 */
void Encoder::startEncoder(int initialValue, int minValue, int maxValue)
{
  // 既に実行中の場合は停止
  if (isRunning)
  {
    stopEncoder();
  }

  // パラメータ設定
  this->minValue = minValue;
  this->maxValue = maxValue;

  // エンコーダー初期化
  setCount(initialValue);
  isRunning = true;
}

bool Encoder::isUpdateEncoder()
{
  // 実行中でない場合は何もしない
  if (!isRunning)
  {
    return false;
  }

  // エンコーダーの値が変わった場合
  if (updateEncoder(minValue, maxValue))
  {
    return true; // 値が変更された
  }

  return false; // 値は変更されていない
}

/**
 * エンコーダーを停止
 */
void Encoder::stopEncoder()
{
  if (isRunning)
  {
    isRunning = false;
  }
}
