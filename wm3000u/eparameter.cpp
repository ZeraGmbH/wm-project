// implementierung eParameter
// zur überprüfung von einheiten behafteter größen
// und zum wandeln derselben

#include "eparameter.h"
#include "scalefactors.h"
#include "errorunits.h"
#include "voltageunits.h"
#include "currentunits.h"
#include "loadpointunits.h"
#include "angleunits.h"
#include <qdatastream.h>


eUnit RCFUnit[AnzRCFUnits] = { eUnit("", 1.0) };

QDataStream& operator << (QDataStream& ds, eUnit& eu)
{
    ds << eu.Name << eu.fak;
    return ds;
}


QDataStream& operator >> (QDataStream& ds, eUnit& eu)
{
    ds >> eu.Name >> eu.fak;
    return ds;
}


void eParameter::operator =(QString s) {
    eP = s;
    Unit = 0;
}

bool eParameter::Test(eUnit* eu, int n)
{
    for (int i=0; i<n; i++,eu++)
    {
        if (eP.contains(eu->Name))
        {
            bool ok;
            QString teststr = eP;
            teststr.remove(eu->Name);
            teststr.toDouble(&ok);
            if (ok)
            {
                Unit = eu;
                return true;
            }
        }
    }
    return false;
}

bool eParameter::isCurrent() {
    Unit = 0;
    return Test(CurrentUnit, sizeof(CurrentUnit)/sizeof(eUnit));
}


bool eParameter::isVoltage() {
    Unit = 0;
    return Test(VoltageUnit, sizeof(VoltageUnit)/sizeof(eUnit));
}


bool eParameter::isLoadPoint() {
    Unit = 0;
    return Test(LoadpointUnit, sizeof(LoadpointUnit)/sizeof(eUnit));
}


bool eParameter::isError() {
    Unit = 0;
    return Test(ErrorUnit, sizeof(ErrorUnit)/sizeof(eUnit));
}


bool eParameter::isAngle() {
    Unit = 0;
    return Test(AngleUnit, sizeof(AngleUnit)/sizeof(eUnit));
}


bool eParameter::withoutUnit() {
    bool t;
    eP.toDouble(&t);
    return t;
}


double eParameter::toDouble(bool* ok) {
    QString s = eP;
    if (Unit != 0) {
        s.remove(Unit->Name);
        s.stripWhiteSpace();
        return (s.toDouble(ok)*(Unit->fak));
    }
    else
    {
        if (isCurrent()) return toDouble(ok);
        if (isVoltage()) return toDouble(ok);
        if (isLoadPoint()) return toDouble(ok);
        if (isError()) return toDouble(ok);
        if (isAngle()) return toDouble(ok);
        if (isScaleFactor()) return toDouble(ok);
        return (s.toDouble(ok)); // im zweifelsfalle für den angeklagten
    }
}


bool eParameter::isScaleFactor()
{
    Unit = 0;
    return Test(ScaleFactor, sizeof(ScaleFactor)/sizeof(eUnit));
}

