#ifndef COMMANDLINEPARAMETER_H
#define COMMANDLINEPARAMETER_H

#include <QString>

class CommandLineParameter
{
public:
    CommandLineParameter();
    void Parse(int argc, char *argv[]);
    void ParseFile();
    QString GetOptionString();
    QString GetOptionStringForFolders();
    bool GetJustage() {return bJustage;};
    bool GetConvent() {return bConvent;};
    bool GetDC() {return bDc;};
    bool GetNewSampleRates() {return bNewSampleRates;};
    bool GetPpsWatchDog() {return bPpsWatchDog;};
    bool GetScreenShoter() {return bScreen;};
    QString GetIpAdress() {return mstrIPAdress;}
    void setLanguage(const QString lang) {mLanguage = lang;};
    void setDevice(const QString device) {mDevice = device;};
    void setFileName(const QString str);
private:
    QStringList convertArgumentsToStringList(int argc, char *argv[]);
    void ParseFromStringList(QStringList largv);

    bool bJustage = false;
    bool bConvent = false;
    bool bDc = false;
    bool bNewSampleRates = false;
    bool bPpsWatchDog = false;
    bool bIpAdress = false;
    bool bScreen = false;
    bool bIgnoreFile = false;
    QString mstrIPAdress = "127.0.0.1";
    QString mLanguage, mDevice;
    QString mFileName;

    const QString strJustage = "-justage";
    const QString strConvent = "-convent";
    const QString strDc = "-dc";
    const QString strNewSampleRates = "-newsamplerates";
    const QString strIPAdress = "-ip";
    const QString strPpsWatchDog = "-ppswatchdog";
    const QString strScreen = "-screen";
    const QString strIgnore = "-ignoreFile";

};

#endif // COMMANDLINEPARAMETER_H
