#ifndef FOOTLIGHT_MODE_SETTING_H
#define FOOTLIGHT_MODE_SETTING_H

#include "../BaseSetting.h"

class FootLightModeSetting : public BaseSetting
{
public:
  FootLightModeSetting();
  void start() override;
  void update() override;
  void cleanup() override;
};

#endif
