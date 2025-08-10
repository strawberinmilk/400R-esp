#ifndef STANDBY_MODE_H
#define STANDBY_MODE_H

#include "setting/BaseSetting.h"

class StandbyMode : public BaseSetting
{

private:
  bool isRunning;

public:
  StandbyMode();
  virtual void start() override;
  virtual void update() override;
  virtual void cleanup() override;
};

#endif
