
#ifndef NV_STORAGE_H
#define NV_STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include "output/PwmLed.h"

// グローバルプリセット情報
extern const int PRESET_COUNT;
extern const int PRESET_INDEX_RIN;

// プリセット構造体（汎用）
struct LedPreset
{
  int volume;   // 明るさ (0-255)
  LedMode mode; // モード
};

// 統合プリセット構造体（FootLight + HeartLight）
struct CombinedPreset
{
  LedPreset footLight;
  LedPreset heartLight;
};

// 互換性のためのエイリアス
typedef LedPreset FootLightPreset;

class NvStorage
{
private:
  Preferences preferences;
  static const char *NAMESPACE;
  static const char *CURRENT_PRESET_KEY;

public:
  NvStorage();
  void init();

  // 統合プリセット操作
  bool saveCombinedPreset(int preset, const CombinedPreset &data);
  bool loadCombinedPreset(int preset, CombinedPreset &data);

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
