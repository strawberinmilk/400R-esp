#include "nvStorage.h"
extern const int PRESET_COUNT;
#include <ArduinoJson.h>
#include "bleManager.h"
#include "nvStorage.h"
#include "output/PwmLeds/footLight.h"
#include "output/PwmLeds/heartLight.h"
#include "interface/nvStorage.h"

// 外部参照
extern FootLight footLight;
extern HeartLight heartLight;
extern NvStorage nvStorage;

/**
 * コンストラクタ
 */
BLEManager::BLEManager()
{
  deviceConnected = false;
  pServer = nullptr;
  pService = nullptr;
  pCharacteristic = nullptr;
}

/**
 * BLE初期化
 */
void BLEManager::init()
{
  Serial.println("Initializing BLE...");

  // BLEデバイス初期化
  BLEDevice::init(BLE_DEVICE_NAME);

  // BLEサーバー作成
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new CustomBLEServerCallbacks(this));

  // BLEサービス作成
  pService = pServer->createService(BLE_SERVICE_UUID);

  // BLE特性作成
  pCharacteristic = pService->createCharacteristic(
      BLE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->setCallbacks(new CustomBLECharacteristicCallbacks(this));

  // Notification用のDescriptor追加
  pCharacteristic->addDescriptor(new BLE2902());

  // サービス開始
  pService->start();

  // アドバタイジング開始
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0006);
  pAdvertising->setMinPreferred(0x0012);
  BLEDevice::startAdvertising();

  Serial.println("BLE server started, waiting for client connection...");
}

/**
 * 更新処理
 */
// void BLEManager::update()
// {
//   // 現在はポーリング不要
//   // 必要に応じて定期的な状態送信などを実装
// }

/**
 * 接続状態設定
 */
void BLEManager::setConnected(bool connected)
{
  deviceConnected = connected;
}

/**
 * 接続状態取得
 */
bool BLEManager::isConnected()
{
  return deviceConnected;
}

/**
 * 状態変更通知
 */
void BLEManager::notifyStatusChange()
{
  if (deviceConnected)
  {
    sendStatusUpdate();
  }
}

/**
 * コマンドタイプ解析（マッピングテーブル方式）
 */
struct CommandTypeMapEntry
{
  const char *modeName;
  int modeValue;
  BLECommandType type;
};

static const CommandTypeMapEntry commandTypeMap[] = {
    {"footLightVol", 0, CMD_FOOT_LIGHT_VOL},
    {"footLightMode", 1, CMD_FOOT_LIGHT_MODE},
    {"presetLoad", 2, CMD_PRESET_LOAD},
    {"presetSave", 3, CMD_PRESET_SAVE},
    {"getStatus", 4, CMD_GET_STATUS},
    {"heartLightVol", 5, CMD_HEART_LIGHT_VOL},
    {"heartLightMode", 6, CMD_HEART_LIGHT_MODE}};

BLECommandType BLEManager::parseCommandType(const String &mode)
{
  for (const auto &entry : commandTypeMap)
  {
    if (mode == entry.modeName || mode == String(entry.modeValue))
    {
      return entry.type;
    }
  }
  return CMD_UNKNOWN;
}

/**
 * JSONコマンド処理
 */
void BLEManager::processCommand(const String &jsonString)
{
  Serial.printf("Received BLE command: %s\n", jsonString.c_str());

  // ArduinoJsonライブラリによる解析
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  if (error)
  {
    sendResponse("error", "JSON parse error", false);
    return;
  }

  String mode = doc["mode"] | "";
  int value = doc["value"] | 0;
  String presetName = doc["presetName"] | "";

  // コマンド実行
  BLECommandType cmdType = parseCommandType(mode);

  switch (cmdType)
  {
  case CMD_FOOT_LIGHT_VOL:
    handleFootLightVol(value);
    break;
  case CMD_FOOT_LIGHT_MODE:
    handleFootLightMode(value);
    break;
  case CMD_PRESET_LOAD:
    handlePresetLoad(presetName);
    break;
  case CMD_PRESET_SAVE:
    handlePresetSave(presetName);
    break;
  case CMD_GET_STATUS:
    handleGetStatus();
    break;
  case CMD_HEART_LIGHT_VOL:
    handleHeartLightVol(value);
    break;
  case CMD_HEART_LIGHT_MODE:
    handleHeartLightMode(value);
    break;
  default:
    sendResponse("error", "Unknown command: " + mode, false);
    break;
  }
}

/**
 * フットライト明度変更
 */
