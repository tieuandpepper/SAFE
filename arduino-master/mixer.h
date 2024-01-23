#ifndef Mixer_h
#define Mixer_h

#include "Arduino.h"

#define DELAY_MS 5

typedef struct mixer_type
{
  bool running = false;
  uint8_t pin;
} mixer_t;

bool MixerStart(mixer_t* mixer);
bool MixerStop(mixer_t* mixer);
bool MixerRun(mixer_t * mixer, int32_t time_ms);

#endif