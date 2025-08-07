#ifndef SETMODE_H
#define SETMODE_H

#include <Arduino.h>

class SetMode
{
// private:

public:
  void init();
  void select();

  void footLightVolume();
  void footLightMode();
};

#endif