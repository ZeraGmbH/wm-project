#include "wm3kscpispecial.h"
#include "wm3000i.h"

extern cWM3000I* g_WMDevice;
extern char* MModeName[];

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
    return "ECT";
}

QString WM3kSCPISpecial::getMaxRange()
{
    return "15.0A";
}

QString WM3kSCPISpecial::getConfOperModeCatalog()
{
    QString rs;
    if (isConventional())
        rs = QString("%1,%2").arg(In_IxAbs).arg(MModeName[In_IxAbs]);
    else

    {
        rs = QString("%1,%2").arg(In_ECT).arg(MModeName[In_ECT]);
        for (int i = In_ECT+1; i < maxMMode; i++)
            rs = rs + ";" + QString("%1,%2").arg(i).arg(MModeName[i]);
    }
    return rs;
}

bool WM3kSCPISpecial::setWM1000SetConfOperMode(cConfData *conf, int m)
{
    conf->m_nMeasMode = In_IxAbs;
    return (m == In_IxAbs);
}

bool WM3kSCPISpecial::setWM3000SetConfOperMode(cConfData *conf, int m)
{
    if (m > In_IxDiff)
    {
        conf->m_nMeasMode = m;
        return true;
    }
    return false;
}

QString WM3kSCPISpecial::fetchActualValues(cwmActValues *act)
{
    QString str;
    str = QString(";%1;%2;%3;%4")
              .arg(act->AngleError)
              .arg(fabs(act->VekDX))
              .arg(UserAtan(act->VekDX))
              .arg(act->RCF);
    return str;
}
