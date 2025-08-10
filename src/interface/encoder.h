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

  // 非ブロッキング版エンコーダー処理関数
private:
  bool isRunningV2 = false;
  int minValueV2 = 0;
  int maxValueV2 = 100;

public:
  void startEncoderV2(int initialValue, int minValue, int maxValue);
  bool updateEncoderV2(); // 非ブロッキング更新、値が変化した場合trueを返す
  void stopEncoderV2();
  bool isEncoderRunningV2() const { return isRunningV2; }
};

#endif