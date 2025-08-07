#include <Arduino.h>
#include "footLight.h"
#include "config/pinConfig.h"

FootLight::FootLight()
{
  // TODO: フラッシュメモリから取得するよう修正
  VOLUME = 255;
  IS_LIGHTING = true;
}

/**
 * 点灯/消灯処理
 */
void FootLight::lighting()
{
  if (IS_LIGHTING)
  {
    ledcWrite(PWM_CH, VOLUME);
  }
  else
  {
    ledcWrite(PWM_CH, 0);
  }
}

/**
 * 初期化処理
 */
void FootLight::init()
{
  pinMode(FOOTLIGHT_PIN, OUTPUT);
  ledcSetup(PWM_CH, 7812.5, 8); // 7812.5Hz, 8Bit(256段階)
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
 * フットライトの点灯状態を設定
 * @param isLighting 点灯状態
 */
void FootLight::setIsLighting(boolean isLighting)
{
  IS_LIGHTING = isLighting;
  Serial.println(IS_LIGHTING ? "Foot Light ON" : "Foot Light OFF");
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
 * フットライトの点灯状態を取得
 * @return フットライトの点灯状態
 */
boolean FootLight::isLighting()
{
  return IS_LIGHTING;
}