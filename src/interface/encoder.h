#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <ESP32Encoder.h>

class Encoder
{
private:
  ESP32Encoder esp32Encoder;

public:
  Encoder();
  volatile bool encoderEnabled;
  int currentEncoderValue;
  int oldEncoderValue;
  void setCount(int value);
  boolean updateEncoder(int min, int max);

  // 汎用エンコーダー処理関数
  int runEncoder(
      int initialValue,
      int minValue,
      int maxValue,
      void (*onValueChange)(int value),
      bool (*isBreak)());
};

#endif