void BLEManager::handleFootLightVol(int value)
{
  if (value < 0 || value > 255)
  {
    sendResponse("error", "Volume value out of range (0-255)", false);
    return;
  }

  footLight.setVolume(value);
  StaticJsonDocument<64> doc;
  doc["exitCode"] = 0;
  doc["message"] = "success vol:" + String(value);
  String json;
  serializeJson(doc, json);
  pCharacteristic->setValue(json.c_str());
  pCharacteristic->notify();

  Serial.printf("FootLight volume set to: %d\n", value);
}

/**
 * フットライトモード変更
 */
void BLEManager::handleFootLightMode(int value)
{
  if (value < 0 || value >= FootLight::getModeCount())
  {
    sendResponse("error", "Mode value out of range", false);
    return;
  }

  footLight.setMode((FootLightMode)value);
  StaticJsonDocument<64> doc;
  doc["exitCode"] = 0;
  doc["message"] = String("success mode:") + FootLightModeTexts[value];
  String json;
  serializeJson(doc, json);
  pCharacteristic->setValue(json.c_str());
  pCharacteristic->notify();

  Serial.printf("FootLight mode set to: %s\n", FootLightModeTexts[value]);
}

/**
 * ハートライト明度変更
 */
void BLEManager::handleHeartLightVol(int value)
{
  if (value < 0 || value > 255)
  {
    sendResponse("error", "Volume value out of range (0-255)", false);
    return;
  }

  heartLight.setVolume(value);
  StaticJsonDocument<64> doc;
  doc["exitCode"] = 0;
  doc["message"] = "success vol:" + String(value);
  String json;
  serializeJson(doc, json);
  pCharacteristic->setValue(json.c_str());
  pCharacteristic->notify();

  Serial.printf("HeartLight volume set to: %d\n", value);
}

/**
 * ハートライトモード変更
 */
void BLEManager::handleHeartLightMode(int value)
{
  if (value < 0 || value >= HeartLight::getModeCount())
  {
    sendResponse("error", "Mode value out of range", false);
    return;
  }

  heartLight.setMode((HeartLightMode)value);
  StaticJsonDocument<64> doc;
  doc["exitCode"] = 0;
  doc["message"] = String("success mode:") + HeartLightModeTexts[value];
  String json;
  serializeJson(doc, json);
  pCharacteristic->setValue(json.c_str());
  pCharacteristic->notify();

  Serial.printf("HeartLight mode set to: %s\n", HeartLightModeTexts[value]);
}

/**
 * プリセット読み込み
 */
void BLEManager::handlePresetLoad(const String &presetName)
{
  // プリセット名からEnum値に変換
  int preset = NvStorage::presetNameFromString(presetName);

  if (preset == PRESET_COUNT)
  {
    sendResponse("error", "Unknown preset: " + presetName, false);
    return;
  }

  CombinedPreset data;
  if (nvStorage.loadCombinedPreset(preset, data))
  {
    footLight.setVolume(data.footLight.volume);
    footLight.setMode(data.footLight.mode);
    heartLight.setVolume(data.heartLight.volume);
    heartLight.setMode(data.heartLight.mode);
    nvStorage.setCurrentPreset(preset);

    sendResponse("success", "Preset loaded: " + presetName);
    sendStatusUpdate();
    Serial.printf("Preset loaded: %s\n", presetName.c_str());
  }
  else
  {
    sendResponse("error", "Failed to load preset: " + presetName, false);
  }
}

/**
 * プリセット保存
 */
void BLEManager::handlePresetSave(const String &presetName)
{
  // プリセット名からEnum値に変換
  int preset = NvStorage::presetNameFromString(presetName);
  if (preset == PRESET_COUNT)
  {
    sendResponse("error", "Unknown preset: " + presetName, false);
    return;
  }

  CombinedPreset data;
  data.footLight.volume = footLight.getVolume();
  data.footLight.mode = footLight.getMode();
  data.heartLight.volume = heartLight.getVolume();
  data.heartLight.mode = heartLight.getMode();

  if (nvStorage.saveCombinedPreset(preset, data))
  {
    nvStorage.setCurrentPreset(preset);
    sendResponse("success", "Preset saved: " + presetName);
    sendStatusUpdate();
    Serial.printf("Preset saved: %s\n", presetName.c_str());
  }
  else
  {
    sendResponse("error", "Failed to save preset: " + presetName, false);
  }
}

/**
 * 現在の状態取得
 */
void BLEManager::handleGetStatus()
{
  sendStatusUpdate();
}

/**
 * レスポンス送信
 */
