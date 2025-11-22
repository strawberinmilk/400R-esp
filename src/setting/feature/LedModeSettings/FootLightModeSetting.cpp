#include "FootLightModeSetting.h"
#include "output/PwmLeds/footLight.h"

extern FootLight footLight;

/**
 * コンストラクタ
 */
FootLightModeSetting::FootLightModeSetting()
    : BaseLedModeSetting(&footLight, "Foot Light")
{
}
