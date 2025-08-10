#include "manager.h"
#include "setting/feature/modeSelect.h"
#include "setting/feature/footLightVolumeSetting.h"
#include "setting/feature/footLightModeSetting.h"
#include "interface/button.h"
#include "config/pinConfig.h"

ModeSelect modeSelect;
FootLightVolumeSetting footLightVolumeSetting;
FootLightModeSetting footLightModeSetting;
extern Button button;


/**
 * コンストラクタ
 */
SettingManager::SettingManager()
{
  currentFeature = nullptr;
  int selectedMode = -1;
  bool modeSelected = false;
}

/**
 * 更新処理
 */
void SettingManager::update()
{
  if (currentFeature)
  {
    currentFeature->update();
  } else if (button.isPushAwait(SELECT_SW_PIN)) {
    // モード選択を開始
    currentFeature = &modeSelect;
    modeSelected = true;
    selectedMode = 0; // modeSelectを起動
    modeSelect.start();
  } else {
    
  }
}

void SettingManager::saveSettings()
{
  // Save settings implementation
}