#ifndef MODESELECT_H
#define MODESELECT_H

#include "../BaseSetting.h"

class ModeSelect : public BaseSetting
{
private:

public:
  ModeSelect();

  // BaseSettingの純粋仮想関数をオーバーライド
  const char* name = "ModeSelect";
  void start() override;
  void update() override;
  void cleanup() override;

  // 独自関数
  void startFeature(BaseSetting& newFeature);
};

#endif
