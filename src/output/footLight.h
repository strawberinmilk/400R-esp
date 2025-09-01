#ifndef FOOTLIGHT_H
#define FOOTLIGHT_H

#include <Arduino.h>

enum FootLightMode
{
  MODE_OFF = 0,
  MODE_ON = 1,
  MODE_AUTO_SIDE_BRAKE = 2,
  MODE_AUTO_ILLUMI = 3
};

// FootLightModeの要素数定数
static const int FOOTLIGHT_MODE_COUNT = 4;

// FootLightModeに対応するテキスト配列
static const char *const FootLightModeTexts[] = {
    "OFF",             // MODE_OFF = 0
    "ON",              // MODE_ON = 1
    "Auto Side Brake", // MODE_AUTO_SIDE_BRAKE = 2
    "Auto Illumi"      // MODE_AUTO_ILLUMI = 3
};

class FootLight
{
private:
  int VOLUME;
  FootLightMode currentMode;

public:
  FootLight();
  void init();
  void setVolume(int volume);
  void setMode(FootLightMode mode);
  int getVolume();
  FootLightMode getMode();
  void lighting();
  static int getModeCount();
  static const char *getModeText(FootLightMode mode);
};

#endif