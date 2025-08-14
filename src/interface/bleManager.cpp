#include "bleManager.h"
#include "output/footLight.h"
#include "interface/nvStorage.h"

// 外部参照
extern FootLight footLight;
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
void BLEManager::update()
{
  // 現在はポーリング不要
  // 必要に応じて定期的な状態送信などを実装
}

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
 * コマンドタイプ解析
 */
BLECommandType BLEManager::parseCommandType(const String &mode)
{
  if (mode == "footLightVol" || mode == "0")
    return CMD_FOOT_LIGHT_VOL;
  if (mode == "footLightMode" || mode == "1")
    return CMD_FOOT_LIGHT_MODE;
  if (mode == "presetLoad" || mode == "2")
    return CMD_PRESET_LOAD;
  if (mode == "presetSave" || mode == "3")
    return CMD_PRESET_SAVE;
  if (mode == "getStatus" || mode == "4")
    return CMD_GET_STATUS;
  return CMD_UNKNOWN;
}

/**
 * JSONコマンド処理
 */
void BLEManager::processCommand(const String &jsonString)
{
  Serial.printf("Received BLE command: %s\n", jsonString.c_str());

  // 簡易JSON解析（ArduinoJsonライブラリ無しで実装）
  // 例: {"mode":"footLightVol","value":128}

  int modeStart = jsonString.indexOf("\"mode\":\"") + 8;
  int modeEnd = jsonString.indexOf("\"", modeStart);
  String mode = jsonString.substring(modeStart, modeEnd);

  int valueStart = jsonString.indexOf("\"value\":") + 8;
  int valueEnd = jsonString.indexOf("}", valueStart);
  if (valueEnd == -1)
    valueEnd = jsonString.indexOf(",", valueStart);
  String valueStr = jsonString.substring(valueStart, valueEnd);
  int value = valueStr.toInt();

  // presetName があるか確認
  String presetName = "";
  int presetStart = jsonString.indexOf("\"presetName\":\"");
  if (presetStart != -1)
  {
    presetStart += 14;
    int presetEnd = jsonString.indexOf("\"", presetStart);
    presetName = jsonString.substring(presetStart, presetEnd);
  }

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
  sendResponse("success", "Volume set to " + String(value));
  sendStatusUpdate();

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
  sendResponse("success", "Mode set to " + String(value));
  sendStatusUpdate();

  Serial.printf("FootLight mode set to: %d\n", value);
}

/**
 * プリセット読み込み
 */
void BLEManager::handlePresetLoad(const String &presetName)
{
  // プリセット名からEnum値に変換
  NvStorage::PresetName preset = NvStorage::PRESET_RIN; // デフォルト

  if (presetName == "rin")
    preset = NvStorage::PRESET_RIN;
  else if (presetName == "IonGrid")
    preset = NvStorage::PRESET_IONGRID;
  else if (presetName == "fuji")
    preset = NvStorage::PRESET_FUJI;
  else if (presetName == "masutate")
    preset = NvStorage::PRESET_MASUTATE;
  else if (presetName == "custom1")
    preset = NvStorage::PRESET_CUSTOM1;
  else if (presetName == "custom2")
    preset = NvStorage::PRESET_CUSTOM2;
  else
  {
    sendResponse("error", "Unknown preset: " + presetName, false);
    return;
  }

  FootLightPreset data;
  if (nvStorage.loadPreset(preset, data))
  {
    footLight.setVolume(data.volume);
    footLight.setMode(data.mode);
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
  NvStorage::PresetName preset = NvStorage::PRESET_CUSTOM1; // デフォルト

  if (presetName == "rin")
    preset = NvStorage::PRESET_RIN;
  else if (presetName == "IonGrid")
    preset = NvStorage::PRESET_IONGRID;
  else if (presetName == "fuji")
    preset = NvStorage::PRESET_FUJI;
  else if (presetName == "masutate")
    preset = NvStorage::PRESET_MASUTATE;
  else if (presetName == "custom1")
    preset = NvStorage::PRESET_CUSTOM1;
  else if (presetName == "custom2")
    preset = NvStorage::PRESET_CUSTOM2;
  else
  {
    sendResponse("error", "Unknown preset: " + presetName, false);
    return;
  }

  FootLightPreset data;
  data.volume = footLight.getVolume();
  data.mode = footLight.getMode();

  if (nvStorage.savePreset(preset, data))
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
  String response = "{\"type\":\"" + type + "\",\"message\":\"" + message + "\",\"success\":" + (success ? "true" : "false") + "}";

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
  NvStorage::PresetName currentPreset = nvStorage.getCurrentPreset();
  FootLightPreset presetData;
  bool isMatched = false;

  if (nvStorage.loadPreset(currentPreset, presetData))
  {
    isMatched = (presetData.volume == footLight.getVolume() && presetData.mode == footLight.getMode());
  }

  String status = "{\"type\":\"status\",";
  status += "\"footLight\":{";
  status += "\"volume\":" + String(footLight.getVolume()) + ",";
  status += "\"mode\":" + String((int)footLight.getMode()) + ",";
  status += "\"isLighting\":" + String(footLight.getMode() != MODE_OFF ? "true" : "false");
  status += "},";
  status += "\"preset\":{";
  status += "\"current\":\"" + String(NvStorage::getPresetName(currentPreset)) + "\",";
  status += "\"isMatched\":" + String(isMatched ? "true" : "false");
  status += "}}";

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
 * BLE特性書き込みコールバック
 */
void CustomBLECharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  std::string value = pCharacteristic->getValue();

  if (value.length() > 0)
  {
    String command = String(value.c_str());
    bleManager->processCommand(command);
  }
}
