#ifndef FADER_H
#define FADER_H
#include "olawrapper.h"
#include <vector>
#include <array>
#include <cstdint>

#define spectrum 0
#define lava 1
#define water 2

class Fader
{
public:
  Fader();
  void step(OlaWrapper* ola);
  void setMode(uint8_t mode);

protected:
  uint32_t getColorForState(float state);

private:
  typedef std::vector<uint32_t> ColorList;
  typedef std::array<ColorList, 3> ModeMap;
  float state;
  uint8_t mode;
  ModeMap modes;
};

#endif // FADER_H
