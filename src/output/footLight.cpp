#include <Arduino.h>
#include "footLight.h"
#include "config/pinConfig.h"

FootLight::FootLight()
{
  VOLUME = 255;
  IS_LIGHTING = true;
}

// 点灯/消灯処理
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

// 初期化処理
void FootLight::init()
{
  pinMode(FOOTLIGHT_PIN, OUTPUT);
  ledcSetup(PWM_CH, 7812.5, 8); // 7812.5Hz, 8Bit(256段階)
  ledcAttachPin(FOOTLIGHT_PIN, PWM_CH);
  lighting();
}

// 明るさ設定
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

void FootLight::setIsLighting(boolean isLighting)
{
  // フットライトの点灯状態を設定
  IS_LIGHTING = isLighting;
  Serial.println(IS_LIGHTING ? "Foot Light ON" : "Foot Light OFF");
  lighting();
}

// 明るさ取得
int FootLight::getVolume()
{
  return VOLUME;
}

// 点灯状態取得
boolean FootLight::isLighting()
{
  return IS_LIGHTING;
}