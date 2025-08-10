#ifndef FOOTLIGHTMODESETTING_H
#define FOOTLIGHTMODESETTING_H

#include "../BaseSetting.h"

class FootLightModeSetting : public BaseSetting
{
public:
  FootLightModeSetting();

  // BaseSettingの純粋仮想関数をオーバーライド
  void start() override;
  void update() override;
  void cleanup() override;
};

#endif
