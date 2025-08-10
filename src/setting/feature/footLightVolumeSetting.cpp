#include "FootLightVolumeSetting.h"
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
FootLightVolumeSetting::FootLightVolumeSetting()
{
  name = "FootLightVolume";
}

/**
 * 開始処理
 */
void FootLightVolumeSetting::start()
{
  display.print("Foot Light Volume", "Adjusting...");
  int initialVolume = footLight.getVolume();
  encoder.startEncoder(
      initialVolume,
      0,
      255);
}

/**
 * 更新処理
 */
void FootLightVolumeSetting::update()
{
  if (encoder.isUpdateEncoder())
  {
    // エンコーダの値が変わった場合
    // TODO: 液晶一部分の書換にする
    display.print("Foot Light Volume", String(encoder.getCurrentValue()).c_str());
    footLight.setVolume(encoder.getCurrentValue());
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
  encoder.stopEncoder();
  settingManager.currentFeature = &standbyMode;
  delay(500);
  display.print("standby", "");
}
