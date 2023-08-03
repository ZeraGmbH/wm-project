#ifndef WM3KSCPISPECIAL_H
#define WM3KSCPISPECIAL_H

#include "wmglobal.h"
#include "confdata.h"
#include <QString>

class WM3kSCPISpecial
{
public:
    WM3kSCPISpecial();
    bool isConventional();
    bool isDC();
    bool isNewSamplerates();
    QString getExTName();
    QString getMaxRange();
    QString getConfOperModeCatalog();
    bool setWM1000SetConfOperMode(cConfData *conf, int m);
    bool setWM3000SetConfOperMode(cConfData *conf, int m);
};

#endif // WM3KSCPISPECIAL_H
