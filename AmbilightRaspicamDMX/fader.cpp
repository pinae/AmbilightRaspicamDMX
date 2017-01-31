#include "fader.h"
#include <math.h>

#include <iostream>

#define RGB(r, g, b) ((r) << 24 | (g) << 16 | (b) << 8)

Fader::Fader()
{
  modes[0] = {RGB(0U, 255U, 0U), RGB(0U, 128U, 128U), RGB(0U, 0U, 255U), RGB(128U, 0U, 128U), RGB(255U, 0U, 0U), RGB(128U, 128U, 0U)};
  modes[1] = {RGB(255U, 0U, 0U), RGB(128U, 128U, 0U), RGB(208U, 48U, 0U), RGB(138U, 118U, 0U)};
  modes[2] = {RGB(0U, 0U, 255U), RGB(0U, 128U, 128U), RGB(10U, 30U, 215U), RGB(0U, 148U, 108U)};
  state = 0;
  mode = 0;
}

void Fader::setRGBChannels(OlaWrapper &ola, uint8_t channelOffset, uint32_t color)
{
  uint8_t r = color >> 24;
  uint8_t g = color >> 16;
  uint8_t b = color >> 8;
  ola.setChannel(channelOffset, r);
  ola.setChannel(channelOffset+1, g);
  ola.setChannel(channelOffset+2, b);
}

void Fader::step(OlaWrapper &ola)
{
  setRGBChannels(ola, 1, getColorForState(state));
  setRGBChannels(ola, 6, getColorForState(state+0.5));
  setRGBChannels(ola, 9, getColorForState(state+1));
  ola.send();
  state += 0.002;
}

void Fader::setMode(uint8_t newMode)
{
  mode = newMode;
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
