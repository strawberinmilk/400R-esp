#include "nvStorage.h"

// 定数定義
const char *NvStorage::NAMESPACE = "footlight";
const char *NvStorage::CURRENT_PRESET_KEY = "current";
const char *NvStorage::PRESET_KEYS[] = {
    "preset_rin",
    "preset_iongrid",
    "preset_fuji",
    "preset_masutate",
    "preset_custom1",
    "preset_custom2"};

/**
 * コンストラクタ
 */
NvStorage::NvStorage()
{
}

/**
 * 初期化処理
 */
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
      if (!loadPreset((PresetName)i, existing))
      {
        savePreset((PresetName)i, defaultPreset);
      }
    }

    // 現在のプリセットをRINに設定
    if (getCurrentPreset() == PRESET_COUNT)
    { // 無効値の場合
      setCurrentPreset(PRESET_RIN);
    }
  }
  preferences.end();

  Serial.println("NvStorage initialized");
}

/**
 * プリセット保存
 */
bool NvStorage::savePreset(PresetName preset, const FootLightPreset &data)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, false);

  // 構造体をバイト配列として保存
  size_t written = preferences.putBytes(PRESET_KEYS[preset], &data, sizeof(FootLightPreset));

  preferences.end();

  if (written == sizeof(FootLightPreset))
  {
    Serial.printf("Preset %s saved (volume: %d, mode: %d)\n",
                  getPresetName(preset), data.volume, data.mode);
    return true;
  }

  Serial.printf("Failed to save preset %s\n", getPresetName(preset));
  return false;
}

/**
 * プリセット読み込み
 */
bool NvStorage::loadPreset(PresetName preset, FootLightPreset &data)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, true); // 読み取り専用

  size_t dataSize = preferences.getBytesLength(PRESET_KEYS[preset]);
  if (dataSize != sizeof(FootLightPreset))
  {
    preferences.end();
    return false;
  }

  size_t read = preferences.getBytes(PRESET_KEYS[preset], &data, sizeof(FootLightPreset));
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
bool NvStorage::setCurrentPreset(PresetName preset)
{
  if (preset >= PRESET_COUNT)
    return false;

  preferences.begin(NAMESPACE, false);
  size_t written = preferences.putUChar(CURRENT_PRESET_KEY, (uint8_t)preset);
  preferences.end();

  if (written > 0)
  {
    Serial.printf("Current preset set to: %s\n", getPresetName(preset));
    return true;
  }

  return false;
}

/**
 * 現在のプリセット取得
 */
NvStorage::PresetName NvStorage::getCurrentPreset()
{
  preferences.begin(NAMESPACE, true);                                       // 読み取り専用
  uint8_t current = preferences.getUChar(CURRENT_PRESET_KEY, PRESET_COUNT); // デフォルトは無効値
  preferences.end();

  if (current < PRESET_COUNT)
  {
    return (PresetName)current;
  }

  return (PresetName)PRESET_COUNT; // 無効値を返す
}

/**
 * プリセット名取得
 */
const char *NvStorage::getPresetName(PresetName preset)
{
  switch (preset)
  {
  case PRESET_RIN:
    return "rin";
  case PRESET_IONGRID:
    return "IonGrid";
  case PRESET_FUJI:
    return "fuji";
  case PRESET_MASUTATE:
    return "masutate";
  case PRESET_CUSTOM1:
    return "custom1";
  case PRESET_CUSTOM2:
    return "custom2";
  default:
    return "Unknown";
  }
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
