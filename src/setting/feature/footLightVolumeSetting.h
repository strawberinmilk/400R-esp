#ifndef FOOTLIGHT_VOLUME_SETTING_H
#define FOOTLIGHT_VOLUME_SETTING_H

#include "../BaseSetting.h"

class FootLightVolumeSetting : public BaseSetting
{
public:
  FootLightVolumeSetting();
  void start() override;
  void update() override;
  void cleanup() override;
};

#endif
