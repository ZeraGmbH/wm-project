#include "commandlineparameter.h"


CommandLineParameter::CommandLineParameter()
{

}

void CommandLineParameter::Parse(int argc, char *argv[])
{
    QString option;
    for (int i = 1; i < argc; i++)
    {
        option = argv[i];
        if (option == strJustage){
            bJustage = true;
            qDebug("Found option justage");
        }
        if (option == strConvent){
            bConvent = true;
            qDebug("Found option convent");
        }
        if (option == strDc){
            bDc = true;
            qDebug("Found option dc");
        }
        if (option == strNewSampleRates){
            bNewSampleRates = true;
            qDebug("Found option newsamplerates");
        }
        if(option.startsWith(strIPAdress)) {
            bIpAdress = true;
            mstrIPAdress = option.replace(strIPAdress, "").trimmed();
            qDebug("Found option ip address");
        }
        if (option == strPpsWatchDog){
            bPpsWatchDog = true;
            qDebug("Found option PPS Watch Dog");
        }
        if (option == strScreen){
            bScreen = true;
            qDebug("Found option Screen");
        }
    }
}

QString CommandLineParameter::GetOptionString()
{
    QString option;
    if(bJustage) option += "just ";
    if(bConvent) option += "conv ";
    if(bDc) option += "dc ";
    if(bNewSampleRates) option += "newsr ";
    if(bPpsWatchDog) option += "pwdt ";
    if(bIpAdress) option += "ip";
    return option;
}

QString CommandLineParameter::GetOptionStringForFolders()
{
    QString str;
    str = mLanguage;
    if(bConvent)  str += "wm1000"; else str += "wm3000";
    str += mDevice;
    if(bDc) str += "dc";
    if(bNewSampleRates) str += "newsr";
    return str;
}
