#include <Arduino.h>
#include "footLight.h"
#include "config/pinConfig.h"
#include "interface/carInput/illumi.h"

extern Illumi illumi;

FootLight::FootLight()
{
  // TODO: フラッシュメモリから取得するよう修正
  VOLUME = 255;
  currentMode = MODE_ON;
}

/**
 * 点灯/消灯処理
 */
void FootLight::lighting()
{
  // モードに基づいて点灯制御
  if (currentMode == MODE_OFF)
  {
    ledcWrite(PWM_CH, 0);
  }
  else if (currentMode == MODE_ON)
  {
    ledcWrite(PWM_CH, VOLUME);
  }
  else if (currentMode == MODE_AUTO_ILLUMI)
  {
    if (illumi.getIllumiState())
    {
      // ライトON - フットライトOFF
      ledcWrite(PWM_CH, 0);
      Serial.println("Foot Light Auto Illumi ON");
    }
    else
    {
      // ライトOFF - フットライトON
      ledcWrite(PWM_CH, VOLUME);
      Serial.println("Foot Light Auto Illumi OFF");
    }
  }
  else if (currentMode == MODE_AUTO_SIDE_BRAKE)
  {
    ledcWrite(PWM_CH, VOLUME);
    Serial.println("TODO: side brake");
  }
}

/**
 * 初期化処理
 */
void FootLight::init()
{
  pinMode(FOOTLIGHT_PIN, OUTPUT);
  ledcSetup(PWM_CH, 120, 8); // 120Hz, 8Bit(256段階)
  ledcAttachPin(FOOTLIGHT_PIN, PWM_CH);
  lighting();
}

/**
 * フットライトの明るさを設定
 */
void FootLight::setVolume(int volume)
{
  // フットライトの明るさを設定
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
 * フットライトのモードを設定
 * @param mode 設定するモード
 */
void FootLight::setMode(FootLightMode mode)
{
  currentMode = mode;

  // モード変更時に点灯状態を更新
  lighting();
}

/**
 * フットライトの明るさを取得
 * @return フットライトの明るさ
 */
int FootLight::getVolume()
{
  return VOLUME;
}

/**
 * フットライトのモードを取得
 * @return 現在のモード
 */
FootLightMode FootLight::getMode()
{
  return currentMode;
}

/**
 * FootLightModeの要素数を取得
 * @return モードの総数
 */
int FootLight::getModeCount()
{
  return static_cast<int>(FOOTLIGHT_MODE_COUNT);
}

/**
 * FootLightModeに対応するテキストを取得
 * @param mode 対象のモード
 * @return モードに対応するテキスト
 */
const char *FootLight::getModeText(FootLightMode mode)
{
  // 配列の範囲チェック
  if (mode >= 0 && mode < FOOTLIGHT_MODE_COUNT)
  {
    return FootLightModeTexts[mode];
  }
  return "Unknown";
}
