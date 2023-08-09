#ifndef WM3000I_H
#define WM3000I_H

#include <memory>

#include <qstring.h>
#include <qstringlist.h>
#include <q3textbrowser.h>
#include <qtimer.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <Q3ValueList>

#include "wmprogressdialog.h"
#include "zhclientsocket.h"
#include "tcpconfig.h"
#include "logfile.h"
#include "range.h"
#include "ownerror.h"
#include "dspiface.h"
#include "pcbiface.h"
#include "en61850.h"
#include "devserver.h"
#include "wm3000scpiface.h"
#include "movingwindowfilter.h"
#include "calcinfo.h"
#include <justValues.h>
#include "wmscopedialog.h"
#include "dspsetup.h"

class cRSelectString: public QString {
public:    
    cRSelectString() : QString(){};
    cRSelectString (const QString s) : QString(s){};
    cRSelectString (const char* s) : QString(s){};
};

enum wm3000ActionHandlerState {
    wm3000Idle,    			// 0
    
    EnterSimulationMode,
    
    InitializationStart,
    InitializationConnect2pcbHost,
    
    InitializationOpenChannel0,
    InitializationOpenChannel1,
    
    InitializationSetRanges,
    InitializationSyncRange0,
    InitializationSyncRange01,
    InitializationSyncRange02, 
    InitializationSyncRange1,		// 10
    InitializationSyncRange11,
    InitializationSyncRange12,
      
    InitializationConnectDspIFace,
    InitializationBootDsp,
    InitializationBootDspPause,
    InitializationSetSamplingSystem,
    InitScopeDialog,
    InitializationSetDspVarList,
    InitializationSetDspCmdLists,
    InitializationActivateCmdLists,
    InitializationSetSamplingFrequency,	//20
    InitializationSetSamplingPSamples,
    InitializationSetAbsDiffMode,
    InitializationSetSenseMode,
    InitializationSetSyncSource,
    InitializationSetSyncTiming,
    InitializationSetDspSignalRouting,
    InitializationSetDsp61850SourceAdr,
    InitializationSetDsp61850DestAdr,
    InitializationSetDsp61850PriorityTagged,
    InitializationSetDsp61850EthTypeAppId,
    InitializationSetDsp61850EthSynchronisation,// 30
    InitializationResetMaximum,
    InitializationResetOVL,
    InitializationProtectionOff,
    InitializationReadPCBDeviceVersion,
    InitializationReadPCBServerVersion,
    InitializationReadPCBSerialNr,
    InitializationReadDSPDeviceVersion,
    InitializationReadDSPServerVersion,
    InitializationReadJustdataChksum,
    InitializationTestAdjustment,

    InitializationFinished,

    ConfigurationStart,
    
    ConfigurationSetRanges, // block wie bei Initialization...also nicht neu realisiert
    ConfigurationSyncRange0,
    ConfigurationSyncRange01,
    ConfigurationSyncRange02,
    ConfigurationSyncRange1,
    ConfigurationSyncRange11, 
    ConfigurationSyncRange12,		// 40
    
    ConfigurationTestIntegrationTime,
    ConfigurationTestDspVarList,
    ConfigurationSetDspVarList, // wie bei Initialization
    ConfigurationSetSamplingSystem, // wie bei Initialization
    ConfigurationTestCmdList,
    ConfigurationSetDspCmdLists, // wie bei Initialization
    ConfigurationActivateCmdLists, // wie bei Initialization
    ConfigurationTestSignalFrequency,
    ConfigurationSetSamplingFrequency, // wie bei Initialization
    ConfigurationTestSRate, // 50
    ConfigurationSetSamplingPSamples, // wie bei Initialization 

    ConfigurationTestAbsDiffMode,
    ConfigurationSetAbsDiffMode, // wie bei Initialization

