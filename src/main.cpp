#include <Arduino.h>
#include <Wire.h>
// config
#include "config/pinConfig.h"
// interface
#include "interface/encoder.h"
#include "interface/display.h"
#include "interface/button.h"
// output
#include "output/footLight.h"
// feature
#include "setting/manager.h"

// interface
Encoder encoder;
Display display;
Button button;
// output
FootLight footLight;
// feature
SettingManager settingManager;

int buttonPins[] = {SELECT_SW_PIN};

void setup()
{
  Serial.begin(115200);

  display.init();
  button.init(buttonPins);

  // setMode.init();
  footLight.init();

  display.print("SKYLINE RV37", "400R Welcome!");
  delay(1000);
  display.print("SKYLINE 400R", "standby mode");

  // Serial.print("Checking I2C device at 0x");
  // Serial.print(LCD_ADDRESS, HEX);
  // Serial.print(": ");

  // Wire.beginTransmission(LCD_ADDRESS);
  // byte error = Wire.endTransmission();

  // if (error == 0)
  // {
  //   Serial.println("Device found!");
  // }
  // else
  // {
  //   Serial.print("Error: ");
  //   Serial.println(error); // 0: OK, 2: NACK on address, etc.
  // }

  delay(1000);
}

void loop()
{
  settingManager.update();
}
