#ifndef WM3KSCPISPECIALBASE_H
#define WM3KSCPISPECIALBASE_H

#include "confdata.h"
#include "wmactvalues.h"
#include "wm3000scpiface.h"
#include <QString>

class WM3kSCPISpecialBase
{
public:
    virtual int getmaxMMode() = 0;
    virtual bool isConventional() = 0;
    virtual bool isDC() = 0;
    virtual bool isNewSamplerates() = 0;
    virtual QString getExTName() = 0;
    virtual QString getMaxRange() = 0;
    virtual QString getConfOperModeCatalog() = 0;
    virtual bool setWM1000SetConfOperMode(cConfData *conf, int m) = 0;
    virtual bool setWM3000SetConfOperMode(cConfData *conf, int m) = 0;
    virtual QString fetchActualValues(cwmActValues *act) = 0;
    virtual bool isCurrentWM() = 0;
};

#endif // WM3KSCPISPECIAL_H
