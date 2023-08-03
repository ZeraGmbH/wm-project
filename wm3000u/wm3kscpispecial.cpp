#include "wm3kscpispecial.h"
#include "wm3000u.h"

extern cWM3000U* g_WMDevice;
extern char* MModeName[];

char* SRatesName[MaxSRate] = {(char*)"80",(char*)"256"}; // abtastraten
int SRates[MaxSRate] = {80,256};

WM3kSCPISpecial::WM3kSCPISpecial()
{

}

bool WM3kSCPISpecial::isConventional()
{
    return g_WMDevice->isConventional();
}

bool WM3kSCPISpecial::isDC()
{
    return g_WMDevice->isDC();
}

bool WM3kSCPISpecial::isNewSamplerates()
{
    return g_WMDevice->isNewSamplerates();
}

QString WM3kSCPISpecial::getExTName()
{
    return "EVT";
}

QString WM3kSCPISpecial::getMaxRange()
{
    return "480V";
}

QString WM3kSCPISpecial::getConfOperModeCatalog()
{
    QString rs;
    rs = QString("%1,%2").arg(Un_UxAbs).arg(MModeName[Un_UxAbs]);
    if (!g_WMDevice->isConventional())
        for (int i = Un_UxAbs+1; i < maxMMode; i++)
            rs = rs + ";" + QString("%1,%2").arg(i).arg(MModeName[i]);
    return rs;
}

bool WM3kSCPISpecial::setWM1000SetConfOperMode(cConfData *conf, int m)
{
    conf->m_nMeasMode = Un_UxAbs;
    return (m == Un_UxAbs);
}

bool WM3kSCPISpecial::setWM3000SetConfOperMode(cConfData *conf, int m)
{
    conf->m_nMeasMode = m;
    return true;
}
