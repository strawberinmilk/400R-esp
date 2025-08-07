#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>

class Display {
  private:

  public:
    void init();
    void print(const char line1[], const char line2[]);
    void printWithPosition(int x, int y, const char text[]);
};

#endif