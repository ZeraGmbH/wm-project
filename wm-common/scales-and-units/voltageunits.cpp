#include "voltageunits.h"

eUnit VoltageUnit[AnzVoltageUnits] = { eUnit("MV", 1.0e6),
                                       eUnit("kV/3", 333.33333),
                                       eUnit("kV/w3", 577.3502692),
                                       eUnit("kV", 1.0e3),
                                       eUnit("V/3", 0.333333333),
                                       eUnit("V/w3", 0.57735027),
                                       eUnit("mV/3", 0.333333333e-3),
                                       eUnit("mV/w3", .57735027e-3),
                                       eUnit("mV", 1.0e-3),
                                       eUnit("uV", 1.0e-6),
                                       eUnit("V", 1.0) };
