#ifndef FOOTLIGHT_H
#define FOOTLIGHT_H

#include <Arduino.h>

class FootLight {
  private:
    int VOLUME;
    boolean IS_LIGHTING;
    void lighting();

  public:
    FootLight();
    void init();
    void setVolume(int volume);
    void setIsLighting(boolean isLighting);
    int getVolume();
    boolean isLighting();
};

#endif