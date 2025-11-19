#include <Arduino.h>
#include "PwmLed.h"
#include "interface/carInput/illumi.h"
#include "interface/carInput/handBrake.h"

extern Illumi illumi;
extern HandBrake handBrake;

PwmLed::PwmLed(int pin, int pwmChannel)
    : VOLUME(255), currentMode(MODE_ON), pin(pin), pwmChannel(pwmChannel)
{
}

/**
 * 初期化処理
 */
void PwmLed::init()
{
  pinMode(pin, OUTPUT);
  ledcSetup(pwmChannel, 120, 8); // 120Hz, 8Bit(256段階)
  ledcAttachPin(pin, pwmChannel);
  lighting();
}

/**
 * 明るさを設定
 */
void PwmLed::setVolume(int volume)
{
  if (volume < 0)
  {
    VOLUME = 0;
  }
  else if (volume > 255)
  {
    VOLUME = 255;
  }
  else
  {
    VOLUME = volume;
  }
  lighting();
}

/**
 * モードを設定
 * @param mode 設定するモード
 */
void PwmLed::setMode(LedMode mode)
{
  currentMode = mode;
  lighting();
}

/**
 * 明るさを取得
 * @return 明るさ
 */
int PwmLed::getVolume() const
{
  return VOLUME;
}

/**
 * モードを取得
 * @return 現在のモード
 */
LedMode PwmLed::getMode() const
{
  return currentMode;
}

/**
 * 点灯/消灯処理のデフォルト実装
 */
void PwmLed::applyLighting()
{
  // モードに基づいて点灯制御
  if (currentMode == MODE_OFF)
  {
    ledcWrite(pwmChannel, 0);
  }
  else if (currentMode == MODE_ON)
  {
    ledcWrite(pwmChannel, VOLUME);
  }
  else if (currentMode == MODE_AUTO_ILLUMI)
  {
    if (illumi.getIllumiState())
    {
      // ライトON - LED OFF
      ledcWrite(pwmChannel, 0);
    }
    else
    {
      // ライトOFF - LED ON
      ledcWrite(pwmChannel, VOLUME);
    }
  }
  else if (currentMode == MODE_AUTO_SIDE_BRAKE)
  {
    if (handBrake.getHandBrakeState())
    {
      // サイドブレーキON - LED ON
      ledcWrite(pwmChannel, VOLUME);
    }
    else
    {
      // サイドブレーキOFF - LED OFF
      ledcWrite(pwmChannel, 0);
    }
  }
}

/**
 * 点灯/消灯処理
 */
void PwmLed::lighting()
{
  applyLighting();
}

/**
 * LedModeの要素数を取得
 * @return モードの総数
 */
int PwmLed::getModeCount()
{
  return LED_MODE_COUNT;
}

/**
 * LedModeに対応するテキストを取得
 * @param mode 対象のモード
 * @return モードに対応するテキスト
 */
const char *PwmLed::getModeText(LedMode mode)
{
  if (mode >= 0 && mode < LED_MODE_COUNT)
  {
    return LedModeTexts[mode];
  }
  return "Unknown";
}
