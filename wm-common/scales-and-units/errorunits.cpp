#include "errorunits.h"

eUnit ErrorUnit[AnzErrorUnits] = { eUnit("%", 1.0e-2),
                                   eUnit(QString(QChar(8240)), 1.0e-3), // promille
                                   eUnit("ppm", 1.0e-6) };
