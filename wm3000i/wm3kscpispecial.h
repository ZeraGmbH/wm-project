#ifndef WM3KSCPISPECIAL_H
#define WM3KSCPISPECIAL_H

#include "wmglobal.h"
#include "confdata.h"
#include "wmactvalues.h"
#include "wm3000scpiface.h"
#include "wm3kscpispecialbase.h"

#include <QString>

class WM3kSCPISpecial : public WM3kSCPISpecialBase
{
public:
    int getmaxMMode()override {return maxMMode;};
    virtual bool isConventional() override;
    virtual bool isDC() override;
    virtual bool isNewSamplerates() override;
    virtual QString getExTName() override;
    virtual QString getMaxRange() override;
    virtual QString getConfOperModeCatalog() override;
    virtual bool setWM1000SetConfOperMode(cConfData *conf, int m) override;
    virtual bool setWM3000SetConfOperMode(cConfData *conf, int m) override;
    virtual QString fetchActualValues(cwmActValues *act) override;
};

#endif // WM3KSCPISPECIAL_H
