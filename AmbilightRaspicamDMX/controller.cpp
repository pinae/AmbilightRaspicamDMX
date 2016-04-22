#include <cstdint>
#include <wiringPi.h>
#include <unistd.h>
#include "fader.h"
#include "olawrapper.h"
#include "ambilight.h"
#include <iostream>
#include <curses.h>

int main()
{
  Ambilight* ambilight;
  bool ambilightIsRunning = false;
  Fader fader;
  bool faderIsRunning = false;
  // Start WiringPi
  if (wiringPiSetup() == -1) return 1;
  // Configure GPIO pins
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  // Curses configuration
  /*initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);*/
  // Create OlaWrapper
  OlaWrapper ola;
  // Main loop
  while(true) {
    if(!digitalRead(1) && digitalRead(2)) {
      if(!ambilightIsRunning) {
        ambilight = new Ambilight();
        ambilightIsRunning = true;
      }
      ambilight->step(ola);
    }
    else {
      if(ambilightIsRunning) {
        ola.blackout();
        ola.send();
        delete ambilight;
        ambilightIsRunning = false;
      }
    }
    if(digitalRead(1) && !digitalRead(2)) {
      if (!faderIsRunning) {
        faderIsRunning = true;
        std::cout << "Started fading" << "\n\r";
      }
      // Start fading
      fader.setMode((digitalRead(3)<<1)+digitalRead(4)-1);
      fader.step(ola);
    }
    else {
      if(faderIsRunning) {
        // Stop fading
        ola.blackout();
        ola.send();
        faderIsRunning = false;
      }
    }
    /*int pk = getch();
    if(pk == 'q') {
      break;
    }*/
    if (!ambilightIsRunning) {
      usleep(30000);
    }
  }
  ola.blackout();
  ola.send();
  if(ambilightIsRunning) {
    delete ambilight;
  }
  //endwin();
  std::cout << "Bye!" << std::endl;
  return 0;
}
