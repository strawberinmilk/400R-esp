#include "FootLightVolumeSetting.h"
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
FootLightVolumeSetting::FootLightVolumeSetting()
{
  name = "FootLightVolume";
}

/**
 * 初期化処理
 */
void FootLightVolumeSetting::start()
{
  encoder.startEncoderV2(
      footLight.getVolume(),
      0,
      255);
}

/**
 * 更新処理
 */
void FootLightVolumeSetting::update()
{
  if (encoder.updateEncoderV2())
  {
    // エンコーダの値が変わった場合
    // TODO: 液晶一部分の書換にする
    display.print("Foot Light Volume", String(encoder.currentEncoderValue).c_str());
    footLight.setVolume(encoder.currentEncoderValue);
  }

  if (button.isPushAwait(SELECT_SW_PIN))
  {
    this->cleanup();
  }
}

/**
 * クリーンアップ処理
 */
void FootLightVolumeSetting::cleanup()
{
  display.print("Foot Light Volume", "success");
  encoder.stopEncoderV2();
  settingManager.currentFeature = nullptr;
  delay(500);
  display.print("standby", "");
}
