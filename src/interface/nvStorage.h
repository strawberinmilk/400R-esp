
#ifndef NV_STORAGE_H
#define NV_STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include "output/PwmLeds/footLight.h"

// グローバルプリセット情報
extern const int PRESET_COUNT;
extern const int PRESET_INDEX_RIN;
extern String PRESET_KEYS[];

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

public:
  NvStorage();
  void init();

  // プリセット操作
  bool savePreset(int preset, const FootLightPreset &data);
  bool loadPreset(int preset, FootLightPreset &data);

  // 現在のプリセット管理
  bool setCurrentPreset(int preset);
  int getCurrentPreset();

  // プリセット名取得
  static const char *getPresetName(int preset);

  // 文字列→PresetName変換
  static int presetNameFromString(const String &name);
  size_t getFreeSpace();
  void clearAll();
};

#endif
