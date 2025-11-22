#include "BaseLedModeSetting.h"
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
BaseLedModeSetting::BaseLedModeSetting(PwmLed *led, const char *name)
    : targetLed(led), ledName(name)
{
  this->name = name;
}

/**
 * 開始処理
 */
void BaseLedModeSetting::start()
{
  // 現在のモードを取得
  int initialValue = (int)targetLed->getMode();

  encoder.startEncoder(
      initialValue,
      0,
      PwmLed::getModeCount() - 1); // 0 から モード数-1 まで
}

/**
 * 更新処理
 */
void BaseLedModeSetting::update()
{
  if (encoder.isUpdateEncoder())
  {
    // エンコーダの値が変わった場合
    int currentMode = encoder.getCurrentValue();
    const char *modeText = PwmLed::getModeText((LedMode)currentMode);

    String title = String(ledName) + " Mode";
    display.print(title.c_str(), modeText);
    targetLed->setMode((LedMode)currentMode);
  }

  if (button.isPushAwait(SELECT_SW_PIN))
  {
    this->cleanup();
  }
}

/**
 * クリーンアップ処理
 */
void BaseLedModeSetting::cleanup()
{
  String title = String(ledName) + " Mode";
  display.print(title.c_str(), "success");
  encoder.stopEncoder();
  delay(500);
  settingManager.currentFeature = &standbyMode;
}
