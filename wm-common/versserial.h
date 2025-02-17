#ifndef VERSSERIAL_H
#define VERSSERIAL_H

#include <QString>
#include <QFile>
#include <QTextStream>

class tVersSerial
{
public:
    tVersSerial();;
    QString GetReleaseVersion();;

    QString DeviceName;
    QString DeviceVersion;
    QString PCBSerialNr;
    QString PCBVersion;
    QString PCBServer;
    QString DSPVersion;
    QString DSPServer;
    QString JDataChksum;
private:
    QString ReleaseVersion;
    void setReleaseNr();

};
#endif // VERSSERIAL_H
