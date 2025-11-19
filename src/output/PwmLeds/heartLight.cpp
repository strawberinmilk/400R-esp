#include "heartLight.h"
#include "config/pinConfig.h"

HeartLight::HeartLight()
    : PwmLed(HEARTLIGHT_PIN, HEARTLIGHT_PWM_CH)
{
}