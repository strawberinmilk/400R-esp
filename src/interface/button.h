#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
public:
  Button();
  template <size_t N>
  void init(int (&pinNumbers)[N])
  {
    for (size_t i = 0; i < N; i++)
    {
      pinMode(pinNumbers[i], INPUT_PULLUP);
    }
  }
  bool isPush(int pinNumber);
  unsigned long isPushAwait(int pinNumber);
};

#endif
