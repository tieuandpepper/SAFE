#include "mixer.h"


bool MixerStart(mixer_t* mixer)
{
  digitalWrite(mixer->pin, HIGH);
  delay(DELAY_MS);
  if (digitalRead(mixer->pin) != HIGH)
  {
    return false;
  }
  mixer->running = true;
  return true;
}

bool MixerStop(mixer_t* mixer)
{
  digitalWrite(mixer->pin, LOW);
  delay(DELAY_MS);
  if (digitalRead(mixer->pin) != LOW)
  {
    return false;
  }
  mixer->running = false;
  return true;
}

bool MixerRun(mixer_t * mixer, uint32_t time_ms)
{
  bool res = MixerStart(mixer);
  delay(time_ms);
  res &= MixerStop(mixer);
  return res;
}