#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>

class Display {
  private:
    void writeCommand(byte t_command);
    void writeData(byte t_data);

  public:
    void initLcd();
    void print(const char line1[], const char line2[]);
};

#endif