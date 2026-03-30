// implementierung cConfData

#include <qdatastream.h>
#include "confdata.h"
#include "qmap.h"
#include "qvariant.h"

void cConfData::setConfVersion()
{
    m_nVersion = ConfVersion10;
}

void cConfData::setConventional(bool b)
{
    mbConventional = b;
}

void cConfData::serialize(QDataStream& ts)
{
    ts << m_nVersion << (int)m_bSimulation << (int)m_bRunning << (int)m_bOECorrection << (int)m_bOENewLoad;
    ts << m_nMeasMode << m_nMeasPeriod << m_nIntegrationTime<< m_nSyncSource;
    ts << m_nTSync << m_nSFreq << m_nSRate << FirstASDU << LastASDU << DataSet;
    ts << m_nLogFileMax << m_fxPhaseShift << m_fxTimeShift;
    ts << m_MacSourceAdr << m_MacDestAdr;
    ts << m_sOETFile << m_sResultFile; 
    ts << m_NPrimary << m_NSecondary << m_XPrimary << m_XSecondary;
    ts << m_ETPrimary << m_ETSecondary;
    ts << m_sRangeN << m_sRangeNSoll << m_sRangeNVorgabe;
    ts << m_sRangeX << m_sRangeXSoll << m_sRangeXVorgabe;
    ts << m_sRangeET << m_sRangeETSoll << m_sRangeETVorgabe;
    ts << m_sADSPFile;
    ts << m_nPriorityTagged;
    ts << m_nEthTypeHeader;
    ts << (int) m_bStrongEthSynchronisation;
    ts << Language;
    ts << (int)m_bDCmeasurement;
    ts << (int)m_bOffsetCorrectionN;
    ts << (int)m_bOffsetCorrectionX;
}


bool cConfData::deserialize(QDataStream& ts)
{
    int tmp;
    bool ret;
    
    ts >> tmp;
    if ((ret = (tmp >= ConfVersion8)))
    {
        int s,r,o,ol;
        ts >> s >> r >> o >> ol;
        m_bSimulation = s; m_bRunning = r; m_bOECorrection = o; m_bOENewLoad = ol;
        ts >> m_nMeasMode >> m_nMeasPeriod >> m_nIntegrationTime>> m_nSyncSource;
        ts >> m_nTSync >> m_nSFreq >> m_nSRate >> FirstASDU >> LastASDU >> DataSet;
        ts >> m_nLogFileMax >> m_fxPhaseShift >> m_fxTimeShift;
        ts >> m_MacSourceAdr >> m_MacDestAdr;
        ts >> m_sOETFile >> m_sResultFile;
        ts >> m_NPrimary >> m_NSecondary >> m_XPrimary >> m_XSecondary;
        ts >> m_ETPrimary >> m_ETSecondary;
        ts >> m_sRangeN >> m_sRangeNSoll >> m_sRangeNVorgabe;
        ts >> m_sRangeX >> m_sRangeXSoll >> m_sRangeXVorgabe;
        ts >> m_sRangeET >> m_sRangeETSoll >> m_sRangeETVorgabe;
        ts >> m_sADSPFile;
        ts >> m_nPriorityTagged;
        ts >> m_nEthTypeHeader;
        int t;
        ts >> t;
        m_bStrongEthSynchronisation = (bool) t;
        ts >> Language;
        m_bOENewLoad = (m_sOETFile != "") ; // falls schon eine datei geladen war -> mitteilen
    }

    if (tmp == ConfVersion9 || tmp == ConfVersion10)
    {
        int dc;
        ts >> dc;
        m_bDCmeasurement = dc;
    }

    if (tmp == ConfVersion10)
    {
        int yes;
        ts >> yes;
        m_bOffsetCorrectionN = yes;
        ts >> yes;
        m_bOffsetCorrectionX = yes;
    }

    return ret;
}

QString cConfData::extractJson()
{
     QMap<QString, QVariant> map;

    map["Version"] =  m_nVersion;
    map["Simulation"] = m_bSimulation;
    map["Running"] = m_bRunning;
    map["OECorrection"] = m_bOECorrection;
    map["OENewLoad"] = m_bOENewLoad;
    map["MeasMode"] =  m_nMeasMode;
    map["MeasPeriod"] = m_nMeasPeriod;
    map["IntegrationTime"] = m_nIntegrationTime;
    map["SyncSource"] = m_nSyncSource;
    map["TSync"] = m_nTSync;
    map["SFreq"] = m_nSFreq;
    map["SRate"] = m_nSRate;
    map["nLogFileMax"] = m_nLogFileMax;
    map["xPhaseShift"] = m_fxPhaseShift;
    map["xTimeShift"] = m_fxTimeShift;
    map["OETFile"] = m_sOETFile;
    map["ResultFile"] = m_sResultFile;
    map["NPrimary"] = m_NPrimary;
    map["NSecondary"] = m_NSecondary;
    map["XPrimary"] = m_XPrimary;
    map["XSecondary"] = m_XSecondary;
    map["ETPrimary"] = m_ETPrimary;
    map["ETSecondary"] = m_ETSecondary;
    map["RangeN"] = m_sRangeN;
    map["RangeNSoll"] = m_sRangeNSoll;
    map["RangeNVorgabe"] = m_sRangeNVorgabe;
    map["RangeX"] = m_sRangeX;
    map["RangeXSoll"] = m_sRangeXSoll;
    map["RangeXVorgabe"] = m_sRangeXVorgabe;
    map["RangeET"] = m_sRangeET;
    map["RangeETSoll"] = m_sRangeETSoll;
    map["RangeETVorgabe"] = m_sRangeETVorgabe;
    //map["ADSPFile"] = m_sADSPFile;        // there is no need to hand out the location of the adsp file!
    map["Language"] = Language;
    map["DCmeasurement"] = m_bDCmeasurement;
    map["OffsetCorrectionN"] = m_bOffsetCorrectionN;
    map["OffsetCorrectionX"] = m_bOffsetCorrectionX;
    if(!mbConventional)
    {
        map["FirstASDU"] = FirstASDU;
        map["LastASDU"] = LastASDU;
        map["DataSet"] = DataSet;
        map["PriorityTagged"] = m_nPriorityTagged;
        map["EthTypeHeader"] = m_nEthTypeHeader;
        map["StrongEthSynchronisation"] = m_bStrongEthSynchronisation;
        map["MacSourceAdr"] = m_MacSourceAdr.GetMacAdress();
        map["MacDestAdr"] = m_MacDestAdr.GetMacAdress();
    }


    return jsonExport.variantToJson(map);
}
