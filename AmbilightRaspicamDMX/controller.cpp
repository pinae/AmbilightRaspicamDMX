#include <cstdint>
#include <wiringPi.h>
#include <unistd.h>
#include "fader.h"
#include "olawrapper.h"

int main()
{
  bool ambilightIsRunning = false;
  Fader* fader;
  bool faderIsRunning = false;
  // Start WiringPi
  if (wiringPiSetup() == -1) return 1;
  // Configure GPIO pins
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  // Create OlaWrapper
  OlaWrapper* ola = new OlaWrapper();
  // Main loop
  while(true) {
    if(!digitalRead(1) && digitalRead(2)) {
      ambilightIsRunning = true;
    } else {
      ambilightIsRunning = false;
    }
    if(digitalRead(1) && !digitalRead(2)) {
      if (!faderIsRunning) {
        fader = new Fader();
        faderIsRunning = true;
      }
      // Start fading
      fader->setMode((digitalRead(3)<<1)+digitalRead(4)-1);
      fader->step(ola);
    } else {
      if(faderIsRunning) {
        // Stop fading
        ola->blackout();
        ola->send();
        delete fader;
        faderIsRunning = false;
      }
    }
    if (!ambilightIsRunning) {
      usleep(30);
    }
  }
  return 0;
}
