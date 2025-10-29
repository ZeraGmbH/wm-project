#include "anglecmpoverfrequency.h"
#include "defsamplerates.h"

extern int g_SRates[];

angleCmpOverFrequency::angleCmpOverFrequency()
{

}

angleCmpOverFrequency::angleCmpOverFrequency(cConfData *confData, cwmActValues *actValues)
{
    getCompValue(getSampleRate(confData->m_nSRate) ,confData->m_fSFreq);
    if (isFreuqencyInRange(actValues->Frequenz)){
        correctAngle(1.0,actValues->Frequenz,confData->m_fSFreq);
    }
}

double angleCmpOverFrequency::getCompValue(int sampleRate, int confFrequency)
{
    double val (0.0);
    if (sampleRate == 80) {
        if (confFrequency == 16)
            val = samp80Freq1623;
        if (confFrequency == 50)
            val = samp80Freq50;
        if (confFrequency == 60)
            val = samp80Freq60;
    }
    if (sampleRate == 256) {
        if (confFrequency == 16)
            val = samp256Freq1623;
        if (confFrequency == 50)
            val = samp256Freq50;
        if (confFrequency == 60)
            val = samp256Freq60;
    }
    m_angleCorrectionFactor = val;
    return val;
}

bool angleCmpOverFrequency::isFreuqencyInRange(double frequency)
{
    bool ok(false);
    if ((frequency > 15.0) && (frequency < 65.0))
        ok = true;
    return ok;
}

double angleCmpOverFrequency::correctAngle(double Angle, double actualFrequency, double confFrequency)
{
    double corrAngle;
    double diffFrequency;
    diffFrequency = actualFrequency - confFrequency;
    double corrVal = diffFrequency * m_angleCorrectionFactor;
    corrAngle = Angle + corrVal;
    m_angleCorrectionValue = corrVal;
    return corrAngle;
}

double angleCmpOverFrequency::getAngleCorrectionValue()
{
    return m_angleCorrectionValue;
}

int angleCmpOverFrequency::getSampleRate(int sr)
{  // don't repeat yourselve!
    return g_SRates[sr];
}
