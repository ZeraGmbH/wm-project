#ifndef DSPSETUP_H
#define DSPSETUP_H

#include "confdata.h"
#include "dspiface.h"

struct MeasDataStruct
{
    cDspMeasData* MaxValData;
    cDspMeasData* RMSValData;
    cDspMeasData* ActValData;
    cDspMeasData* RawValData0;
    cDspMeasData* RawValData1;
    cDspMeasData* RawValDataSinConHanning;
    cDspMeasData* RawValData2;
    cDspMeasData* RawValData3;
};

class DspSetup
{
public:
    static void SetDspVarList(cConfData* confData, cDspIFace* DspIFace, MeasDataStruct &dspMeasData, int sampleCount);
};

#endif // DSPSETUP_H
