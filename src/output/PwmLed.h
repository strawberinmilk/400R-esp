#ifndef PWMLED_H
#define PWMLED_H

#include <Arduino.h>

enum LedMode
{
  MODE_OFF = 0,
  MODE_ON = 1,
  MODE_AUTO_SIDE_BRAKE = 2,
  MODE_AUTO_ILLUMI = 3
};

// LedModeの要素数定数
static const int LED_MODE_COUNT = 4;

// LedModeに対応するテキスト配列
static const char *const LedModeTexts[] = {
    "OFF",             // MODE_OFF = 0
    "ON",              // MODE_ON = 1
    "Auto Side Brake", // MODE_AUTO_SIDE_BRAKE = 2
    "Auto Illumi"      // MODE_AUTO_ILLUMI = 3
};

class PwmLed
{
protected:
  int VOLUME;
  LedMode currentMode;
  int pin;
  int pwmChannel;

  // 派生クラスでオーバーライド可能な仮想関数
  virtual void applyLighting();

public:
  PwmLed(int pin, int pwmChannel);
  virtual ~PwmLed() = default;

  void init();
  void setVolume(int volume);
  void setMode(LedMode mode);
  int getVolume() const;
  LedMode getMode() const;
  void lighting();
  static int getModeCount();
  static const char *getModeText(LedMode mode);
};

#endif
