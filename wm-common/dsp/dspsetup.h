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

struct EthDataStruct
{
    cDspMeasDataUlong* ETHStatusHandle;
    cDspMeasDataUlong* ETHStatusResetHandle;
};

class DspSetup
{
public:
    void setDspMemList(cDspIFace* DspIFace);
    void setDspVarList(cConfData* confData, cDspIFace* DspIFace, int sampleCount);
    void SetDspWMCmdList(cConfData* confData, cDspIFace* DspIFace, int sampleCount, bool bnConventional);
    MeasDataStruct* getMeasData();
    EthDataStruct* getEthData();
private:
    MeasDataStruct m_measData;
    EthDataStruct m_ethData;
};

#endif // DSPSETUP_H
