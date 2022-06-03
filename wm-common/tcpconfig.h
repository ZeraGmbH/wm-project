#ifndef TCPCONFIG_H
#define TCPCONFIG_H

#include <QString>

class cTCPConfig
{
    public:
    cTCPConfig(){};

    QString pcbHost, dspHost;
    uint pcbPort, dspPort;
};

#endif // TCPCONFIG_H
