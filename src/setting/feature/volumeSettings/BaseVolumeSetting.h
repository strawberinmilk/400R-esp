#ifndef BASE_VOLUME_SETTING_H
#define BASE_VOLUME_SETTING_H

#include "../../BaseSetting.h"
#include "output/PwmLed.h"

class BaseVolumeSetting : public BaseSetting
{
protected:
  PwmLed *targetLed;
  const char *ledName;

public:
  BaseVolumeSetting(PwmLed *led, const char *name);
  void start() override;
  void update() override;
  void cleanup() override;
};

#endif
