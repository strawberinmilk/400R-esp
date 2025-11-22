#include "BaseSetting.h"

class SettingManager
{
private:
public:
  SettingManager();

  void update();
  void saveSettings();

  static constexpr int MODE_COUNT = 7;
  const char *modeNames[MODE_COUNT] = {
      "Mode Select",
      "Foot Light Vol",
      "Foot Light Mode",
      "Heart Light Vol",
      "Heart Light Mode",
      "Preset Manager",
      "Standby"};
  int selectedMode;
  bool modeSelected;
  BaseSetting *currentFeature;
};