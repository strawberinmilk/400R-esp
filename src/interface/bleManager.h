#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE UUIDs
#define BLE_SERVICE_UUID "7eda57ff-6226-4f8c-ba8f-02f632e88609"
#define BLE_CHARACTERISTIC_UUID "b661a0f3-a25c-47fa-96a2-17abcb9655df"
#define BLE_DEVICE_NAME "400R-controller"

// コマンドタイプ
enum BLECommandType
{
  CMD_FOOT_LIGHT_VOL = 0,
  CMD_FOOT_LIGHT_MODE = 1,
  CMD_PRESET_LOAD = 2,
  CMD_PRESET_SAVE = 3,
  CMD_GET_STATUS = 4,
  CMD_UNKNOWN = 99
};

// 前方宣言
class BLEManager;

class BLEManager
{
private:
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristic;
  bool deviceConnected;

public:
  BLEManager();
  void init();
  // void update();
  void processCommand(const String &command);
  void setConnected(bool connected);
  bool isConnected();
  void notifyStatusChange();

private:
  // コマンド処理
  BLECommandType parseCommandType(const String &mode);
  void handleFootLightVol(int value);
  void handleFootLightMode(int value);
  void handlePresetLoad(const String &presetName);
  void handlePresetSave(const String &presetName);
  void handleGetStatus();

  // レスポンス送信
  void sendResponse(const String &status, const String &message, bool sendUpdate = true);
  void sendStatusUpdate();
};

// BLEサーバーコールバック
class CustomBLEServerCallbacks : public BLEServerCallbacks
{
  BLEManager *bleManager;

public:
  CustomBLEServerCallbacks(BLEManager *manager) : bleManager(manager) {}
  void onConnect(BLEServer *pServer);
  void onDisconnect(BLEServer *pServer);
};

// BLE特性コールバック
class CustomBLECharacteristicCallbacks : public BLECharacteristicCallbacks
{
  BLEManager *bleManager;

public:
  CustomBLECharacteristicCallbacks(BLEManager *manager) : bleManager(manager) {}
  void onWrite(BLECharacteristic *pCharacteristic);
};

#endif
