#ifndef MODE_SELECT_H
#define MODE_SELECT_H

#include "../BaseSetting.h"

class ModeSelect : public BaseSetting
{
private:

public:
  ModeSelect();
  void start() override;
  void update() override;
  void cleanup() override;

  // 独自関数
  void startFeature(BaseSetting& newFeature);
};

#endif
