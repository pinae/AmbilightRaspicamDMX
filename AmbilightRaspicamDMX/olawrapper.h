#ifndef OLAWRAPPER_H
#define OLAWRAPPER_H
#include <cstdint>
#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>


class OlaWrapper
{
public:
    OlaWrapper();
    void setChannel(uint8_t channel, uint8_t value);
    void blackout();
    void send();

private:
    uint8_t universe;
    ola::DmxBuffer buffer; // A DmxBuffer to hold the data.
    ola::client::StreamingClient ola_client;
};

#endif // OLAWRAPPER_H
