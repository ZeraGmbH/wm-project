// implementierung WmParameter
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