    ConfigurationTestTMode,
    ConfigurationSetSenseMode, // wie bei Initialization
    ConfigurationTestSyncMode,
    ConfigurationSetSyncSource, // wie bei Initialization
    ConfigurationTestSyncTiming,
    ConfigurationSetSyncTiming, // wie bei Initialization
    ConfigurationTestDspSignalRouting,
    ConfigurationSetDspSignalRouting, // wie bei Initialization
    ConfigurationTestDsp61850SourceAdr, // 60
    ConfigurationSetDsp61850SourceAdr, // wie bei Initialization 
    ConfigurationTestDsp61850DestAdr,
    ConfigurationSetDsp61850DestAdr, // wie bei Initialization
    ConfigurationTestDsp61850PriorityTagged,
    ConfigurationSetDsp61850PriorityTagged, // wie bei Initialization
    ConfigurationTestDsp61850EthTypeAppId,
    ConfigurationSetDsp61850EthTypeAppId, // wie bei Initialization
    ConfigurationTestDsp61850EthSynchronisation,
    ConfigurationSetDsp61850EthSynchronisation, // wie bei Initialization
    ConfigurationFinished, // 70
    
    SetRangeStart,		
    SetRangeSetRanges, // block wie bei Initialization...also nicht neu realisiert
    SetRangeSyncRange0,
    SetRangeSyncRange01,
    SetRangeSyncRange02,
    SetRangeSyncRange1,
    SetRangeSyncRange11, 
    SetRangeSyncRange12,
    
    SetRangeResetMaximum, // wie bei Initialization
    SetRangeFinished, // 80
    
    TriggerMeasureStart,
    TriggerMeasureFinished,
    
    TriggerMeasureDataAcquisition,
    TriggerMeasureComputation,
    TriggerMeasureGetGainCorrCh0,
    TriggerMeasureGetGainCorrCh1,
    TriggerMeasureGetPhaseCorrCh0,
    TriggerMeasureGetPhaseCorrCh1,			// 90
    TriggerMeasureGetOffsetCorrCh0,
    TriggerMeasureGetOffsetCorrCh1,
    TriggerMeasureCorrection,
    
    MeasureStart,
    MeasureDataAcquisition, // wie bei Trigger.. 
    MeasureComputation, // dito
    MeasureGetGainCorrCh0,
    MeasureGetGainCorrCh1,
    MeasureGetPhaseCorrCh0,
    MeasureGetPhaseCorrCh1, // 100
    MeasureGetOffsetCorrCh0,
    MeasureGetOffsetCorrCh1,
    MeasureCorrection, // hier ist messung zu ende
    MeasureAllDataSampleRequest,
    MeasureAllDataSample,
    MeasureAllDataSampleRequest2ndCh,
    MeasureAllDataSample2ndCh,

    
    MeasureLPStart,
    MeasureLPComputation,
    
    RestartMeasurementStart,
    RestartMeasurementResetMaximum,
    RestartMeasurementRestart,
    RestartMeasurementFinished,
    
    RangeObsermaticStart,
    RangeObsermaticTestHWOVL,
    RangeObsermaticGetMaxData,
    RangeObsermaticTest, // wir testen die software max sucher und wählen bereiche vor
    RangeObsermaticSyncRange0, // block wie bei Initialization...also nicht neu realisiert
    RangeObsermaticSyncRange01,
    RangeObsermaticSyncRange02,
    RangeObsermaticSyncRange1,
    RangeObsermaticSyncRange11, 
    RangeObsermaticSyncRange12, // 110
    RangeObsermaticOverload, 
    RangeObsermaticResetMaximum,	
    RangeObsermaticResetOVL,
    RangeObsermaticFinished,
    
    SenseProtectionOff,
    SenseProtectionOffRM,
    SenseProtectionOffResetMaximum,
    SenseProtectionOffFinished,
    
    CmpPhaseCoeffStart,
    CmpPhaseCoeffCh0,
    CmpPhaseCoeffCh1,
    CmpPhaseCoeffCh2,
    CmpPhaseSetStatus,
    CmpPhaseSetStatus2,
    CmpPhaseCoeffFinished,

    CmpOffsetCoeffStart,
    CmpOffsetCoeffCh0,
    CmpOffsetCoeffCh1,
    CmpOffsetCoeffCh2, // 140
    CmpOffsetSetStatus,
    CmpOffsetSetStatus2,
    CmpOffsetCoeffFinished,
   
    PhaseNodeMeasStart,
    PhaseNodeMeasCoefficientClearNode,			// 120
    PhaseNodeMeasCoefficientClearStatus,
    PhaseNodeMeasCoefficientClearNodeStatus,
    PhaseNodeMeasCoefficientClearNFinished,
    PhaseNodeMeasBaseConfiguration,
    PhaseNodeMeasNodeConfig, // für jeden knoten muss etwas umkonfiguriert werden
    PhaseNodeMeasExec1,
    PhaseNodeMeasExec2,
    PhaseNodeMeasExec3,
    PhaseNodeMeasExec4,            
    PhaseNodeMeasExec5,
    PhaseNodeMeasFinished,			// 130

