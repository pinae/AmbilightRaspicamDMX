#ifndef AMBILIGHT_H
#define AMBILIGHT_H
#include "camera.h"


class Ambilight
{
public:
  Ambilight();
  ~Ambilight();

protected:
  void loop();

private:
  bool run;
  CCamera* cam;
};

#endif // AMBILIGHT_H
