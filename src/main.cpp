#include <Arduino.h>
#include <Wire.h>
// config
#include "config/pinConfig.h"
// interface
#include "interface/encoder.h"
#include "interface/display.h"
#include "interface/button.h"
#include "interface/nvStorage.h"
#include "interface/bleManager.h"
// carInput
#include "interface/carInput/illumi.h"
#include "interface/carInput/handBrake.h"
// output
#include "output/PwmLeds/footLight.h"
#include "output/PwmLeds/heartLight.h"
// feature
#include "setting/manager.h"

// interface
Encoder encoder;
Display display;
Button button;
NvStorage nvStorage;
BLEManager bleManager;
// carInput
Illumi illumi;
HandBrake handBrake;
// output
FootLight footLight;
HeartLight heartLight;
// feature
SettingManager settingManager;

int buttonPins[] = {SELECT_SW_PIN};

void setup()
{
  Serial.begin(115200);

  nvStorage.init();
  display.init();
  button.init(buttonPins);
  footLight.init();
  heartLight.init();
  bleManager.init();
  illumi.init();
  handBrake.init();

  display.print("SKYLINE RV37", "400R Welcome!");
  delay(1000);
}

void loop()
{
  settingManager.update();
  illumi.update();
  handBrake.update();
}
