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
