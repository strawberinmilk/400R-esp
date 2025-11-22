#include "BaseVolumeSetting.h"
#include "interface/display.h"
#include "interface/encoder.h"
#include "config/pinConfig.h"
#include "interface/button.h"
#include "setting/manager.h"
#include "setting/feature/standbyMode.h"

extern Display display;
extern Encoder encoder;
extern Button button;
extern SettingManager settingManager;
extern StandbyMode standbyMode;

/**
 * コンストラクタ
 */
BaseVolumeSetting::BaseVolumeSetting(PwmLed *led, const char *name)
    : targetLed(led), ledName(name)
{
  this->name = name;
}

/**
 * 開始処理
 */
void BaseVolumeSetting::start()
{
  String title = String(ledName) + " Volume";
  display.print(title.c_str(), "Adjusting...");
  int initialVolume = targetLed->getVolume();
  encoder.startEncoder(
      initialVolume,
      0,
      255);
}

/**
 * 更新処理
 */
void BaseVolumeSetting::update()
{
  if (encoder.isUpdateEncoder())
  {
    // エンコーダの値が変わった場合
    String title = String(ledName) + " Volume";
    display.print(title.c_str(), String(encoder.getCurrentValue()).c_str());
    targetLed->setVolume(encoder.getCurrentValue());
  }

  if (button.isPushAwait(SELECT_SW_PIN))
  {
    this->cleanup();
  }
}

/**
 * クリーンアップ処理
 */
void BaseVolumeSetting::cleanup()
{
  String title = String(ledName) + " Volume";
  display.print(title.c_str(), "success");
  encoder.stopEncoder();
  delay(500);
  settingManager.currentFeature = &standbyMode;
}
