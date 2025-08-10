#include "FootLightModeSetting.h"
#include "output/footLight.h"
#include "interface/display.h"
#include "interface/encoder.h"
#include "config/pinConfig.h"
#include "interface/button.h"
#include "setting/manager.h"

#include "setting/feature/standbyMode.h"

extern Display display;
extern FootLight footLight;
extern Encoder encoder;
extern Button button;
extern SettingManager settingManager;
extern StandbyMode standbyMode;

/**
 * コンストラクタ
 */
FootLightModeSetting::FootLightModeSetting()
{
  name = "FootLightMode";
}

/**
 * 初期化処理
 */
void FootLightModeSetting::start()
{
  // 現在のモードを取得
  int initialValue = (int)footLight.getMode();

  encoder.startEncoder(
      initialValue,
      0,
      FootLight::getModeCount() - 1); // 0 から モード数-1 まで
}

/**
 * 更新処理
 */
void FootLightModeSetting::update()
{
  if (encoder.isUpdateEncoder())
  {
    // エンコーダの値が変わった場合
    int currentMode = encoder.getCurrentValue();
    const char *modeText = FootLight::getModeText((FootLightMode)currentMode);

    display.print("Foot Light Mode", modeText);
    footLight.setMode((FootLightMode)currentMode);
  }

  if (button.isPushAwait(SELECT_SW_PIN))
  {
    this->cleanup();
  }
}

/**
 * クリーンアップ処理
 */
void FootLightModeSetting::cleanup()
{
  display.print("Foot Light Mode", "success");
  encoder.stopEncoder();
  delay(500);
  settingManager.currentFeature = &standbyMode;
}
