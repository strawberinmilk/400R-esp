#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include "../BaseSetting.h"
#include "interface/nvStorage.h"

class PresetManager : public BaseSetting
{
private:
  enum MenuState
  {
    MENU_SELECT_PRESET, // プリセット選択
    MENU_SAVE_PRESET,   // プリセット保存
    MENU_LOAD_PRESET,   // プリセット読み込み
    MENU_CURRENT_INFO   // 現在の設定情報表示
  };

  MenuState currentState;
  int selectedPreset;
  int menuIndex;

  void updateSelectPreset();
  void updateSavePreset();
  void updateLoadPreset();
  void updateCurrentInfo();
  void displayPresetInfo();

public:
  PresetManager();
  void start() override;
  void update() override;
  void cleanup() override;
};

#endif