    OffsetMeasWM3000Start,
    OffsetMeasWM3000CoefficientClearNode,
    OffsetMeasWM3000CoefficientClearStatus,
    OffsetMeasWM3000CoefficientClearNodeStatus,
    OffsetMeasWM3000CoefficientClearNFinished,
    OffsetMeasWM3000BaseConfiguration,
    OffsetMeasWM3000Exec1, // 160
    OffsetMeasWM3000Exec2,
    OffsetMeasWM3000Exec3,
    OffsetMeasWM3000Exec4,
    OffsetMeasWM3000Exec5,
    OffsetMeasWM3000Exec6,
    OffsetMeasWM3000Finished,

    OffsetMeasWM3000StartVar,
    OffsetMeasWM3000BaseConfigurationVar,
    OffsetMeasWM3000Exec1Var,
    OffsetMeasWM3000Exec2Var, // 170
    OffsetMeasWM3000Exec3Var,
    OffsetMeasWM3000Exec4Var,
    OffsetMeasWM3000Exec5Var,
    OffsetMeasWM3000FinishedVar,

    OffsetMeasChannelNStart,
    OffsetMeasChannelNSync,
    OffsetMeasChannelNFinished,

    OffsetMeasChannelXStart,
    OffsetMeasChannelXSync,
    OffsetMeasChannelXFinished,

    JustageFlashProgStart,
    JustageFlashProgFinished,
    
    JustageFlashExportStart,
    JustageFlashExportFinished,
    
    JustageFlashImportStart,
    JustageFlashImportFinished,
    
    EN61850ReadStatusStart,
    EN61850ReadStatusFinished,
    
    EN61850WriteStatusStart,
    EN61850WriteStatusFinished,
    
    SelftestStart,
    SelftestBaseConfiguration,
    SelftestMeasConfiguration,
    SelftestSetRangeNX,
    SelftestMeasureNSync,
    SelftestMeasureN,
    SelftestMeasureXSync,
    SelftestMeasureX,
    SelftestFinished,
        
    wm3000Continue,
    wm3000Repeat,
};

class cWMessageBox: public QMessageBox
{
    Q_OBJECT
    
public:
    cWMessageBox();
    cWMessageBox ( const QString & caption, const QString & text, Icon icon, int button0, int button1, int button2, QWidget * parent = 0, const char * name = 0, bool modal = TRUE, Qt::WFlags f = Qt::WStyle_DialogBorder );
    virtual ~cWMessageBox();
    
protected slots:
    virtual void done(int r);
    
signals:
    void WMBoxClosed();
};
    

class Wm3000iOwnErrorParamChecker : public IOwnErrorParamUISpecific
{
public:
    bool checkUISpecificInvalidParam(WmParameter* param) override
    {
        return !param->isCurrent() || param->withoutUnit();
    }
};

class cWM3000I: public QObject
{
    Q_OBJECT

public:
    
    cWM3000I(); 
    virtual ~cWM3000I();
       
    cConfData m_ConfData; // konfigurationsdaten 
    cOwnError m_ownError; // eigenfehler
    cDspIFace* DspIFace;
    cPCBIFace* PCBIFace;
    void InitWM3000(); // einmal komplett initialisieren
    void setConventional(bool b);
    bool isConventional();
    void setDC(bool b);
    bool isDC();
    void setNewSamplerates(bool b);
    bool isNewSamplerates();
    void setIpAddress(QString address);

public slots:
     // slots, die vom hauptfenster aus aktiviert werden
    void GetOETAnalizeDone(void); // eigenfehler object hat seine arbeit erledigt (synchronisation)
    void JustageAmplitudeSlot(void);
    void JustagePhaseSlot(void);
    void JustagePhaseBerechnungSlot(void);
    void JustageFlashProgSlot(void);
    void JustageFlashExportSlot(QString);
    void JustageFlashImportSlot(QString);
    void JustageOffsetSlot(void);
    void JustageOffsetVarSlot();
    void JustageOffsetBerechnungSlot(void);
    void OffsetMessungChannelNSlot(void);
    void OffsetMessungChannelXSlot(void);
    void SelfTestManuell();
    void SelfTestRemote();
    void OffsetMessungChannelNRemote();
    void OffsetMessungChannelXRemote();
    
