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
#include "output/PwmLeds/heartLight.h"
#include "setting/manager.h"
#include "setting/feature/standbyMode.h"

extern Display display;
extern Encoder encoder;
extern Button button;
extern FootLight footLight;
extern HeartLight heartLight;
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

  encoder.startEncoder(0, 0, 4); // 5つのメニュー項目 (0-4)
  display.print("Preset Manager", "Select Menu");
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
  case MENU_CLEAR_ALL:
    updateClearAll();
    break;
  }
}

/**
 * メニュー選択
 */
void PresetManager::updateSelectPreset()
{
  // エンコーダ操作で画面更新
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
      display.print("Preset Manager", "4.Clear All");
      break;
    case 4:
      display.print("Preset Manager", "5.Exit");
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
      currentState = MENU_CLEAR_ALL;
      display.print("Clear All?", "Press to confirm");
      break;

    case 4:
      this->cleanup();
      break;
    }
  }
}

/**
 * プリセット内容保存
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
    // 統合プリセットとして保存
    CombinedPreset preset;
    preset.footLight.volume = footLight.getVolume();
    preset.footLight.mode = footLight.getMode();
    preset.heartLight.volume = heartLight.getVolume();
    preset.heartLight.mode = heartLight.getMode();

    if (nvStorage.saveCombinedPreset(selectedPreset, preset))
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
    encoder.startEncoder(menuIndex, 0, 4);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * プリセット内容を読み出し、LEDに適用
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
    CombinedPreset preset;

    if (nvStorage.loadCombinedPreset(selectedPreset, preset))
    {
      // 両方のLEDに設定を適用
      footLight.setVolume(preset.footLight.volume);
      footLight.setMode(preset.footLight.mode);
      heartLight.setVolume(preset.heartLight.volume);
      heartLight.setMode(preset.heartLight.mode);
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
    encoder.startEncoder(menuIndex, 0, 4);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * 現在の情報表示
 */
void PresetManager::updateCurrentInfo()
{
  // SELECT_SWボタンで戻る
  if (button.isPushAwait(SELECT_SW_PIN))
  {
    currentState = MENU_SELECT_PRESET;
    encoder.startEncoder(menuIndex, 0, 4);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * 全プリセット消去
 */
void PresetManager::updateClearAll()
{
  // SELECT_SWボタンで消去実行
  if (button.isPushAwait(SELECT_SW_PIN))
  {
    nvStorage.clearAll();
    display.print("Clear Complete", "All data erased");
    delay(1500);

    // メインメニューに戻る
    currentState = MENU_SELECT_PRESET;
    encoder.startEncoder(menuIndex, 0, 4);
    display.print("Preset Manager", "Select Menu");
  }
}

/**
 * プリセット情報表示
 */
void PresetManager::displayPresetInfo()
{
  int current = nvStorage.getCurrentPreset();
  CombinedPreset preset;

  if (nvStorage.loadCombinedPreset(current, preset))
  {
    // 現在の設定を取得
    int footVol = footLight.getVolume();
    LedMode footMode = footLight.getMode();
    int heartVol = heartLight.getVolume();
    LedMode heartMode = heartLight.getMode();

    // プリセットと現在の設定を比較
    bool isFootMatched = (preset.footLight.volume == footVol && preset.footLight.mode == footMode);
    bool isHeartMatched = (preset.heartLight.volume == heartVol && preset.heartLight.mode == heartMode);
    bool isMatched = isFootMatched && isHeartMatched;

    char line1[17], line2[17];

    if (isMatched)
    {
      // 一致している場合、プリセット名を表示
      snprintf(line1, sizeof(line1), "PS:%s F:%d H:%d", NvStorage::getPresetName(current), preset.footLight.volume, preset.heartLight.volume);
      snprintf(line2, sizeof(line2), "Fm:%d Hm:%d", (int)preset.footLight.mode, (int)preset.heartLight.mode);
    }
    else
    {
      // 一致していない場合、「not preset」を表示
      snprintf(line1, sizeof(line1), "NOT Preset");
      snprintf(line2, sizeof(line2), "F:%d/%d H:%d/%d", footVol, (int)footMode, heartVol, (int)heartMode);
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
