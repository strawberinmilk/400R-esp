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
  footLight.init();

  display.print("SKYLINE RV37", "400R Welcome!");

  delay(1000);
}

void loop()
{
  settingManager.update();
}
