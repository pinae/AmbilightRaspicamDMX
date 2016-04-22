#ifndef FADER_H
#define FADER_H
#include "olawrapper.h"
#include <vector>
#include <array>
#include <cstdint>

class Fader
{
public:
  Fader();
  void step(OlaWrapper &ola);
  void setMode(uint8_t mode);

protected:
  uint32_t getColorForState(float state);
  void setRGBChannels(OlaWrapper &ola, uint8_t channelOffset, uint32_t color);

private:
  typedef std::vector<uint32_t> ColorList;
  float state;
  uint8_t mode;
  ColorList modes[3];
};

#endif // FADER_H
