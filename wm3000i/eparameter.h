// headerdatei eparameter.h

#ifndef EPARAMETER_H
#define EPARAMETER_H

#include "eunit.h"
#include <qstring.h>

enum ErrorUnits {ErrProzent, ErrPromill, Errppm, AnzErrorUnits};
enum VoltageUnits {VoltMV, VoltkV3, VoltkVw3, VoltkV, VoltV3, VoltVw3, VoltmV3, VoltmVw3,VoltmV, VoltuV, VoltV, AnzVoltageUnits};
enum CurrentUnits {CurrkAw3,CurrmAw3, CurruAw3, CurrkA3, CurrmA3, CurruA3, CurrkA, CurrmA,CurruA,  CurrAw3, CurrA3, CurrA, AnzCurrentUnits};
enum LoadpointUnits {LPProzent, LPPromill, AnzLoadPointUnits};
enum AngleUnits { Anglegrad, Anglemin , Anglecrad, Anglemrad, Angleurad, AngleGrad2, Anglerad , AnzAngleUnits};
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
