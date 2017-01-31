#include "ambilight.h"
#include "camera.h"
#include <iostream>
#include <iomanip>

#define CAPTURE_WIDTH 64
#define CAPTURE_HEIGHT 32

Ambilight::Ambilight()
{
  //should the camera convert frame data from yuv to argb automatically?
  bool do_argb_conversion = true;

  //initialize the camera
  cam = StartCamera(CAPTURE_WIDTH,
                    CAPTURE_HEIGHT,
                    30,
                    1,
                    do_argb_conversion);
}

Ambilight::~Ambilight()
{
  StopCamera();
}

void Ambilight::setRGBChannels(OlaWrapper &ola, uint8_t channelOffset, uint32_t color)
{
  uint8_t b = color >> 16;
  uint8_t g = color >> 8;
  uint8_t r = color;
  ola.setChannel(channelOffset, r);
  ola.setChannel(channelOffset+1, g);
  ola.setChannel(channelOffset+2, b);
}

void Ambilight::step(OlaWrapper &ola)
{
  const void* frame_data;
  int frame_sz;
  if (cam->BeginReadFrame(0, frame_data, frame_sz)) {
    setRGBChannels(ola, 1, getPixel(frame_data, 39, 16));
    setRGBChannels(ola, 6, getPixel(frame_data, 19, 16));
    setRGBChannels(ola, 9, getPixel(frame_data, 30, 12));
    ola.send();
    cam->EndReadFrame(0);
  }
}

uint32_t Ambilight::getPixel(const void* frame_data, uint32_t x, uint32_t y) const
{
  return *(reinterpret_cast<const uint32_t*>(frame_data)+y*CAPTURE_WIDTH+x);
}
