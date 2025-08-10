#include "manager.h"
#include "setting/feature/modeSelect.h"
#include "setting/feature/footLightVolumeSetting.h"
#include "setting/feature/footLightModeSetting.h"
#include "setting/feature/PresetManager.h"
#include "setting/feature/StandbyMode.h"
#include "interface/button.h"
#include "config/pinConfig.h"

ModeSelect modeSelect;
FootLightVolumeSetting footLightVolumeSetting;
FootLightModeSetting footLightModeSetting;
PresetManager presetManager;
StandbyMode standbyMode;
extern Button button;

/**
 * コンストラクタ
 */
SettingManager::SettingManager()
{
  // 初期状態はstandbyモード
  currentFeature = &standbyMode;
  int selectedMode = -1;
  bool modeSelected = false;
}

/**
 * ループで呼ばれる処理
 */
void SettingManager::update()
{
  if (currentFeature)
  {
    currentFeature->update();
  }
  else
  {
    // currentFeatureがnullptrの場合はモード選択を開始
    currentFeature = &modeSelect;
    modeSelected = true;
    selectedMode = 0; // modeSelectを起動
    modeSelect.start();
  }
}

void SettingManager::saveSettings()
{
  // Save settings implementation
}