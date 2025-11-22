#include "FootLightVolumeSetting.h"
#include "output/PwmLeds/footLight.h"

extern FootLight footLight;

/**
 * コンストラクタ
 */
FootLightVolumeSetting::FootLightVolumeSetting()
    : BaseVolumeSetting(&footLight, "Foot Light")
{
}
