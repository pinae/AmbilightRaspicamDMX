#ifndef OLAWRAPPER_H
#define OLAWRAPPER_H
#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>


class OlaWrapper
{
public:
    OlaWrapper();
    void setChannel(unsigned int channel, unsigned int value);
    void blackout();
    void send();

private:
    unsigned int universe;
    ola::DmxBuffer buffer; // A DmxBuffer to hold the data.
    ola::client::StreamingClient ola_client;
};

#endif // OLAWRAPPER_H