    // slots, die vom bereichauswahl menu aktiviert werden
    void SetRangeSlot(cConfData*); 
    // slots, die vom konfigurationsmenu aktiviert werden
    void SetConfDataSlot(cConfData*); // konfiguration aus confdialog übernehmen
    bool LoadSettings(QString); // zum lesen der einstellungen ....
    void WriteSettings(QString); // dito zum schreiben
    void StoreResultsSlot(); // abspeichern von messergebnissen
    void EN61850ResetStatusSlot();
    void EN61850InfoRequestSlot(); 
    void DefaultSettingsSlot();
    void DefaultSettingsMeasurementSlot();
    void wmScopeViewEnabled(bool bshow);
    
private slots:
    void ActionHandler(int); // zur serialisierung von actions , arbeitet die actions hintereinander ab
    // slot für messung und bereich obsermatic werden vom Timer aktiviert
    void MeasureSlot(); // messung 
    void MeasureLPSlot(); // messung rms n für lastpunkt
    void RangeObsermaticSlot(); // bereich überwachung und automatik
    void DspIFaceAsyncDataSlot(const QString&); // für asynchrone meldungen vom dsp server
    void XIFaceDoneSlot(); // wenn gültige daten da sind vom einen oder anderen server
    void DspIFaceErrorSlot(); // wenn fehler aufgetreten sind am dsp server
    void pcbIFaceErrorSlot(); // dito für leitenkarten server
    void PhaseJustSyncSlot(); // zur synchronisation des justageablaufes mit configuration und messung 
    void OffsetJustSyncSlot(); // dito
    void JustAbortSlot();
    void SelftestSyncSlot(); // zur synchronisation des selbsttest ablaufes mit configuration und messung
    void SelftestAbortSlot();
    void OverLoadMaxQuitSlot();
    void externalTriggerTimeoutTriggerd();
    
signals:    
    void SendConfDataSignal(cConfData*); // konfigurationsdaten senden
    void SendOEDataSignal(cOwnError*); // eigenfehlerdaten senden
    void SendActValuesSignal(cwmActValues*); // istwerte senden
    void SendJustValuesSignal(tJustValues*);
    void SendLPSignal(cwmActValues*); // es werden alle istwerte versendet, es sollaber nur der lp aktualisiert werden
    void SendIOStringSignal(QString); //  scpi input/output senden
    void SendLogFileSizeSignal(const long); // logfile grösse senden
    void SendConfDialogInfoSignal(QStringList&,QStringList&); // wandler primär/sekundär listen senden
    void SendRangeListSignal(cWMRangeList&,cWMRangeList&); // bereichauswahllisten  senden 
    void AbortProgramSignal(); // für einen programmabbruch
    void StartStateMachine(int);
    void EN61850StatusSignal(cEN61850Info*);
    void ConfigReady(); // zur sync. des phasejustage ablaufes
    void MeasureReady(); // dito
    void SetRangeReady(); // dito
    void RangeAutomaticDone(); // info dass bereichautomatik 1x aktiv war
    void SendVersionInfo(tVersSerial*); // wir versenden die versions , serial , chksum info 
    void AffectStatus(uchar, ushort); // zum setzen, rücksetzen der scpi status systeme
    void SelftestDone(int); // 0 ok  -1 fehler
    void OffsetValue(double);
    void JustifiedSignal(bool);
    void FreqQuestionable(bool);
    void PPSQuestionable(bool);
    
protected:
    tVersSerial SerialVersions; // serien nummer und version
    
    // listen für bereiche der messeinrichtung und auswahlwerte hierzu 
    cWMRangeList m_sNRangeList; // verkettete liste von absolut bereichen
    cWMRangeList m_sXRangeList; // verkettete liste von absolut bereichen
    cWMRangeList m_sECTRangeList; // liste für die bereiche der elektronischen wandler
    cCalcInfoList m_CalcInfoList; // liste mit bereichen, deren phasenkorrektur koeffizienten zu berechnen sind (kanal, bereich)
    cPhaseNodeMeasInfoList m_PhaseNodeMeasInfoList; // liste mit bereichinfo´s , und allen parameter die erforderlich siehe cPhaseNodeMeasInfo (wmglobal)
    cOffsetMeasInfoList m_OffsetMeasInfoList; // dito
    QStringList m_SelftestInfoList;
    cTCPConfig TCPConfig; // information zur verbindung zu den servern
    
private:
    bool m_binitDone;
    bool m_bIFError; // zum feststellen ob bei der letzten aktion ein fehler war
    
