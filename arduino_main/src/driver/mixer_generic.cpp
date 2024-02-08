#include "mixer_generic.h"


bool MixerStart(mixer_t* mixer)
{
  digitalWrite(mixer->pin, HIGH);
  mixer->running = true;
  return true;
}

bool MixerStop(mixer_t* mixer)
{
  digitalWrite(mixer->pin, LOW);
  mixer->running = false;
  return true;
}

bool MixerRun(mixer_t * mixer, int32_t time_ms)
{
  bool res = MixerStart(mixer);
  delay(time_ms);
  res &= MixerStop(mixer);
  return res;
}