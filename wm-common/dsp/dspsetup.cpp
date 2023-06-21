#include "dspsetup.h"

void DspSetup::setDspVarList(cConfData *confData, cDspIFace *DspIFace, int sampleCount)
{
    int nS = sampleCount * confData->m_nMeasPeriod;
    int schanLen = (confData->m_nMeasPeriod < 4)? 4:confData->m_nMeasPeriod;
    schanLen *= sampleCount;

    DspIFace->ClearVarLists();

    // maxima
    m_measData.MaxValData = DspIFace->GetMVHandle(""); // wir holen uns ein handle für den maximumsucher
    DspIFace->addVarItem(m_measData.MaxValData, new cDspVar("MAXN",1,vApplication | vDspIntern));
    DspIFace->addVarItem(m_measData.MaxValData, new cDspVar("MAXX",1,vApplication | vDspIntern));
    DspIFace->addVarItem(m_measData.MaxValData, new cDspVar("MAXRDY",1,vApplication | vDspIntern));

    // schnelle rms messung zur lastpunkt bestimmung 1x rms gesamtsignal 1x ampl 1. grundwelle
    // erweitert jetzt auch für kanal x
    m_measData.RMSValData = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(m_measData.RMSValData, new cDspVar("FRMSN",1,vApplication | vDspIntern));
    DspIFace->addVarItem(m_measData.RMSValData, new cDspVar("FAMPL1N",1,vApplication | vDspIntern));
    DspIFace->addVarItem(m_measData.RMSValData, new cDspVar("FRMSX",1,vApplication | vDspIntern));
    DspIFace->addVarItem(m_measData.RMSValData, new cDspVar("FAMPL1X",1,vApplication | vDspIntern));

    m_measData.ActValData = DspIFace->GetMVHandle(""); // wir holen uns ein handle für die istwerte daten
    //	nur dsp intern verwendete messdaten
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("SINDEX",1,vDspIntern)); // index zur speicherung der sampledaten für die fehlermessung (variables messintervall);
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("SINDEX2",1,vDspIntern)); // index zur speicherung der sampledaten für die schnelle lastpunktmessung (festes messintervall = 4 signalperioden);

    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("TEMP1",1,vDspIntern)); // werden nur temp. benötigt weil winkel und betrag
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("TEMP2",1,vDspIntern)); // direkt ermittelt werden zwecks filterung

    // diese werte ab hier werden gefiltert
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("KFKORR",1,vDspIntern)); // kreisfrequenz korrektur koeffizient
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("RMSN",1,vDspIntern));
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("AMPL1N",1,vDspIntern));
    DspIFace->addVarItem(m_measData.ActValData,new cDspVar("RMSX",1,vDspIntern));
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("AMPL1X",1,vDspIntern));
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("DPHI",1,vDspIntern));
    // dphi=(phix-phin) - tdsync * (10*10^-9) * 2PI / (signalfreq* kfkorr) =(phix-phin) -tdsync * (2PI / (signalfreq*10^8)) * (1/kfkorr) -->

    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("FILTER",10,vDspIntern));
    // KFKORR wird separat gefiltert
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("N",1,vDspIntern));

    // gefilterte messergebnisse
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("KFKORRF",1,vApplication | vDspIntern)); // kreisfrequenz korrektur koeffizient
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("RMSNF",1,vApplication | vDspIntern)); // rms wert kanal n
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("AMPL1NF",1,vApplication | vDspIntern)); // amplitude 1. oberwelle kanal n
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("RMSXF",1,vApplication | vDspIntern)); // rms wert kanal x
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("AMPL1XF",1,vApplication | vDspIntern)); // amplitude 1. oberwelle kanal x
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("DPHIF",1,vApplication | vDspIntern)); // winkel kanal x - winkel kanal n

    // nicht gefilterte messergebnisse
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("TDSYNC",1,vApplication | vDspIntern)); // delay time pps -> 1. sample
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("PHIN",1,vApplication | vDspIntern));
    DspIFace->addVarItem(m_measData.ActValData, new cDspVar("PHIX",1,vApplication | vDspIntern));

    m_measData.RawValData0 = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(m_measData.RawValData0, new cDspVar("MESSSIGNAL0",nS,vApplication | vDspIntern));
    m_measData.RawValData1 = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(m_measData.RawValData1, new cDspVar("MESSSIGNAL1",nS,vApplication | vDspIntern));
    m_measData.RawValDataSinConHanning = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(m_measData.RawValDataSinConHanning, new cDspVar("SCHAN",schanLen,vApplication | vDspIntern)); // sinus, cosinus, hanning abwechselnd
    m_measData.RawValData2 = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(m_measData.RawValData2, new cDspVar("MESSSIGNAL2",4*sampleCount,vApplication | vDspIntern));
    m_measData.RawValData3 = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(m_measData.RawValData3, new cDspVar("MESSSIGNAL3",4*sampleCount,vApplication | vDspIntern));
}

MeasDataStruct *DspSetup::getMeasData()
{
    return &m_measData;
}
