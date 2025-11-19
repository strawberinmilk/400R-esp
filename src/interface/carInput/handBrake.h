#ifndef HAND_BRAKE_H
#define HAND_BRAKE_H

#include <Arduino.h>

class HandBrake
{
private:
  volatile bool handBrakeState;

public:
  void init();
  void update();
  bool getHandBrakeState() const;
};

extern HandBrake handBrake;

#endif
