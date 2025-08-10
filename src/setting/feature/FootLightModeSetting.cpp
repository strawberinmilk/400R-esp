#include "FootLightModeSetting.h"
#include "output/footLight.h"
#include "interface/display.h"
#include "interface/encoder.h"
#include "config/pinConfig.h"
#include "interface/button.h"
#include "setting/manager.h"

extern Display display;
extern FootLight footLight;
extern Encoder encoder;
extern Button button;
extern SettingManager settingManager;

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
  // 現在のモード状態を取得（ON=1, OFF=0）
  int initialValue = footLight.isLighting() ? 1 : 0;

  encoder.startEncoderV2(
      initialValue,
      0,  // 最小値: OFF
      1); // 最大値: ON
}

/**
 * 更新処理
 */
void FootLightModeSetting::update()
{
  if (encoder.updateEncoderV2())
  {
    // エンコーダの値が変わった場合
    bool isOn = (encoder.currentEncoderValue == 1);
    const char *modeText = isOn ? "ON" : "OFF";

    display.print("Foot Light Mode", modeText);
    footLight.setIsLighting(isOn);
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
  encoder.stopEncoderV2();
  settingManager.currentFeature = nullptr;
  delay(500);
  display.print("standby", "");
}
