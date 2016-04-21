#include "fader.h"
#include <math.h>

#define RGB(r, g, b) ((r) << 24 | (g) << 16 | (b) << 8)

Fader::Fader()
{
  state = 0;
  mode = spectrum;
  modes = {
    {RGB(0, 255, 0), RGB(0, 128, 128), RGB(0, 0, 255), RGB(128, 0, 128), RGB(255, 0, 0), RGB(128, 128, 0)},
    {RGB(255, 0, 0), RGB(128, 128, 0), RGB(208, 48, 0), RGB(138, 118, 0)},
    {RGB(0, 0, 255), RGB(0, 128, 128), RGB(10, 30, 215), RGB(0, 148, 108)},
  }
}

void Fader::step(OlaWrapper* ola)
{
  uint32_t color = getColorForState(state);
  uint8_t r = color >> 24;
  uint8_t g = color >> 16;
  uint8_t b = color >> 8;
  ola->setChannel(1, r);
  ola->setChannel(2, g);
  ola->setChannel(3, b);
  color = getColorForState(state+0.5);
  r = color >> 24;
  g = color >> 16;
  b = color >> 8;
  ola->setChannel(6, r);
  ola->setChannel(7, g);
  ola->setChannel(8, b);
  color = getColorForState(state+1);
  r = color >> 24;
  g = color >> 16;
  b = color >> 8;
  ola->setChannel(10, r);
  ola->setChannel(11, g);
  ola->setChannel(12, b);
}

void Fader::setMode(uint8_t newMode)
{
  mode = newMode;
  state = 0;
}

uint32_t Fader::getColorForState(float state)
{
  unsigned int lower = (unsigned int)(floor(state)) % modes[mode].size();
  unsigned int upper = (unsigned int)(ceil(state)) % modes[mode].size();
  if(upper == lower) upper = (upper + 1) % modes[mode].size();
  float phase = state - trunc(state);
  uint8_t lower_r = modes[mode][lower] >> 24;
  uint8_t lower_g = modes[mode][lower] >> 16;
  uint8_t lower_b = modes[mode][lower] >> 8;
  uint8_t upper_r = modes[mode][upper] >> 24;
  uint8_t upper_g = modes[mode][upper] >> 16;
  uint8_t upper_b = modes[mode][upper] >> 8;
  return RGB((uint8_t)round(lower_r * (1-phase) + upper_r * phase),
             (uint8_t)round(lower_g * (1-phase) + upper_g * phase),
             (uint8_t)round(lower_b * (1-phase) + upper_b * phase));
}
