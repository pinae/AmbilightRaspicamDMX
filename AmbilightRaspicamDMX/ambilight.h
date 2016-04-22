#ifndef AMBILIGHT_H
#define AMBILIGHT_H
#include <cstdint>
#include "camera.h"
#include "olawrapper.h"


class Ambilight
{
public:
  Ambilight();
  ~Ambilight();
  void step(OlaWrapper &ola);

protected:
  uint32_t getPixel(const void* frame_data, uint32_t x, uint32_t y) const;
  void setRGBChannels(OlaWrapper &ola, uint8_t channelOffset, uint32_t color);

private:
  CCamera* cam;
};

#endif // AMBILIGHT_H
