#include "interface/nvStorage.h"
extern const int PRESET_INDEX_RIN;
extern const int PRESET_COUNT;
#include "PresetManager.h"
#include "interface/nvStorage.h"
#include "interface/display.h"
#include "interface/encoder.h"
#include "interface/button.h"
#include "config/pinConfig.h"
#include "output/PwmLeds/footLight.h"
#include "setting/manager.h"
#include "setting/feature/standbyMode.h"

extern Display display;
extern Encoder encoder;
extern Button button;
extern FootLight footLight;
extern SettingManager settingManager;
extern StandbyMode standbyMode;
extern NvStorage nvStorage;

/**
 * コンストラクタ
 */
PresetManager::PresetManager()
{
  name = "Preset Manager";
  currentState = MENU_SELECT_PRESET;
  selectedPreset = PRESET_INDEX_RIN;
  menuIndex = 0;
}

/**
 * 開始処理
 */
void PresetManager::start()
{
  currentState = MENU_SELECT_PRESET;
  selectedPreset = nvStorage.getCurrentPreset();
  menuIndex = 0;

  encoder.startEncoder(0, 0, 3); // 4つのメニュー項目 (0-3)
  display.print("Preset Manager", "Select Menu");

  Serial.println("PresetManager started");
}

/**
 * 更新処理
 */
void PresetManager::update()
{
  switch (currentState)
  {
  case MENU_SELECT_PRESET:
    updateSelectPreset();
    break;
  case MENU_SAVE_PRESET:
    updateSavePreset();
    break;
  case MENU_LOAD_PRESET:
    updateLoadPreset();
    break;
  case MENU_CURRENT_INFO:
    updateCurrentInfo();
    break;
  }
}

/**
 * プリセット選択メニュー更新
 */
void PresetManager::updateSelectPreset()
{
  if (encoder.isUpdateEncoder())
  {
    menuIndex = encoder.getCurrentValue();

    switch (menuIndex)
    {
    case 0:
      display.print("Preset Manager", "1.Load Preset");
      break;
    case 1:
      display.print("Preset Manager", "2.Save Preset");
      break;
    case 2:
      display.print("Preset Manager", "3.Current Info");
      break;
    case 3:
      display.print("Preset Manager", "4.Exit");
      break;
    }
  }

  // エンコーダープッシュで項目決定 (SELECT_SWボタンを使用)
  if (button.isPushAwait(SELECT_SW_PIN))
  {
    switch (menuIndex)
    {
    case 0:
      currentState = MENU_LOAD_PRESET;
      selectedPreset = nvStorage.getCurrentPreset();
      encoder.startEncoder(selectedPreset, 0, PRESET_COUNT - 1);
      display.print("Load Preset", NvStorage::getPresetName(selectedPreset));
      break;

    case 1:
      currentState = MENU_SAVE_PRESET;
      selectedPreset = nvStorage.getCurrentPreset();
      encoder.startEncoder(selectedPreset, 0, PRESET_COUNT - 1);
      display.print("Save Preset", NvStorage::getPresetName(selectedPreset));
      break;

    case 2:
      currentState = MENU_CURRENT_INFO;
      displayPresetInfo();
      break;

    case 3:
      this->cleanup();
      break;
    }
  }
}

/**
 * プリセット保存メニュー更新
 */
void PresetManager::updateSavePreset()
{
  if (encoder.isUpdateEncoder())
  {
    selectedPreset = encoder.getCurrentValue();
    display.print("Save Preset", NvStorage::getPresetName(selectedPreset));
  }

  if (button.isPushAwait(SELECT_SW_PIN))
  {
    // 現在のフットライト設定を保存
    FootLightPreset preset;
    preset.volume = footLight.getVolume();
    preset.mode = footLight.getMode();

    if (nvStorage.savePreset(selectedPreset, preset))
    {
      nvStorage.setCurrentPreset(selectedPreset);
      display.print("Save Complete", NvStorage::getPresetName(selectedPreset));
      delay(1000);
    }
    else
    {
      display.print("Save Failed", NvStorage::getPresetName(selectedPreset));
      delay(1000);
    }

    // メインメニューに戻る
    currentState = MENU_SELECT_PRESET;
    encoder.startEncoder(menuIndex, 0, 3);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * プリセット読み込みメニュー更新
 */
void PresetManager::updateLoadPreset()
{
  if (encoder.isUpdateEncoder())
  {
    selectedPreset = encoder.getCurrentValue();
    display.print("Load Preset", NvStorage::getPresetName(selectedPreset));
  }

  if (button.isPushAwait(SELECT_SW_PIN))
  {
    FootLightPreset preset;

    if (nvStorage.loadPreset(selectedPreset, preset))
    {
      // フットライトに設定を適用
      footLight.setVolume(preset.volume);
      footLight.setMode(preset.mode);
      nvStorage.setCurrentPreset(selectedPreset);

      display.print("Load Complete", NvStorage::getPresetName(selectedPreset));
      delay(1000);
    }
    else
    {
      display.print("Load Failed", NvStorage::getPresetName(selectedPreset));
      delay(1000);
    }

    // メインメニューに戻る
    currentState = MENU_SELECT_PRESET;
    encoder.startEncoder(menuIndex, 0, 3);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * 現在の情報表示更新
 */
void PresetManager::updateCurrentInfo()
{
  // SELECT_SWボタンで戻る
  if (button.isPushAwait(SELECT_SW_PIN))
  {
    currentState = MENU_SELECT_PRESET;
    encoder.startEncoder(menuIndex, 0, 3);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * プリセット情報表示
 */
void PresetManager::displayPresetInfo()
{
  int current = nvStorage.getCurrentPreset();
  FootLightPreset preset;

  if (nvStorage.loadPreset(current, preset))
  {
    // 現在のfootLightの設定を取得
    int currentVolume = footLight.getVolume();
    FootLightMode currentMode = footLight.getMode();

    // プリセットとfootLightの設定を比較
    bool isMatched = (preset.volume == currentVolume && preset.mode == currentMode);

    char line1[17], line2[17];

    if (isMatched)
    {
      // 一致している場合、プリセット名を表示
      snprintf(line1, sizeof(line1), "Preset: %s", NvStorage::getPresetName(current));
      snprintf(line2, sizeof(line2), "Vol:%d Mode:%d", preset.volume, (int)preset.mode);
    }
    else
    {
      // 一致していない場合、「not preset」を表示
      snprintf(line1, sizeof(line1), "Preset: not preset");
      snprintf(line2, sizeof(line2), "Vol:%d Mode:%d", currentVolume, (int)currentMode);
    }

    display.print(line1, line2);
  }
  else
  {
    display.print("Current Info", "Load Error");
  }
}

/**
 * クリーンアップ処理
 */
void PresetManager::cleanup()
{
  display.print("Preset Manager", "Exit");
  encoder.stopEncoder();
  settingManager.currentFeature = &standbyMode;
  delay(500);
}
