#include "modeSelect.h"
#include "interface/display.h"
#include "interface/encoder.h"
#include "interface/button.h"
#include "config/pinConfig.h"
#include "FootLightVolumeSetting.h"
#include "FootLightModeSetting.h"
#include "setting/manager.h"

// mainで定義
extern Display display;
extern Encoder encoder;
extern Button button;

// managerで定義
extern FootLightVolumeSetting footLightVolumeSetting;
extern FootLightModeSetting footLightModeSetting;
extern SettingManager settingManager;

ModeSelect::ModeSelect()
{
  name = "ModeSelect";

  settingManager.selectedMode = 0;
  settingManager.modeSelected = false;
}

void ModeSelect::start()
{
  display.print("Mode Select", "Choose mode");
  delay(500);

  // エンコーダーを初期化（1からMODE_COUNT-1の範囲）
  // 0はセレクトモードなので、1から開始
  encoder.startEncoder(1, 1, settingManager.MODE_COUNT - 1);
  settingManager.selectedMode = 0;
  settingManager.modeSelected = true;

  // 初回表示
  display.print("Mode Select", settingManager.modeNames[0]);
}

void ModeSelect::update()
{
  // エンコーダの値が変わった場合
  if (encoder.isUpdateEncoder())
  {
    settingManager.selectedMode = encoder.getCurrentValue();
    display.print("Mode Select", settingManager.modeNames[settingManager.selectedMode]);
  }

  // ボタンが押されて離された場合にモード決定
  if (button.isPushAwait(SELECT_SW_PIN))
  {
    this->cleanup();
    settingManager.selectedMode = encoder.getCurrentValue();

    // 選択されたモードを起動
    if (settingManager.selectedMode == 1)
    {
      // Foot Light Volume
      startFeature(footLightVolumeSetting);
    }
    else if (settingManager.selectedMode == 2)
    {
      // Foot Light Mode
      startFeature(footLightModeSetting);
    }
  }
}

void ModeSelect::cleanup()
{
  // エンコーダー停止
  encoder.stopEncoder();
  settingManager.currentFeature = nullptr;
  settingManager.selectedMode = -1;
  settingManager.modeSelected = false;

  Serial.println("ModeSelect cleaned up");
}

void ModeSelect::startFeature(BaseSetting &newFeature)
{
  settingManager.currentFeature = &newFeature;
  settingManager.currentFeature->start();
}
