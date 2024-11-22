#ifndef COMMANDLINEPARAMETER_H
#define COMMANDLINEPARAMETER_H

#include <QString>

class CommandLineParameter
{
public:
    CommandLineParameter();
    void Parse(int argc, char *argv[]);
    QString GetOptionString();
    bool GetJustage() {return bJustage;};
    bool GetConvent() {return bConvent;};
    bool GetDC() {return bDc;};
    bool GetNewSampleRates() {return bNewSampleRates;};
    bool GetPpsWatchDog() {return bPpsWatchDog;};
    QString GetIpAdress() {return mstrIPAdress;}
private:
    bool bJustage = false;
    bool bConvent = false;
    bool bDc = false;
    bool bNewSampleRates = false;
    bool bPpsWatchDog = false;
    bool bIpAdress = false;
    QString mstrIPAdress = "127.0.0.1";

    const QString strJustage = "-justage";
    const QString strConvent = "-convent";
    const QString strDc = "-dc";
    const QString strNewSampleRates = "-newsamplerates";
    const QString strIPAdress = "-ip";
    const QString strPpsWatchDog = "-ppswatchdog";

};

#endif // COMMANDLINEPARAMETER_H
