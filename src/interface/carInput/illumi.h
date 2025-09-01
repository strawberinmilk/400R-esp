
#ifndef ILLUMI_H
#define ILLUMI_H

#include <Arduino.h>

class Illumi
{
private:
  volatile bool illumiState;
  // static void isrHandler();
  // static Illumi *instance;

public:
  void init();
  void update();
  bool getIllumiState() const;
};

extern Illumi illumi;

#endif
