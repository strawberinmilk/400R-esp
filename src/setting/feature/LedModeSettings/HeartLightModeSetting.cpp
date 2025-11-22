#include "HeartLightModeSetting.h"
#include "output/PwmLeds/heartLight.h"

extern HeartLight heartLight;

/**
 * コンストラクタ
 */
HeartLightModeSetting::HeartLightModeSetting()
    : BaseLedModeSetting(&heartLight, "Heart Light")
{
}
