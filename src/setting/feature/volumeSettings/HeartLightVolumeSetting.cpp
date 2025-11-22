#include "HeartLightVolumeSetting.h"
#include "output/PwmLeds/heartLight.h"

extern HeartLight heartLight;

/**
 * コンストラクタ
 */
HeartLightVolumeSetting::HeartLightVolumeSetting()
    : BaseVolumeSetting(&heartLight, "Heart Light")
{
}
