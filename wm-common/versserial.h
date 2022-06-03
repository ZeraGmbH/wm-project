#ifndef VERSSERIAL_H
#define VERSSERIAL_H

#include <QString>

struct tVersSerial
{
    QString DeviceName;
    QString DeviceVersion;
    QString PCBSerialNr;
    QString PCBVersion;
    QString PCBServer;
    QString DSPVersion;
    QString DSPServer;
    QString JDataChksum;
};

#endif // VERSSERIAL_H
