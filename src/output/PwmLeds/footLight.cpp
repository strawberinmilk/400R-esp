#include "footLight.h"
#include "config/pinConfig.h"

FootLight::FootLight()
    : PwmLed(FOOTLIGHT_PIN, PWM_CH)
{
}