void BLEManager::sendResponse(const String &type, const String &message, bool success)
{
  StaticJsonDocument<96> doc;
  int exitCode = (type == "success") ? 0 : 1;
  doc["exitCode"] = exitCode;
  doc["message"] = message;

  String response;
  serializeJson(doc, response);

  if (deviceConnected && pCharacteristic)
  {
    pCharacteristic->setValue(response.c_str());
    pCharacteristic->notify();
  }

  Serial.printf("BLE Response: %s\n", response.c_str());
}

/**
 * 状態更新通知
 */
void BLEManager::sendStatusUpdate()
{
  int currentPreset = nvStorage.getCurrentPreset();
  CombinedPreset presetData;
  bool isMatched = false;

  if (nvStorage.loadCombinedPreset(currentPreset, presetData))
  {
    bool footMatched = (presetData.footLight.volume == footLight.getVolume() &&
                        presetData.footLight.mode == footLight.getMode());
    bool heartMatched = (presetData.heartLight.volume == heartLight.getVolume() &&
                         presetData.heartLight.mode == heartLight.getMode());
    isMatched = footMatched && heartMatched;
  }

  StaticJsonDocument<512> doc;
  doc["type"] = "status";
  JsonObject footLightObj = doc.createNestedObject("footLight");
  footLightObj["volume"] = footLight.getVolume();
  footLightObj["mode"] = (int)footLight.getMode();
  footLightObj["isLighting"] = (footLight.getMode() != MODE_OFF);

  JsonObject heartLightObj = doc.createNestedObject("heartLight");
  heartLightObj["volume"] = heartLight.getVolume();
  heartLightObj["mode"] = (int)heartLight.getMode();
  heartLightObj["isLighting"] = (heartLight.getMode() != MODE_OFF);

  JsonObject presetObj = doc.createNestedObject("preset");
  presetObj["current"] = NvStorage::getPresetName(currentPreset);
  presetObj["isMatched"] = isMatched;

  // --- constants ---
  JsonObject constants = doc.createNestedObject("constants");
  JsonObject flConst = constants.createNestedObject("footLight");
  flConst["min"] = 0;
  flConst["max"] = 255;

  // モード値リスト
  JsonArray modeValArr = flConst.createNestedArray("modeVal");
  JsonArray modeNameArr = flConst.createNestedArray("modeName");
  for (int i = 0; i < FOOTLIGHT_MODE_COUNT; ++i)
  {
    modeValArr.add(i);
    modeNameArr.add(FootLightModeTexts[i]);
  }

  JsonObject hlConst = constants.createNestedObject("heartLight");
  hlConst["min"] = 0;
  hlConst["max"] = 255;

  // モード値リスト
  JsonArray hlModeValArr = hlConst.createNestedArray("modeVal");
  JsonArray hlModeNameArr = hlConst.createNestedArray("modeName");
  for (int i = 0; i < HEARTLIGHT_MODE_COUNT; ++i)
  {
    hlModeValArr.add(i);
    hlModeNameArr.add(HeartLightModeTexts[i]);
  }

  // プリセットリスト
  JsonObject presetConst = constants.createNestedObject("preset");
  JsonArray presetNameList = presetConst.createNestedArray("presetNameList");
  JsonArray presetValueList = presetConst.createNestedArray("presetValueList");
  for (int i = 0; i < PRESET_COUNT; ++i)
  {
    presetNameList.add(NvStorage::getPresetName(i));
    presetValueList.add(i);
  }

  String status;
  serializeJson(doc, status);

  if (deviceConnected && pCharacteristic)
  {
    pCharacteristic->setValue(status.c_str());
    pCharacteristic->notify();
  }

  Serial.printf("Status update: %s\n", status.c_str());
}

// ========== コールバック実装 ==========

/**
 * BLEサーバー接続コールバック
 */
void CustomBLEServerCallbacks::onConnect(BLEServer *pServer)
{
  bleManager->setConnected(true);
  Serial.println("BLE Client connected");
}

/**
 * BLEサーバー切断コールバック
 */
void CustomBLEServerCallbacks::onDisconnect(BLEServer *pServer)
{
  bleManager->setConnected(false);
  Serial.println("BLE Client disconnected");
  // 再アドバタイジング開始
  BLEDevice::startAdvertising();
}

/**
 * BLEにアクセスがあった際のコールバック
 */
void CustomBLECharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  std::string value = pCharacteristic->getValue();

  if (0 < value.length())
  {
    String command = String(value.c_str());
    bleManager->processCommand(command);
  }
}
