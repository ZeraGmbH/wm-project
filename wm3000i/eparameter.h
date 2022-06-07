// headerdatei eparameter.h

#ifndef EPARAMETER_H
#define EPARAMETER_H

#include "eunit.h"
#include <qstring.h>

enum RCFUnits { nix, AnzRCFUnits};



class eParameter	{
public:
    eParameter(QString s) {eP = s;Unit=0;};
    eParameter(){Unit=0;};
    ~eParameter(){};
    
    void operator = (QString);
    bool isCurrent();
    bool isVoltage();
    bool isLoadPoint();
    bool isError();
    bool isAngle();
    bool withoutUnit();
    double toDouble(bool *ok = 0);
private:
    bool isScaleFactor();
    eUnit* Unit;
    QString eP;
    bool Test(eUnit*, int);
};


#endif
