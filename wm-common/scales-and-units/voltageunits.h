#ifndef VOLTAGEUNITS_H
#define VOLTAGEUNITS_H

#include "eunit.h"

enum VoltageUnits {VoltMV, VoltkV3, VoltkVw2, VoltkVw3, VoltkV, VoltV3, VoltVw2, VoltVw3, VoltmV3, VoltkVm3, VoltmVw3,VoltmV, VoltuV, VoltV, AnzVoltageUnits};

extern eUnit VoltageUnit[AnzVoltageUnits];

#endif // VOLTAGEUNITS_H
