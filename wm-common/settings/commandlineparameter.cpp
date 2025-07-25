#include "commandlineparameter.h"
#include "qfileinfo.h"
#include "qstringlist.h"


CommandLineParameter::CommandLineParameter()
{

}

void CommandLineParameter::Parse(int argc, char *argv[])
{
    ParseFromStringList(convertArgumentsToStringList(argc,argv));
}

void CommandLineParameter::ParseFromStringList(QStringList largv)
{
    foreach(QString option, largv)
    {

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

void CommandLineParameter::setFileName(const QString str)
{
    mFileName = str;
}

void CommandLineParameter::ParseFile()
{
    QStringList largv;
    QString option;
    QFile fConfFile;
    if (!mFileName.isEmpty())
    {
        fConfFile.setFileName(mFileName);
        if (fConfFile.open(QIODevice::ReadOnly))
        {
            while(!fConfFile.atEnd())
            {
                option = fConfFile.readLine();
                option = option.mid(0,option.length()-1);
                largv.append(option);
            }
            fConfFile.close();
        }
        ParseFromStringList(largv);
    }
}

QStringList CommandLineParameter::convertArgumentsToStringList(int argc, char *argv[])
{
    QStringList list;
    for (int i = 1; i < argc; i++)
    {
        list.append(argv[i]);
    }
    return list;
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
    if(bJustage) str += "just ";
    return str;
}
