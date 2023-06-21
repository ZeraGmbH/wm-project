#ifndef DSPSETUP_H
#define DSPSETUP_H

#include "confdata.h"
#include "dspiface.h"

struct MeasDataStruct
{
    cDspMeasData* MaxValData;
    cDspMeasData* RMSValData; // schnelle rms messung (4..26 * 20ms @ 50hz)
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
    void setDspVarList(cConfData* confData, cDspIFace* DspIFace, int sampleCount);
    MeasDataStruct* getMeasData();
private:
    MeasDataStruct m_measData;
};

#endif // DSPSETUP_H
