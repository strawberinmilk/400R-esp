#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <ESP32Encoder.h>

class Encoder
{
private:
  ESP32Encoder esp32Encoder;
  bool isRunning = false;
  int minValue = 0;
  int maxValue = 100;
  int currentEncoderValue = 0;
  int oldEncoderValue = 0;

  void setCount(int value);
  boolean updateEncoder(int min, int max);

public:
  Encoder();
  int getCurrentValue();

  // 非ブロッキング版エンコーダー処理関数
  void startEncoder(int initialValue, int minValue, int maxValue);
  bool isUpdateEncoder();
  void stopEncoder();
};

#endif