#include "StandbyMode.h"
#include "interface/display.h"
#include "interface/button.h"
#include "config/pinConfig.h"
#include "setting/manager.h"

// mainで定義
extern Display display;
extern Button button;
extern SettingManager settingManager;

/**
 * コンストラクタ
 */
StandbyMode::StandbyMode()
{
  name = "Standby";
  isRunning = false;
}

/**
 * 開始処理
 */
void StandbyMode::start()
{
  display.print("SKYLINE 400R", "standby mode");
}

/**
 * 更新処理
 */
void StandbyMode::update()
{
  if (isRunning == false)
  {
    start();
    isRunning = true;
  }
  // SELECT_SW_PINが押されたらモード選択を開始
  if (button.isPushAwait(SELECT_SW_PIN))
  {
    this->cleanup();
  }
}

/**
 * クリーンアップ処理
 */
void StandbyMode::cleanup()
{
  settingManager.currentFeature = nullptr;
  isRunning = false;
}