    void DefaultSettings(cConfData&);
    void DefaultSettingsMeasurement(cConfData&);
    void ServerIFaceErrorHandling(int, QString, int);
    cSMTimer* m_ActTimer; // für actionhandler ablaufsteuerung
    cSMTimer* m_AsyncTimer; // für actionhandler ablaufsteuerung
    QList<int> AHSFifo;
     
    cConfData m_ConfDataCopy; // kopie der konfigurationsdaten 
    cConfData SaveConfData; // retten der konfiguration während des justageablaufes
    cConfData NewConfData; // zum setzen der konfiguration aus justageablauf  
    bool TestOverload(cWMRangeList&,QString&, float);
    bool SelectRange(cWMRangeList&,QString&,QString&,QString&,float,bool&); // sucht aus der liste der bereiche und soll,ist,vorgabe werte und dem max. einen bereich, rückgabe -> zwingende änderung
    CWMRange* Range(cRSelectString,cWMRangeList&); // sucht den bereich über den selector
    CWMRange* Range(QString,cWMRangeList&); // sucht den bereich über den namen
    CWMRange* Range(float,cWMRangeList&); // sucht einen beeich dessen bereichendwert >= float par ist
    
    CWMRange* DummyRange;
 
    int getSampleRate(int sr);
    void SetDspWMVarList();
    void SetDspWMCmdList();

    void SetPhaseCalcInfo();
    void SetPhaseNodeMeasInfo();
    void SetOffsetCalcInfo();
    void SetOffsetMeasInfo(int te, int tm);
    void offsetCorrectionHash2File();
    void readOffsetCorrectionFile();
    void SetSelfTestInfo(bool); // manuell oder remote
    int signum(double value);
    
    cMovingWindowFilter m_MovingWindowFilter;
    QTimer m_measureTimer;
    QTimer m_measureLPTimer;
    QTimer m_rangeTimer;
    
    void StopMeasurement();

    void SimulatedMeasurement();
    void CmpActFrequency();
    void CmpActValues();
    void CmpRMSValues();
    void CorrActValues();
    void CorrRMSValues();
    
    bool m_bJust;
    bool m_bStopped;
    bool m_bDspMeasureTriggerActive;
    bool m_bDspMeasureIgnore;
    tJustValues m_JustValues;
    cwmActValues ActValues;
    cDspMaxValues MaxValues;
    wmScopeDialog *mScopeDialog = nullptr;
    wmProgressDialog *mWmProgressDialog;
    std::unique_ptr<cJustMeasInfo> PhaseNodeMeasInfo;
    cJustMeasInfo* OffsetMeasInfo;
    /*wm3000ActionHandlerState*/ int m_PhaseNodeMeasState; // hier merken wir uns wo´s weiter geht
    /*wm3000ActionHandlerState*/ int m_OffsetMeasState; // hier merken wir uns wo´s weiter geht
    /*wm3000ActionHandlerState*/ int m_SelftestState; // hier merken wir uns wo´s weiter geht
    Q3ValueList<float> JustValueList;
    QHash<QString, double> adjOffsetCorrectionHash; // offset korrektur in lsb hier sammeln wir während des abgleiches
    QHash<QString, double> measOffsetCorrectionHash; // wenn abgleich durchgelaufen ist werden die werte hierhin kopiert
    QString m_sJustText;
   // QPushButton* m_pAbortButton;
    QString JDataFile;
    QFile m_SelftestLogfile;
    QFile m_PhaseJustLogfile;
    QFile m_OffsetJustLogfile;
    QFile m_OffsetDatafile;
    QFile m_NSAOffsetDatafile;
    cWMessageBox *m_OVLMsgBox;
    cWMessageBox *m_SelftestMsgBox;
    bool m_bConventional;
    bool m_bDC;
    bool m_bNewSamplerates;
    bool m_bNoDCAdjust;
    QString m_IpAddress;
    DspSetup m_dspSetup;
    WmWatchDogTimer m_wmwdt;
    void setupServers();
    void setupSampleDialog();
};

#endif    

