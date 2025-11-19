#ifndef HEARTLIGHT_H
#define HEARTLIGHT_H

#include "../PwmLed.h"

// 互換性のためのエイリアス
typedef LedMode HeartLightMode;
static const int HEARTLIGHT_MODE_COUNT = LED_MODE_COUNT;
static const char *const *HeartLightModeTexts = LedModeTexts;

class HeartLight : public PwmLed
{
public:
  HeartLight();
};

#endif
