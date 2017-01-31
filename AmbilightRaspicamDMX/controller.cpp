#include <cstdint>
#include <wiringPi.h>
#include <unistd.h>
#include "fader.h"
#include "olawrapper.h"
#include "ambilight.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>

#define NB_DISABLE 0
#define NB_ENABLE 1


class KeypressMonitor {
public:
  KeypressMonitor(void)
  {
    nonblock(NB_ENABLE);
  }
  
  ~KeypressMonitor(void)
  {
    nonblock(NB_DISABLE);
  }
  
  int pressed()
  {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
  }
  
private:
  void nonblock(int state)
  {
    struct termios ttystate;
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);
    if (state==NB_ENABLE) {
      //turn off canonical mode
      ttystate.c_lflag &= ~ICANON;
      //minimum of number input read.
      ttystate.c_cc[VMIN] = 1;
    }
    else if (state==NB_DISABLE) {
      //turn on canonical mode
      ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
  }
};

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
  // Keyboard input configuration
  KeypressMonitor anyKey;
  // Create OlaWrapper
  OlaWrapper ola;
  // Main loop
  while(true) {
    if(!digitalRead(1) && digitalRead(2)) {
      if(!ambilightIsRunning) {
        ambilight = new Ambilight();
        ambilightIsRunning = true;
        std::cout << "Starting ambilight ..." << std::endl;
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
        std::cout << "Starting the color transitions ..." << std::endl;
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
    if(anyKey.pressed()) {
      if(getchar() == 'q') break;
    }
    if (!ambilightIsRunning) {
      usleep(30000);
    }
  }
  if(ambilightIsRunning) {
    delete ambilight;
  }
  std::cout << " ... Bye!" << std::endl;
  return 0;
}
