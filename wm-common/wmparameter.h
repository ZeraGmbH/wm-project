#ifndef WMPARAMETER_H
#define WMPARAMETER_H

#include "eunit.h"
#include <QString>

class WmParameter	{
public:
    WmParameter(QString s) {eP = s;Unit=0;};
    WmParameter(){Unit=0;};
    ~WmParameter(){};

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

#endif // WMPARAMETER_H
