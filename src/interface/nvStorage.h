#ifndef NV_STORAGE_H
#define NV_STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include "output/footLight.h"

// プリセット構造体
struct FootLightPreset
{
  int volume;         // 明るさ (0-255)
  FootLightMode mode; // モード
};

class NvStorage
{
private:
  Preferences preferences;
  static const char *NAMESPACE;
  static const char *CURRENT_PRESET_KEY;
  static const char *PRESET_KEYS[];

public:
  // プリセット名列挙
  enum PresetName
  {
    PRESET_RIN = 0,
    PRESET_IONGRID = 1,
    PRESET_FUJI = 2,
    PRESET_MASUTATE = 3,
    PRESET_CUSTOM1 = 4,
    PRESET_CUSTOM2 = 5,
    PRESET_COUNT = 6
  };

  NvStorage();
  void init();

  // プリセット操作
  bool savePreset(PresetName preset, const FootLightPreset &data);
  bool loadPreset(PresetName preset, FootLightPreset &data);

  // 現在のプリセット管理
  bool setCurrentPreset(PresetName preset);
  PresetName getCurrentPreset();

  // プリセット名取得
  static const char *getPresetName(PresetName preset);

  // ストレージ管理
  void clearAll();
  size_t getFreeSpace();
};

#endif
