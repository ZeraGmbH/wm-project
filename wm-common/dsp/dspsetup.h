#ifndef DSPSETUP_H
#define DSPSETUP_H

#include "confdata.h"
#include "dspiface.h"

struct MeasDataStruct
{
    cDspMeasData<float>* MaxValData;
    cDspMeasData<float>* RMSValData; // schnelle rms messung (4..26 * 20ms @ 50hz)
    cDspMeasData<float>* ActValData;
    cDspMeasData<float>* RawValData0;
    cDspMeasData<float>* RawValData1;
    cDspMeasData<float>* RawValDataSinConHanning;
    cDspMeasData<float>* RawValData2;
    cDspMeasData<float>* RawValData3;
};

struct EthDataStruct
{
    cDspMeasData<quint32>* ETHStatusHandle;
    cDspMeasData<quint32>* ETHStatusResetHandle;
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
