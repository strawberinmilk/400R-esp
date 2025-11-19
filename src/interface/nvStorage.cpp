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
String PRESET_KEYS[presetNameMapSize];

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

// 動的プリセットキー生成
__attribute__((constructor)) static void initPresetKeys()
{
  for (size_t i = 0; i < presetNameMapSize; ++i)
  {
    PRESET_KEYS[i] = "preset_" + String(presetNameMap[i].name);
  }
}

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
    FootLightPreset defaultPreset;
    defaultPreset.volume = 128; // 中間の明るさ
    defaultPreset.mode = MODE_ON;

    // 全プリセットにデフォルト値を設定
    for (int i = 0; i < PRESET_COUNT; i++)
    {
      FootLightPreset existing;
      if (!loadPreset(i, existing))
      {
        savePreset(i, defaultPreset);
      }
    }

    // 現在のプリセットをRINに設定
    if (getCurrentPreset() == PRESET_COUNT)
    { // 無効値の場合
      setCurrentPreset(PRESET_INDEX_RIN);
    }
  }
  preferences.end();

  Serial.println("NvStorage initialized");
}

/**
 * 初期化処理
 */
// フットライトの現在プリセットを起動時に反映
#include "output/PwmLeds/footLight.h"
extern FootLight footLight;

void NvStorage::init()
{
  // デフォルトプリセットの設定（初回起動時）
  if (!preferences.begin(NAMESPACE, true))
  { // 読み取り専用でチェック
    preferences.end();
    preferences.begin(NAMESPACE, false); // 書き込み可能で開く

    // デフォルト値の設定
    FootLightPreset defaultPreset;
    defaultPreset.volume = 128; // 中間の明るさ
    defaultPreset.mode = MODE_ON;

    // 全プリセットにデフォルト値を設定
    for (int i = 0; i < PRESET_COUNT; i++)
    {
      FootLightPreset existing;
      if (!loadPreset(i, existing))
      {
        savePreset(i, defaultPreset);
      }
    }

    // 現在のプリセットをRINに設定
    if (getCurrentPreset() == PRESET_COUNT)
    { // 無効値の場合
      setCurrentPreset(PRESET_INDEX_RIN);
    }
  }
  preferences.end();

  // 起動時にcurrentPresetをfootLightへ反映
  int current = getCurrentPreset();
  FootLightPreset preset;
  if (loadPreset(current, preset))
  {
    footLight.setVolume(preset.volume);
    footLight.setMode(preset.mode);
  }
}

/**
 * プリセット保存
 */
bool NvStorage::savePreset(int preset, const FootLightPreset &data)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, false);

  // 構造体をバイト配列として保存
  size_t written = preferences.putBytes(PRESET_KEYS[preset].c_str(), &data, sizeof(FootLightPreset));

  preferences.end();

  if (written == sizeof(FootLightPreset))
  {
    Serial.printf("Preset %s saved (volume: %d, mode: %d)\n",
                  getPresetName(preset), data.volume, data.mode);
    return true;
  }

  return false;
}

/**
 * プリセット読み込み
 */
bool NvStorage::loadPreset(int preset, FootLightPreset &data)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, true); // 読み取り専用

  size_t dataSize = preferences.getBytesLength(PRESET_KEYS[preset].c_str());
  if (dataSize != sizeof(FootLightPreset))
  {
    preferences.end();
    return false;
  }

  size_t read = preferences.getBytes(PRESET_KEYS[preset].c_str(), &data, sizeof(FootLightPreset));
  preferences.end();

  if (read == sizeof(FootLightPreset))
  {
    Serial.printf("Preset %s loaded (volume: %d, mode: %d)\n",
                  getPresetName(preset), data.volume, data.mode);
    return true;
  }

  return false;
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

  Serial.println("All presets cleared");
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
