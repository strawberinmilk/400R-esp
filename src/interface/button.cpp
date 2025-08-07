#include <Arduino.h>
#include "button.h"

Button::Button()
{
  // コンストラクタ
}

/**
 * ボタンが押されているかどうかを判定
 * 即次の処理が走るため、押下を継続する用途向け
 * @param pinNumber ボタンのピン番号
 * @return ボタンが押されている場合はtrue、そうでない場合はfalse
 */
bool Button::isPush(int pinNumber)
{
  return digitalRead(pinNumber) == LOW;
}

/**
 * ボタンが押されていた場合に押されていた時間を返す
 * ボタンが離されるまで待機するため、長押し検出などに利用できる
 * @param pinNumber ボタンのピン番号
 * @return ボタンが押されていた時間（ミリ秒）
 */
unsigned long Button::isPushAwait(int pinNumber)
{
  // ボタンが押されていない場合は0を返す
  if (digitalRead(pinNumber) == HIGH)
  {
    return 0;
  }

  // ボタンが押された時刻を記録
  unsigned long startTime = millis();

  // ボタンが離されるまで待機
  while (digitalRead(pinNumber) == LOW)
  {
    delay(1); // CPU負荷軽減のための短い遅延
  }

  // 押されていた時間を計算して返す
  return millis() - startTime;
}
