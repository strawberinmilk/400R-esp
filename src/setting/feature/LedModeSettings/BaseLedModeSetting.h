#ifndef BASE_LED_MODE_SETTING_H
#define BASE_LED_MODE_SETTING_H

#include "../../BaseSetting.h"
#include "output/PwmLed.h"

class BaseLedModeSetting : public BaseSetting
{
protected:
  PwmLed *targetLed;
  const char *ledName;

public:
  BaseLedModeSetting(PwmLed *led, const char *name);
  void start() override;
  void update() override;
  void cleanup() override;
};

#endif
