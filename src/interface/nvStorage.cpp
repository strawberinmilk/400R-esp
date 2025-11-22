#include "nvStorage.h"

// プリセット名とenum値の一元管理
struct PresetNameMapEntry
{
  const char *name;
};

// プリセット名の管理
static const PresetNameMapEntry presetNameMap[] = {
    {"rin"},
    {"fuji"},
    {"masutate"},
    {"IonGrid"},
    {"custom1"},
    {"custom2"}};

const size_t presetNameMapSize = sizeof(presetNameMap) / sizeof(presetNameMap[0]);
const int PRESET_COUNT = (int)presetNameMapSize;
const int PRESET_INDEX_RIN = 0;

// プリセット名をプリセット数値に変換
int NvStorage::presetNameFromString(const String &name)
{
  for (size_t i = 0; i < presetNameMapSize; ++i)
  {
    if (name == presetNameMap[i].name)
    {
      return (int)i;
    }
  }
  return (int)PRESET_COUNT;
}

// 定数定義
const char *NvStorage::NAMESPACE = "footlight";
const char *NvStorage::CURRENT_PRESET_KEY = "current";

/**
 * コンストラクタ
 */
NvStorage::NvStorage()
{
  // デフォルトプリセットの設定（初回起動時）
  if (!preferences.begin(NAMESPACE, true))
  { // 読み取り専用でチェック
    preferences.end();
    preferences.begin(NAMESPACE, false); // 書き込み可能で開く

    // デフォルト値の設定
    CombinedPreset defaultPreset;
    defaultPreset.footLight.volume = 128;
    defaultPreset.footLight.mode = MODE_ON;
    defaultPreset.heartLight.volume = 128;
    defaultPreset.heartLight.mode = MODE_ON;

    // 全プリセットにデフォルト値を設定
    for (int i = 0; i < PRESET_COUNT; i++)
    {
      CombinedPreset existing;
      if (!loadCombinedPreset(i, existing))
      {
        saveCombinedPreset(i, defaultPreset);
      }
    }

    // 現在のプリセットをRINに設定
    if (getCurrentPreset() == PRESET_COUNT)
    { // 無効値の場合
      setCurrentPreset(PRESET_INDEX_RIN);
    }
  }
  preferences.end();
}

/**
 * 初期化処理
 */
// フットライトとハートライトの現在プリセットを起動時に反映
#include "output/PwmLeds/footLight.h"
#include "output/PwmLeds/heartLight.h"
extern FootLight footLight;
extern HeartLight heartLight;

void NvStorage::init()
{
  // デフォルトプリセットの設定（初回起動時）
  if (!preferences.begin(NAMESPACE, true))
  { // 読み取り専用でチェック
    preferences.end();
    preferences.begin(NAMESPACE, false); // 書き込み可能で開く

    // デフォルト値の設定
    CombinedPreset defaultPreset;
    defaultPreset.footLight.volume = 128;
    defaultPreset.footLight.mode = MODE_ON;
    defaultPreset.heartLight.volume = 128;
    defaultPreset.heartLight.mode = MODE_ON;

    // 全プリセットにデフォルト値を設定
    for (int i = 0; i < PRESET_COUNT; i++)
    {
      CombinedPreset existing;
      if (!loadCombinedPreset(i, existing))
      {
        saveCombinedPreset(i, defaultPreset);
      }
    }

    // 現在のプリセットをRINに設定
    if (getCurrentPreset() == PRESET_COUNT)
    { // 無効値の場合
      setCurrentPreset(PRESET_INDEX_RIN);
    }
  }
  preferences.end();

  // 起動時にcurrentPresetを両方のLEDへ反映
  int current = getCurrentPreset();
  CombinedPreset preset;
  if (loadCombinedPreset(current, preset))
  {
    footLight.setVolume(preset.footLight.volume);
    footLight.setMode(preset.footLight.mode);
    heartLight.setVolume(preset.heartLight.volume);
    heartLight.setMode(preset.heartLight.mode);
  }
}

/**
 * 統合プリセット保存（FootLight + HeartLight）
 */
bool NvStorage::saveCombinedPreset(int preset, const CombinedPreset &data)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, false);

  String key = String(presetNameMap[preset].name);
  size_t written = preferences.putBytes(key.c_str(), &data, sizeof(CombinedPreset));

  preferences.end();

  return (written == sizeof(CombinedPreset));
}

/**
 * 統合プリセット読み込み（FootLight + HeartLight）
 */
bool NvStorage::loadCombinedPreset(int preset, CombinedPreset &data)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, true);

  String key = String(presetNameMap[preset].name);
  size_t dataSize = preferences.getBytesLength(key.c_str());
  if (dataSize != sizeof(CombinedPreset))
  {
    preferences.end();
    return false;
  }

  size_t read = preferences.getBytes(key.c_str(), &data, sizeof(CombinedPreset));
  preferences.end();

  return (read == sizeof(CombinedPreset));
}

/**
 * 現在のプリセット設定
 */
bool NvStorage::setCurrentPreset(int preset)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, false);
  size_t written = preferences.putUChar(CURRENT_PRESET_KEY, (uint8_t)preset);
  preferences.end();

  if (written > 0)
  {
    return true;
  }

  return false;
}

/**
 * 現在のプリセット取得
 */
int NvStorage::getCurrentPreset()
{
  preferences.begin(NAMESPACE, true);                                       // 読み取り専用
  uint8_t current = preferences.getUChar(CURRENT_PRESET_KEY, PRESET_COUNT); // デフォルトは無効値
  preferences.end();

  if (current < PRESET_COUNT)
  {
    return (int)current;
  }

  return (int)PRESET_COUNT; // 無効値を返す
}

/**
 * プリセット名取得
 */
const char *NvStorage::getPresetName(int preset)
{
  if (preset >= 0 && preset < (int)presetNameMapSize)
  {
    return presetNameMap[preset].name;
  }
  return "Unknown";
}

/**
 * 全設定クリア
 */
void NvStorage::clearAll()
{
  preferences.begin(NAMESPACE, false);
  preferences.clear();
  preferences.end();
}

/**
 * 空き容量取得
 */
size_t NvStorage::getFreeSpace()
{
  preferences.begin(NAMESPACE, true);
  size_t freeSpace = preferences.freeEntries();
  preferences.end();

  return freeSpace;
}
