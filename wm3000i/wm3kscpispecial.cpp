#include "wm3kscpispecial.h"

extern char* MModeName[];

char* SRatesName[MaxSRate] = {(char*)"80",(char*)"96",(char*)"240",(char*)"256",(char*)"288"}; // abtastraten
int SRates[MaxSRate] = {80,96,240,256,288};

WM3kSCPISpecial::WM3kSCPISpecial()
{

}

QString WM3kSCPISpecial::getExTName()
{
    return "ECT";
}

QString WM3kSCPISpecial::getMaxRange()
{
    return "15.0A";
}

QString WM3kSCPISpecial::getConfOperModeCatalog()
{
/*    QString rs;
    if (g_WMDevice->isConventional())
        rs = QString("%1,%2").arg(In_IxAbs).arg(MModeName[In_IxAbs]);
    else

    {
        rs = QString("%1,%2").arg(In_ECT).arg(MModeName[In_ECT]);
        for (int i = In_ECT+1; i < maxMMode; i++)
            rs = rs + ";" + QString("%1,%2").arg(i).arg(MModeName[i]);
    }
    return rs;*/
}
