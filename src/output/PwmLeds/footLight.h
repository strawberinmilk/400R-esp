#ifndef FOOTLIGHT_H
#define FOOTLIGHT_H

#include "../PwmLed.h"

// 互換性のためのエイリアス
typedef LedMode FootLightMode;
static const int FOOTLIGHT_MODE_COUNT = LED_MODE_COUNT;
static const char *const *FootLightModeTexts = LedModeTexts;

class FootLight : public PwmLed
{
public:
  FootLight();
};

#endif