#ifndef FOOTLIGHTVOLUMESETTING_H
#define FOOTLIGHTVOLUMESETTING_H

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