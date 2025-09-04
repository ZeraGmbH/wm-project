#ifndef CURRENTUNITS_H
#define CURRENTUNITS_H

#include "eunit.h"

enum CurrentUnits { CurrkAw3,
                    CurrmAw3,
                    CurruAw3,
                    CurrkA3,
                    CurrmA3,
                    CurruA3,
                    CurrkA,
                    CurrmA,
                    CurruA,
                    CurrAw3,
                    CurrA3,
                    CurrA,
                    CurrkAw2,
                    CurrAw2,
                    CurrmAw2,
                    CurruAw2,
                    AnzCurrentUnits};

extern eUnit CurrentUnit[AnzCurrentUnits];

#endif // CURRENTUNITS_H
