#include "olawrapper.h"
#include <stdlib.h>
#include <unistd.h>
#include <ola/DmxBuffer.h>
#include <ola/Logging.h>
#include <ola/client/StreamingClient.h>
#include <iostream>
using std::cout;
using std::endl;

OlaWrapper::OlaWrapper()
  // Create a new client.
  :ola_client((ola::client::StreamingClient::Options()))
{
  universe = 0; // universe to use for sending data
  ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);
  buffer.Blackout(); // Set all channels to 0
  // Setup the client, this connects to the server
  if (!ola_client.Setup()) {
    std::cerr << "Setup failed" << endl;
    exit(1);
  }
}

OlaWrapper::~OlaWrapper()
{
  blackout();
  send();
}

void OlaWrapper::setChannel(uint8_t channel, uint8_t value)
{
  buffer.SetChannel(channel, value);
}

void OlaWrapper::blackout()
{
  buffer.Blackout();
}

void OlaWrapper::send()
{
  if (!ola_client.SendDmx(universe, buffer)) {
    cout << "Send DMX failed" << endl;
    exit(1);
  }
}
