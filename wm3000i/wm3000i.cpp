// definition wm3000i

#include <qapplication.h>
#include <q3ptrlist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qfile.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qaction.h>
#include <q3mainwindow.h>
#include <qsize.h>
#include <qwidget.h>
#include <q3textbrowser.h>
#include <q3dockwindow.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <math.h>
#include <stdlib.h>
#include <qdom.h>
#include <qdatetime.h>
#include <q3valuelist.h>
#include <qfileinfo.h>

#include "wmviewbase.h"
#include "logfile.h"
#include "range.h"
#include "wm3000i.h"
#include "wmparameter.h"
#include "tools.h"
#include "maxperiods.h"
#include "scpiaffectatatuscode.h"
#include "scpioperationstates.h"
#include "scpiquestionstates.h"


extern WMViewBase *g_WMView;
extern char* MModeName[];

const double PI = 3.141592654;
const double PI_180 = 1.74532925e-2;

const float PhaseJustFreq[4] = {16.66, 30.0, 50.0, 60.0}; // feste frequenzwerte zur phasenjustage


cWMessageBox::cWMessageBox ( const QString & caption, const QString & text, Icon icon, int button0, int button1, int button2, QWidget * parent, const char * name , bool modal, Qt::WFlags f ) : QMessageBox ( caption, text, icon, button0, button1, button2, parent, name, modal, f)
{
}

cWMessageBox::~cWMessageBox()
{
}


void cWMessageBox::done(int r)
{
    emit WMBoxClosed();
    QMessageBox::done(r);
}


cWM3000I::cWM3000I() :
    m_ownError(new Wm3000iOwnErrorParamChecker)
{

    SerialVersions.DeviceVersion = WMVersion;
    SerialVersions.PCBSerialNr = "Unknown"; // wird ggf. später aus hardware gelesen
    SerialVersions.PCBVersion = "Unknown";
    SerialVersions.PCBServer = "Unknown";
    SerialVersions.DSPVersion = "Unknown";
    SerialVersions.DSPServer = "Unknown";
    SerialVersions.JDataChksum = "Unknown";

    // wenn device confdata sendet -> auch  an eigenfehler klasse
    connect(this,SIGNAL(SendConfDataSignal(cConfData*)), &m_ownError,SLOT(SetConfInfoSlot(cConfData*)));
    connect(this,SIGNAL(SendActValuesSignal(cwmActValues*)), &m_ownError,SLOT(SetActualValuesSlot(cwmActValues*)));
    connect(&m_ownError,SIGNAL(SendAnalizeDone(void)),this,SLOT(GetOETAnalizeDone(void)));
    m_sNRangeList.setAutoDelete( TRUE ); // the list owns the objects
    m_sNRangeList.append( new CWMRange("15.0A","15.0A",15.0,3595118,0.1,"N15.0A") ); //name,selectname,wert,aussteuerung, linearitätslimit 0.1 = 10%, Offsetkorr. key
    m_sNRangeList.append( new CWMRange("10.0A","10.0A",10.0,4793490,0.1,"N10.0A") );
    m_sNRangeList.append( new CWMRange("5.0A","5.0A",5.0,4793490,0.1,"N5.0A") );
    m_sNRangeList.append( new CWMRange("2.5A","2.5A",2.5,4793490,0.1,"N2.5A") );
    m_sNRangeList.append( new CWMRange("1.0A","1.0A",1.0,4793490,0.1,"N1.0A") );
    m_sNRangeList.append( new CWMRange("500mA","500mA",0.5,4793490,0.1,"N500mA") );
    m_sNRangeList.append( new CWMRange("250mA","250mA",0.25,4793490,0.1,"N250mA") );
    m_sNRangeList.append( new CWMRange("100mA","100mA",0.1,4793490,0.1,"N100mA") );
    m_sNRangeList.append( new CWMRange("50mA","50mA",0.05,4793490,0.1,"N50mA") );
    m_sNRangeList.append( new CWMRange("25mA","25mA",0.025,4793490,0.1,"N25mA") );
    m_sNRangeList.append( new CWMRange("10mA","10mA",0.01,3834792,0.1,"N10mA") );
    m_sNRangeList.append( new CWMRange("5mA","5mA",0.005,3834792,0.1,"N5mA") );
    m_sNRangeList.append( new CWMRange("Auto","Auto",-1.0,500000,0.1,"N15.0A") ); // sonderfall

    m_sXRangeList.setAutoDelete( TRUE ); // the list owns the objects
    m_sXRangeList.append( new CWMRange("15.0A","15.0A",15.0,3595118,0.1,"X15.0A") ); //name,selectname,wert,aussteuerung, linearitätslimit 0.1 = 10%
    m_sXRangeList.append( new CWMRange("10.0A","10.0A",10.0,4793490,0.1,"X10.0A") );
    m_sXRangeList.append( new CWMRange("5.0A","5.0A",5.0,4793490,0.1,"X5.0A") );
    m_sXRangeList.append( new CWMRange("2.5A","2.5A",2.5,4793490,0.1,"X2.5A") );
    m_sXRangeList.append( new CWMRange("1.0A","1.0A",1.0,4793490,0.1,"X1.0A") );
    m_sXRangeList.append( new CWMRange("500mA","500mA",0.5,4793490,0.1,"X500mA") );
    m_sXRangeList.append( new CWMRange("250mA","250mA",0.25,4793490,0.1,"X250mA") );
    m_sXRangeList.append( new CWMRange("100mA","100mA",0.1,4793490,0.1,"X100mA") );
    m_sXRangeList.append( new CWMRange("50mA","50mA",0.05,4793490,0.1,"X50mA") );
    m_sXRangeList.append( new CWMRange("25mA","25mA",0.025,4793490,0.1,"X25mA") );
    m_sXRangeList.append( new CWMRange("10mA","10mA",0.01,3834792,0.1,"X10mA") );
    m_sXRangeList.append( new CWMRange("5mA","5mA",0.005,3834792,0.1,"X5mA") );
    m_sXRangeList.append( new CWMRange("Auto","Auto",-1.0,500000,0.1,"X15.0A") ); // sonderfall

    // ende n bzw. dut (x)  kanäle anlegen
    
    // ect bereiche anlegen
    m_sECTRangeList.setAutoDelete( TRUE ); // the list owns the objects
    m_sECTRangeList.append( new CWMRange("15.0V","E15.0V",15.0,2831155,0.1,"ECT15.0V") );
    m_sECTRangeList.append( new CWMRange("10.0V","E10.0V",10.0,4718592,0.1,"ECT10.0V") );
    m_sECTRangeList.append( new CWMRange("5.0V","E5.0V",5.0,4718592,0.1,"ECT5.0V") );
    m_sECTRangeList.append( new CWMRange("2.5V","E2.5V",2.5,4718592,0.1,"ECT2.5V") );
    m_sECTRangeList.append( new CWMRange("1.0V","E1.0V",1.0,4718592,0.1,"ECT1.0V") );
    m_sECTRangeList.append( new CWMRange("500mV","E500mV",0.5,2995931,0.1,"ECT500mV") );
    m_sECTRangeList.append( new CWMRange("250mV","E250mV",0.25,2995931,0.1,"ECT250mV") );
    m_sECTRangeList.append( new CWMRange("100mV","E100mV",0.1,2995931,0.1,"ECT100mV") );
    m_sECTRangeList.append( new CWMRange("50mV","E50mV",0.05,2995931,0.1,"ECT50mV") );
    m_sECTRangeList.append( new CWMRange("25mV","E25mV",0.025,2995931,0.1,"ECT25mV") );
    //    m_sECTRangeList.append( new CWMRange("10mV","E10mV",0.01,2995931,0.1,"ECT10mV") );
    //    m_sECTRangeList.append( new CWMRange("5mV","E5mV",0.005,2995931,0.1,"ECT5mV") );
    m_sECTRangeList.append( new CWMRange("Auto","Auto",-1.0,500000,0.1,"ECT15.0V") ); // sonderfall
    // ende ect bereiche anlegen
    
    DummyRange = new CWMRange("Unknown","Unknown",-1.0,0,0.1,"Dummy");
    
    // default konfiguration setzen
    DefaultSettings(m_ConfData);
    m_binitDone = false; // system ist noch nicht initisalisiert

    LoadSettings(".ses"); // liess ev. mal die einstellungen
    
    m_ConfData.m_sRangeNVorgabe = "Auto";
    m_ConfData.m_sRangeXVorgabe = "Auto";
    m_ConfData.m_sRangeETVorgabe = "Auto";
    
    connect(&m_measureTimer, SIGNAL(timeout()), this, SLOT(MeasureSlot())); // aktivieren messung über timer
    connect(&m_rangeTimer, SIGNAL(timeout()),this, SLOT(RangeObsermaticSlot())); // aktivieren range observation + automatic über timer
    connect(&m_measureLPTimer, SIGNAL(timeout()), this, SLOT(MeasureLPSlot())); // aktivieren messung über timer
    
    // wir starten die timer erst nach erfolgreicher initialisierung ...oder doch nicht ?
    
    m_MovingWindowFilter.setFilterLength(m_ConfData.m_nIntegrationTime); // volle sekunden
    m_measureTimer.start(1*1000); //  ab movingwindowfilter immer 1*1000 msec
    m_rangeTimer.start(500);
    m_measureLPTimer.start(500);
    
    m_bStopped = false;

    m_bDspMeasureTriggerActive = false;
    m_bDspMeasureIgnore = false;
    
    m_ActTimer = new cSMTimer();
    connect(m_ActTimer,SIGNAL(timeout(int)),this,SLOT(ActionHandler(int)));
    m_AsyncTimer = new cSMTimer();
    connect(m_AsyncTimer,SIGNAL(timeout(int)),this,SLOT(ActionHandler(int)));
    connect(this,SIGNAL(StartStateMachine(int)),this,SLOT(ActionHandler(int)));
    
    if(!QDir(QString("%1/wm3000i/log/").arg(QDir::homePath())).exists())
    {
        //create temporary object that gets deleted when leaving the control block
        QDir().mkdir(QString("%1/wm3000i/").arg(QDir::homePath()));
        QDir().mkdir(QString("%1/wm3000i/log/").arg(QDir::homePath()));
    }

    m_SelftestLogfile.setName(QDir(SelftestLogFilePath).absPath());
    m_PhaseJustLogfile.setName(QDir(PhaseJustLogFilePath).absPath());
    m_OffsetJustLogfile.setName(QDir(OffsetJustLogFilePath).absPath());


    ActValues.RMSNSek = 0.0;  // wir benötigen definierte istwerte, damit wir die korrekturwerte
    ActValues.RMSXSek = 0.0; // lesen können
    ActValues.VekN = 0.0;
    ActValues.VekNSek = 0.0;
    ActValues.VekX = 0.0;
    ActValues.VekXSek = 0.0;
    ActValues.VekDX = 0.0;
    ActValues.VekDXSek = 0.0;
    ActValues.PHIN = 0.0;
    ActValues.PHIX = 0.0;
    ActValues.Frequenz = 50.0;
    
    m_OVLMsgBox = new cWMessageBox ( trUtf8("Übersteuerung"), trUtf8("Es ist eine Übersteuerung im grössten Bereich\naufgetreten. Bitte überprüfen Sie die Messgrössen"), QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, 0, 0, false ) ;
    connect(m_OVLMsgBox,SIGNAL(WMBoxClosed()),this,SLOT(OverLoadMaxQuitSlot()));
    m_SelftestMsgBox = new cWMessageBox ( trUtf8("Selbstest"), trUtf8("Test beendet\nDetails stehen im Logfile"), QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, 0, 0, false ) ;
    mWmProgressDialog = nullptr;
    connect(&m_wmwdt,SIGNAL(timeout()),this,SLOT(externalTriggerTimeoutTriggerd()));
}


cWM3000I::~cWM3000I() 
{
    WriteSettings(".ses");

    delete m_OVLMsgBox;
    delete m_SelftestMsgBox;
    delete m_ActTimer;
    delete m_AsyncTimer;
    delete DummyRange;

    if (mScopeDialog) delete mScopeDialog;
    delete DspIFace;
    delete PCBIFace;
}

void cWM3000I::setupServers(){

    // default TCP connection
    TCPConfig.pcbHost = TCPConfig.dspHost = m_IpAddress;
    TCPConfig.pcbPort = 6300;
    TCPConfig.dspPort = 6310;
    // ende default TCP connection

    DspIFace = new cDspIFace (m_ConfData.m_sADSPFile, TCPConfig.dspHost, TCPConfig.dspPort);
    connect(DspIFace,SIGNAL(iFaceAsync(const QString&)),this,SLOT(DspIFaceAsyncDataSlot(const QString&)));
    connect(DspIFace,SIGNAL(iFaceDone()),this,SLOT(XIFaceDoneSlot()));
    connect(DspIFace,SIGNAL(iFaceError()),this,SLOT(DspIFaceErrorSlot()));

    PCBIFace = new cPCBIFace(TCPConfig.pcbHost, TCPConfig.pcbPort);
    connect(PCBIFace,SIGNAL(iFaceDone()),this,SLOT(XIFaceDoneSlot()));
    connect(PCBIFace,SIGNAL(iFaceError()),this,SLOT(pcbIFaceErrorSlot()));

    m_dspSetup.setDspMemList(DspIFace);
}

void cWM3000I::setupSampleDialog()
{
    mScopeDialog->setChannelPointer(m_dspSetup.getMeasData()->RawValData0);
    mScopeDialog->setChannelPointer(m_dspSetup.getMeasData()->RawValData1);
    mScopeDialog->setChannelPointer(m_dspSetup.getMeasData()->RawValDataSinConHanning);
    mScopeDialog->setChannelPointer(m_dspSetup.getMeasData()->RawValData2);
    mScopeDialog->setChannelPointer(m_dspSetup.getMeasData()->RawValData3);
    mScopeDialog->updateBoxItems();
}

void cWM3000I::ActionHandler(int entryAHS)
{
    const char* SenseModeText[anzSenseMode] = {"sensNsensX", "adcNadcX", "sensNadcX", "sensXadcN", "sensNsensX0V"};

    static int AHS = wm3000Idle; // action handler state
    static bool bOverloadMax = false;
    static bool bOverloadMaxOld = false;
    static bool bOverload = false;
    static bool bHWOverload = true; // for initialisation set true
    static bool mustDo;
    static unsigned long lprogress;
    static int N;
    static int mCount;
    static cCalcInfo *PhaseCalcInfo;
    static cCalcInfo *OffsetCalcInfo;
    static cJustMeasInfo *OffsetMeasInfo;
    static float ph0,ph1;
    static float offs0, offs1;
    static complex SenseNVektor, ADCXVektor, SenseXVektor, ADCNVektor;

#ifdef DEBUG
    static int mRCount = 0;
#endif    

    if (entryAHS == EnterSimulationMode) { // wenn fehler aufgetreten und bediener sim. gewählt hat
        m_ConfData.m_bSimulation = true;
        emit SendConfDataSignal(&m_ConfData);
        AHSFifo.clear();
        if (mWmProgressDialog)
            delete mWmProgressDialog;
        m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
        AHS = wm3000Idle;
        return;
    }
    
    if ( (entryAHS != wm3000Continue) && (entryAHS != wm3000Repeat) ) { // wir sollen was neues starten
        if (AHS != wm3000Idle) { // sind aber noch nicht fertig
            if (AHSFifo.contains(entryAHS) ==0 ) // wir haben dieses event noch nicht in der liste
                AHSFifo.push_back(entryAHS); // dann merken wir uns was starten wir sollten
            return; // und sind fertig
        }
        else // oder wir sind fertig und
            AHS = entryAHS; // wir starten es
    }
    else // es ist continue oder repeat
    {
        if (entryAHS == wm3000Repeat)
            AHS--; // für repeat den state dekrementieren
    }

    switch (AHS)
    {

    case InitializationStart:
        StopMeasurement();
        m_binitDone = false; // wir initialisieren !!
        emit SendConfDataSignal(&m_ConfData);
        emit SendRangeListSignal(m_sNRangeList,m_sECTRangeList);
        AHS++;
        m_ActTimer->start(0,wm3000Continue); // wir starten uns selbst
        break; // InitializationStart

    case InitializationConnect2pcbHost:
        PCBIFace->connectYourself();
        AHS++;
        break; // InitializationConnect2pcbHost

    case InitializationOpenChannel0:
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
            m_ActTimer->start(10,RestartMeasurementStart); // falls wir simuliert gestartet sind ...
        }
        else
        {
            PCBIFace->openChannel(0); // kanal 0 öffnen
            AHS++;
        }
        break; // InitializationOpenChannel0

    case InitializationOpenChannel1:
        if (m_ConfData.m_bSimulation)
            AHS = wm3000Idle;
        else
        {
            PCBIFace->openChannel(1); // kanal 1 öffnen
            AHS++;
        }
        break; // InitializationOpenChannel1

    case SetRangeSetRanges:
    case ConfigurationSetRanges:
    case InitializationSetRanges:
        // abgleich der vorgaben
        if  ( m_ConfData.m_sRangeNVorgabe != QString("Auto") ) m_ConfData.m_sRangeNSoll = m_ConfData.m_sRangeNVorgabe;
        if  ( m_ConfData.m_sRangeXVorgabe != QString("Auto") ) m_ConfData.m_sRangeXSoll = m_ConfData.m_sRangeXVorgabe;
        if  ( m_ConfData.m_sRangeETVorgabe != QString("Auto") ) m_ConfData.m_sRangeETSoll = m_ConfData.m_sRangeETVorgabe;

        // das hier war vorher syncrange .... jetzt angepasst auf statemachine
        if (m_ConfData.m_bSimulation) { // wir sind durch fehler oder sonst wie in sim.
            m_ConfData.m_sRangeN = m_ConfData.m_sRangeNSoll; // wir tun so als ob alles gesetzt worden wäre
            m_ConfData.m_sRangeX = m_ConfData.m_sRangeXSoll;
            m_ConfData.m_sRangeET = m_ConfData.m_sRangeETSoll;
            emit SendConfDataSignal(&m_ConfData); // und teilen dies mit
            AHS = wm3000Idle;
        }
        else
        {
            m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft
            AHS++;
        }
        break; // InitializationSetRanges


    case RangeObsermaticSyncRange0:
    case SetRangeSyncRange0:
    case ConfigurationSyncRange0:
    case InitializationSyncRange0:
        if (bOverloadMax) // nach übersterung im grössten bereich kein bereiche schalten mehr
        {
            AHS+=6;
            m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft
        }
        else
        {
            PCBIFace->readRange(0); // bereich abfrage starten
            AHS++;
        }

        break; // InitializationSyncRange0


    case RangeObsermaticSyncRange01:
    case SetRangeSyncRange01:
    case ConfigurationSyncRange01:
    case InitializationSyncRange01:
    {
        cRSelectString rss;
        CWMRange* rng;

        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            rss = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            rng = Range(rss,m_sNRangeList); // sucht den bereich über den selektor aus der liste
            m_ConfData.m_sRangeN = rng->Name(); // istbereich
            if (m_ConfData.m_sRangeN != m_ConfData.m_sRangeNSoll)  // muss was getan werden?
            {
                StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
                PCBIFace->switchRange(0,Range(m_ConfData.m_sRangeNSoll,m_sNRangeList)->Selector());
            }
            else
                m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft wenn wir nichts gesendet haben
            AHS++;
        }
        break; // InitializationSwitchRange01
    }


    case RangeObsermaticSyncRange02:
    case SetRangeSyncRange02:
    case ConfigurationSyncRange02:
    case InitializationSyncRange02:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            m_ConfData.m_sRangeN = m_ConfData.m_sRangeNSoll; // bereiche sind jetzt gleich
            AHS++;
            m_ActTimer->start(0,wm3000Continue);
        }
        break; // InitializationSyncRange02


    case RangeObsermaticSyncRange1:
    case SetRangeSyncRange1:
    case ConfigurationSyncRange1:
    case InitializationSyncRange1:
        PCBIFace->readRange(1); // bereich abfrage starten
        AHS++;
        break; // InitializationSyncRange1


    case RangeObsermaticSyncRange11:
    case SetRangeSyncRange11:
    case ConfigurationSyncRange11:
    case InitializationSyncRange11:
    {
        cRSelectString rss;
        CWMRange* rng;

        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
            break; // InitializationSwitchRange1
        }
        else
        {
            rss = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            switch (m_ConfData.m_nMeasMode)
            {
            case In_IxDiff:
            case In_IxAbs:
                rng = Range(rss,m_sXRangeList); // sucht den bereich über den selektor aus der liste
                m_ConfData.m_sRangeX = rng->Name(); // istbereich
                if (m_ConfData.m_sRangeX != m_ConfData.m_sRangeXSoll)
                {
                    StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
                    PCBIFace->switchRange(1,Range(m_ConfData.m_sRangeXSoll,m_sXRangeList)->Selector());
                }
                else
                    m_ActTimer->start(0,wm3000Continue);
                break;
            case In_ECT:
                rng = Range(rss,m_sECTRangeList);
                m_ConfData.m_sRangeET = rng->Name();
                if (m_ConfData.m_sRangeET != m_ConfData.m_sRangeETSoll)
                {
                    StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
                    PCBIFace->switchRange(1,Range(m_ConfData.m_sRangeETSoll,m_sECTRangeList)->Selector());
                }
                else
                    m_ActTimer->start(0,wm3000Continue);
                break;
            case In_nConvent:
                if (rss != m_sNRangeList.first()->Selector())
                {
                    StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
                    PCBIFace->switchRange(1,m_sNRangeList.first()->Selector());
                }
                else
                    m_ActTimer->start(0,wm3000Continue);
                break;

            default :
                m_ActTimer->start(0,wm3000Continue);
            }
        }

        AHS++;
        break; //InitializationSyncRange11
    }


    case RangeObsermaticSyncRange12:
    case SetRangeSyncRange12:
    case ConfigurationSyncRange12:
    case InitializationSyncRange12:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            switch (m_ConfData.m_nMeasMode)
            {
            case In_IxDiff:
            case In_IxAbs:
                m_ConfData.m_sRangeX = m_ConfData.m_sRangeXSoll;
                break;
            case In_ECT:
                m_ConfData.m_sRangeET = m_ConfData.m_sRangeETSoll;
                break;
            default :
                break;
            }
            emit SendConfDataSignal(&m_ConfData); // wir informieren die anderen dass bereiche synchronisiert sind
            AHS++;
            m_ActTimer->start(0,wm3000Continue);
        }

        break; // InitializationSyncRange12

    case InitializationConnectDspIFace:
        // hier kein test auf sim nötig, weil wir nur m_ConfData sync. haben !!!
        DspIFace->connectYourself(); // interface verbindet sich mit seinem server
        AHS++;
        break; // InitializationConnect2dspHost

    case InitializationBootDsp:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->BootDsp();
            AHS++;
        }
        break; // InitializationBootDsp

    case InitializationBootDspPause:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            AHS++;
            m_ActTimer->start(50,wm3000Continue);
        }
        break; // InitializationBootDspPause

    case ConfigurationSetSamplingSystem:
    case InitializationSetSamplingSystem:
    {
        int nSPSP = 80;
        int nSPMP = 80;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            nSPSP = getSampleRate(m_ConfData.m_nSRate);
            nSPMP = nSPSP /* * m_ConfData.m_nMeasPeriod */;
            DspIFace->SetSamplingSystem(3, nSPSP, nSPMP); // messperiode = signalperiode ... dsp handelt die messperiode
            AHS++;
        }
        break; // InitializationSetSamplingSystem
    }

    case ConfigurationSetDspVarList:
    case InitializationSetDspVarList:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            mScopeDialog->clearChannelPointerList();
            SetDspWMVarList(); // dsp user variable definieren
            DspIFace->VarList2Dsp(); // und an den dsp senden
            setupSampleDialog();
            AHS++;
        }
        break; // InitializationSetDspVarList

    case ConfigurationSetDspCmdLists:
    case InitializationSetDspCmdLists:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SetDspWMCmdList(); // dsp kommando listen defineren
            DspIFace->CmdLists2Dsp(); // und senden
            AHS++;
        }
        break; // InitializationSetDspCmdList

    case ConfigurationActivateCmdLists:
    case InitializationActivateCmdLists:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->ActivateInterface();
            AHS++;
        }
        break; // InitializationActivateCmdLists

    case ConfigurationSetSamplingFrequency:
    case InitializationSetSamplingFrequency:
    {
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->setSamplingFrequency(m_ConfData.m_fSFreq);
            AHS++;
        }
        break; // InitializationSetSamplingFrequency
    }

    case ConfigurationSetSamplingPSamples:
    case InitializationSetSamplingPSamples:
    {
        int ps=80;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            ps = getSampleRate(m_ConfData.m_nSRate);
            PCBIFace->setSamplingPSamples(ps);
            AHS++;
        }
        break; // InitializationSetSamplingPSamples
    }


    case ConfigurationSetAbsDiffMode:
    case InitializationSetAbsDiffMode:
    {
        int tm;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            m_ConfData.m_nMeasMode == In_IxDiff ? tm = 1 : tm = 0;
            PCBIFace->setDiffAbsMode(tm);
            AHS++;
        }
        break; // InitializationSetTMode
    }


    case ConfigurationSetSenseMode:
    case InitializationSetSenseMode:
    {
        int sm;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            sm = m_ConfData.m_nSenseMode;
            PCBIFace->setTMode(sm);
            AHS++;
        }
        break; // InitializationSetSenseMode
    }

    case ConfigurationSetSyncSource:
    case InitializationSetSyncSource:
    {
        int sm;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfData.m_nSyncSource == Intern){
                sm = 1;
            }
            else {
                sm = 0;
                emit PPSQuestionable(false); // reset PPS Status indicator on external Trigger setting
            }
            PCBIFace->setSyncSource(sm);
            AHS++;
        }
        break; // InitializationSetSyncSource
    }

    case ConfigurationSetSyncTiming:
    case InitializationSetSyncTiming:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->setSyncTiming(m_ConfData.m_nTSync);
            AHS++;
        }
        break; // InitializationSetSyncTiming

    case ConfigurationSetDspSignalRouting:
    case InitializationSetDspSignalRouting:
    {
        ulong ethroute[64];
        int i;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            for (i = 0; i < 16; i++) ethroute[i] = 0;
            if (m_ConfData.m_nMeasMode == In_nConvent) // ersetzt kanal 1 daten durch eth. daten
            {
                int asdu;
                int n = m_ConfData.LastASDU - m_ConfData.FirstASDU +1;
                for (i = 0, asdu = m_ConfData.FirstASDU; i < n; i++, asdu++ )
                    ethroute[i >> 1] |= ((1 << 8) | (asdu << 4) | m_ConfData.DataSet) << ((1-(i&1))*16);
            }
            DspIFace->SetSignalRouting(ethroute);
            AHS++;
        }
        break; // InitializationSetDspSignalRouting
    }

    case ConfigurationSetDsp61850SourceAdr:
    case InitializationSetDsp61850SourceAdr:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->SetDsp61850SourceAdr(m_ConfData.m_MacSourceAdr);
            AHS++;
        }
        break; // InitializationSetDsp61850SourceAdr

    case ConfigurationSetDsp61850DestAdr:
    case InitializationSetDsp61850DestAdr:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->SetDsp61850DestAdr(m_ConfData.m_MacDestAdr);
            AHS++;
        }
        break; // InitializationSetDsp61850DestAdr

    case ConfigurationSetDsp61850PriorityTagged:
    case InitializationSetDsp61850PriorityTagged:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->SetDsp61850PriorityTagged(m_ConfData.m_nPriorityTagged);
            AHS++;
        }
        break; // InitializationSetDsp61850PriorityTagged

    case ConfigurationSetDsp61850EthTypeAppId:
    case InitializationSetDsp61850EthTypeAppId:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->SetDsp61850EthTypeAppId(m_ConfData.m_nEthTypeHeader);
            AHS++;
        }
        break; // InitializationSetDsp61850EthTypeAppId


    case ConfigurationSetDsp61850EthSynchronisation:
    case InitializationSetDsp61850EthSynchronisation:
    {
        ulong p = 0;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if ( m_ConfData.m_bStrongEthSynchronisation)
                p = m_ConfData.FirstASDU;

            DspIFace->SetDsp61850EthSynchronisation(p);
            AHS++;
        }
        break;
    }

    case SenseProtectionOffResetMaximum:
    case RangeObsermaticResetMaximum:
    case SetRangeResetMaximum:
    case InitializationResetMaximum:
    case RestartMeasurementResetMaximum:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            MaxValues.maxRdy = 0.0; // intern maxima gelöscht
            DspIFace->ResetMaximum();
            AHS++;
        }
        break;


    case RangeObsermaticResetOVL:
    case InitializationResetOVL:

        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfData.m_sRangeN == m_sNRangeList.first()->Selector()) // we only reset when we are in maximum range
                PCBIFace->resetStatusOVL(); // reset overload condition
            else
                m_ActTimer->start(0,wm3000Continue); // otherwise we must do something to continue

            AHS++;
        }

        break;

    case SenseProtectionOff:
    case InitializationProtectionOff:

        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->SetSenseProtection(0); // overloadmax -> schutzschaltung deaktivieren
            AHS++;
        }

        break;

    case InitializationReadPCBDeviceVersion:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->ReadDeviceVersion();
            AHS++;
        }
        break;

    case InitializationReadPCBServerVersion:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SerialVersions.PCBVersion = PCBIFace->iFaceSock->GetAnswer();
            PCBIFace->ReadServerVersion();
            AHS++;
        }
        break;

    case InitializationReadPCBSerialNr:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SerialVersions.PCBServer = PCBIFace->iFaceSock->GetAnswer();
            PCBIFace->ReadSerialNr();
            AHS++;
        }
        break;

    case InitializationReadDSPDeviceVersion:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SerialVersions.PCBSerialNr = PCBIFace->iFaceSock->GetAnswer();
            DspIFace->ReadDeviceVersion();
            AHS++;
        }
        break;

    case InitializationReadDSPServerVersion:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SerialVersions.DSPVersion = DspIFace->iFaceSock->GetAnswer();
            DspIFace->ReadServerVersion();
            AHS++;
        }
        break;

    case  InitializationReadJustdataChksum:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SerialVersions.DSPServer = DspIFace->iFaceSock->GetAnswer();
            PCBIFace->JustFlashGetChksum();
            AHS++;
        }
        break;

    case InitializationTestAdjustment:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            SerialVersions.JDataChksum = PCBIFace->iFaceSock->GetAnswer();
            emit SendVersionInfo(&SerialVersions);
            PCBIFace->GetAdjustmentStatus();
            AHS++;
        }
        break; // InitializationTestAdjustment

    case InitializationFinished:
    {
        QString s;
        int stat;
        if (m_ConfData.m_bSimulation) {
        }
        else
        {
            s = PCBIFace->iFaceSock->GetAnswer();
            stat = s.toInt();
            if ( stat  > 0) // nicht justiert
            {
                m_bJust = false;
                emit JustifiedSignal(false);
                emit AffectStatus(SetQuestStat, QuestNotJustified);
            }
            else
            {
                m_bJust = true;
                emit JustifiedSignal(true);
                emit AffectStatus(ResetQuestStat, QuestNotJustified);
            }


            m_binitDone = true; // wir hatten keinen fehler
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
        }

        AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
        break;
    }
        // konfigurations änderungen und zugehörige auszuführende aktionen
        // modus messung
        // 	bereiche neu setzen, justierwerte im dsp aktualisieren, neues dsp programm, ethroutingtab im dsp
        // signalfrequenz
        //	wenn nConventional neues dsp programm, messplatine init.(signal frequenz)
        // samples/periode
        //	neue dsp varliste, neues dsp programm, messplatine init. (samples/per), sampling system im dsp
        // messintervall
        //	neue dsp varliste, neues dsp program, sampling system im dsp
        // synchronisation
        //	messplatine init.(syncmode bzw. synctiming)
        // mac adressen
        //	ethsourcedest adr im dsp setzen
        // datensatz
        //	asdu u. dataset im dsp setzen (ethrouting)
        // integrationszeit
        //	Timer neu setzen

        // reihenfolge der aktionen wie in !!!! initWM3000 aber nicht alles und selektiv um traffic zu vermeiden
        //	bereiche, varlisten, cmdlisten, messplatine (signalfrequenz, samples/per, syncmode, synctiming)
        // 	signalrouting, mac adressen,
        //	integrationszeit


    case ConfigurationStart:
        m_binitDone = false; // wir konfigurieren
        emit AffectStatus(SetOperStat, OperConfiguring);
        if (m_ConfDataCopy.m_bSimulation && !m_ConfData.m_bSimulation) { // wenn programmkontrolle  -> /simulation
            InitWM3000(); // => alle Initialisierungen vornehmen
            AHS = wm3000Idle;
        }
        else
        {
            // bereiche beim konfigurieren immer setzen
            AHS++; // wir kommen in ConfigurationSetRanges !!!! die sind gleich mit Initialization !!
            m_ActTimer->start(0,wm3000Continue);
        }

        break; // ConfigurationStart

    case ConfigurationTestIntegrationTime: //  gestartet wird der timer wenn config fertig
        if (m_ConfDataCopy.m_nIntegrationTime != m_ConfData.m_nIntegrationTime)
        {
            StopMeasurement();
            AHS++;
        }

    case ConfigurationTestDspVarList:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if ( (m_ConfDataCopy.m_nSRate != m_ConfData.m_nSRate) ||
                 (m_ConfDataCopy.m_nMeasPeriod != m_ConfData.m_nMeasPeriod) )
            {
                StopMeasurement();
                AHS++; // dsp varlisten und sampling system im dsp
            }
            else
                AHS = ConfigurationTestCmdList;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDspVarList

    case ConfigurationTestCmdList:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if ( (m_ConfDataCopy.m_nSRate != m_ConfData.m_nSRate) ||
                 (m_ConfDataCopy.m_nMeasPeriod != m_ConfData.m_nMeasPeriod) ||
                 (m_ConfDataCopy.m_nMeasMode != m_ConfData.m_nMeasMode) ||
                 (fabs(m_ConfDataCopy.m_fSFreq - m_ConfData.m_fSFreq) > 1e-6) ||
                 (m_ConfDataCopy.m_bDCmeasurement != m_ConfData.m_bDCmeasurement) )
            {
                StopMeasurement();
                AHS++; // dsp cmdlisten setzen und übersetzen
            }
            else
                AHS = ConfigurationTestSignalFrequency;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestCmdList

    case ConfigurationTestSignalFrequency:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfDataCopy.m_fSFreq != m_ConfData.m_fSFreq)
            {
                StopMeasurement();
                AHS++; // messplatine signalfrequenz
            }
            else
                AHS = ConfigurationTestSRate;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestSignalFrequency

    case ConfigurationTestSRate:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfDataCopy.m_nSRate != m_ConfData.m_nSRate)
            {
                StopMeasurement();
                AHS++; // messplatine samples/periode
            }
            else
                AHS = ConfigurationTestAbsDiffMode;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestSRate


    case ConfigurationTestAbsDiffMode:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if ( (m_ConfDataCopy.m_nMeasMode != m_ConfData.m_nMeasMode) && ((m_ConfDataCopy.m_nMeasMode == In_IxDiff) || (m_ConfData.m_nMeasMode == In_IxDiff)))
            {
                StopMeasurement();
                AHS++; // messplatine samples/periode
            }
            else
                AHS = ConfigurationTestTMode;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestAbsDiffMode


    case ConfigurationTestTMode:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfDataCopy.m_nSenseMode != m_ConfData.m_nSenseMode)
            {
                StopMeasurement();
                AHS++; // messplatine test modus
            }
            else
                AHS = ConfigurationTestSyncMode;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestTMode

    case ConfigurationTestSyncMode:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfDataCopy.m_nSyncSource != m_ConfData.m_nSyncSource)
            {
                StopMeasurement();
                AHS++; // // messplatine syncmode
            }
            else
                AHS = ConfigurationTestSyncTiming;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestSyncMode

    case ConfigurationTestSyncTiming:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (m_ConfDataCopy.m_nTSync != m_ConfData.m_nTSync)
            {
                StopMeasurement();
                AHS++; // sync timing einstellen
            }
            else
                AHS = ConfigurationTestDspSignalRouting;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestSyncTiming

    case ConfigurationTestDspSignalRouting:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if ( (m_ConfDataCopy.m_nMeasMode != m_ConfData.m_nMeasMode)  ||
                 (m_ConfDataCopy.FirstASDU != m_ConfData.FirstASDU) ||
                 (m_ConfDataCopy.LastASDU != m_ConfData.LastASDU) ||
                 (m_ConfDataCopy.DataSet != m_ConfData.DataSet) )
            {
                StopMeasurement();
                AHS++; // signalrouting (ethrouting tab)
            }
            else
                AHS = ConfigurationTestDsp61850SourceAdr;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDspSignalRouting

    case ConfigurationTestDsp61850SourceAdr:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if   ( (m_ConfData.m_nMeasMode == In_nConvent) &&
                   ( m_ConfDataCopy.m_MacSourceAdr != m_ConfData.m_MacSourceAdr) )
            {
                StopMeasurement();
                AHS++; // mac adressen 61850 source setzen
            }
            else
                AHS = ConfigurationTestDsp61850DestAdr;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDsp61850SourceAdr

    case ConfigurationTestDsp61850DestAdr:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if   ( (m_ConfData.m_nMeasMode == In_nConvent) &&
                   ( m_ConfDataCopy.m_MacDestAdr != m_ConfData.m_MacDestAdr) )
            {
                StopMeasurement();
                AHS++; // mac adressen 61850 dest
            }
            else
                AHS = ConfigurationTestDsp61850PriorityTagged;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDsp61850DestAdr

    case ConfigurationTestDsp61850PriorityTagged:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if   ( (m_ConfData.m_nMeasMode == In_nConvent) &&
                   ( m_ConfDataCopy.m_nPriorityTagged != m_ConfData.m_nPriorityTagged) )
            {
                StopMeasurement();
                AHS++; // eth frame priority tagged
            }
            else
                AHS = ConfigurationTestDsp61850EthTypeAppId;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDsp61850PriorityTagged

    case ConfigurationTestDsp61850EthTypeAppId:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if   ( (m_ConfData.m_nMeasMode == In_nConvent) &&
                   ( m_ConfDataCopy.m_nEthTypeHeader != m_ConfData.m_nEthTypeHeader) )
            {
                StopMeasurement();
                AHS++; // eth frame eth type + appl. id
            }
            else
                AHS = ConfigurationTestDsp61850EthSynchronisation;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDsp61850EthTypeAppId

    case ConfigurationTestDsp61850EthSynchronisation:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if   ( (m_ConfData.m_nMeasMode == In_nConvent) &&
                   ( m_ConfDataCopy.m_bStrongEthSynchronisation != m_ConfData.m_bStrongEthSynchronisation) )
            {
                StopMeasurement();
                AHS++;
            }
            else
                AHS = ConfigurationFinished;

            m_ActTimer->start(0,wm3000Continue);
        }
        break; // ConfigurationTestDsp61850EthSynchronisation

    case ConfigurationFinished:
        m_wmwdt.setState(wmwdt_idle);
        if (m_ConfData.m_bSimulation) {
        }
        else
        {
            // integrationszeit starten wir in jedem fall neu nach konfiguration
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
            m_binitDone = true; //
            emit SendConfDataSignal(&m_ConfData); // andere objekte informieren
            // wenn's keine probleme gab sind wir hier angekommen und initialisiert
            emit ConfigReady(); // wir sync. eine ev. laufenden justage prozess
            emit AffectStatus(ResetOperStat, OperConfiguring);
            WriteSettings(".ses");
        }

        AHS = wm3000Idle;
        break; // case ConfigurationAct

    case SetRangeStart:
        emit AffectStatus(SetOperStat, OperRanging);
        AHS++; // es folgen bereich setzen und maximum rücksetzen
        m_ActTimer->start(0,wm3000Continue);
        break; // SetRangeStart

    case SetRangeFinished:
        if (m_ConfData.m_bSimulation) {
        }
        else
        {
            emit AffectStatus(ResetOperStat, OperRanging);
            emit SendConfDataSignal(&m_ConfData); // andere objekte informieren
            emit SetRangeReady(); // falls sich jemand sync. wollte
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
        }

        AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
        break; // SetRangeFinished

    case TriggerMeasureStart:
        m_wmwdt.setState(wmwdt_start);
        m_bDspMeasureTriggerActive = true; // der trigger ist aktiviert
        DspIFace->TriggerIntHKSK(1); // die intliste hksk = 1 triggern
        AHS++;
        break; // TriggerMeasureStart

    case  TriggerMeasureFinished:
        AHS = wm3000Idle; // wir sind mit oder ohne fehler fertig
        break;

    case MeasureDataAcquisition:
    case TriggerMeasureDataAcquisition:
        m_wmwdt.setState(wmwdt_meas);
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            // wieso das hier ??
            // die schnittstellenverbindung kann erst aufgebaut werden wenn das gerät läuft.
            // das interface wird dynamisch instanziiert nach aufbau der verbindung
            // justiert oder nicht justiert wird normalerweise nur einmal beim start emitted
            // und kommt daher dann nicht an. isn workaround, sollte später begradigt werden
            if (m_bJust)
                emit AffectStatus(ResetQuestStat, QuestNotJustified);
            else
                emit AffectStatus(SetQuestStat, QuestNotJustified);

            emit AffectStatus(ResetOperStat, OperMeasuring);
            DspIFace->DataAcquisition(m_dspSetup.getMeasData()->ActValData); // holt die daten ins dsp interface
            emit AffectStatus(SetOperStat, OperMeasuring);
            AHS++;
        }

        break; // TriggerMeasureDataAcquisition

    case MeasureComputation:
    case TriggerMeasureComputation:
        m_wmwdt.setState(wmwdt_compu);
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
        }
        else
        {
            float *source = DspIFace->data(m_dspSetup.getMeasData()->ActValData);
            float *dest = (float*) &ActValues.dspActValues;
            for (uint i=0; i< sizeof(ActValues.dspActValues)/sizeof(float);i++) *dest++ = *source++;

            m_MovingWindowFilter.append(ActValues.dspActValues);
            ActValues.dspActValues = m_MovingWindowFilter.getOutput();

            // die hanningfenster korrektur findet hier statt weil bei simulation auch cmpactvalues
            // aufgerufen wird, die simulationsdaten aber ohne hanningfenster sind... weil einfacher

            ActValues.dspActValues.rmsnf *= 1.63299; // hanning fenster korrektur
            ActValues.dspActValues.rmsxf *= 1.63299; // dito
            ActValues.dspActValues.ampl1nf *= 2.0; // dito
            ActValues.dspActValues.ampl1xf *= 2.0; // dito

            CmpActFrequency(); // die brauchen wir schon mal zum lesen der justagewerte
            AHS++;
            m_ActTimer->start(0,wm3000Continue);
        }

        break; // TriggerMeasureComputation

    case MeasureGetGainCorrCh0:
    case TriggerMeasureGetGainCorrCh0:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->readGainCorrection(0, Range(m_ConfData.m_sRangeN,m_sNRangeList)->Selector(), ActValues.RMSNSek);
            AHS++;
        }

        break; // TriggerMeasureGetGainCorrCh0

    case MeasureGetGainCorrCh1:
    case TriggerMeasureGetGainCorrCh1:
    {
        QString gcs;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            gcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.GainCorrCh0 = gcs.toFloat();

            switch ( m_ConfData.m_nMeasMode)
            {
            case In_IxDiff:
            case In_IxAbs:
                PCBIFace->readGainCorrection(1,  Range(m_ConfData.m_sRangeX,m_sXRangeList)->Selector(), ActValues.RMSXSek);
                break;
            case In_ECT:
            case In_nConvent: // wir lesen dies in jedem fall
                PCBIFace->readGainCorrection(1,  Range(m_ConfData.m_sRangeET,m_sECTRangeList)->Selector(), ActValues.RMSXSek);
            }
            AHS++;
        }

        break; // TriggerMeasureGetGainCorrCh1
    }

    case MeasureGetPhaseCorrCh0:
    case TriggerMeasureGetPhaseCorrCh0:
    {
        QString gcs;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            gcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.GainCorrCh1 = gcs.toFloat();

            PCBIFace->readPhaseCorrection(0, Range(m_ConfData.m_sRangeN,m_sNRangeList)->Selector(), ActValues.Frequenz);
            AHS++;
        }

        break; // TriggerMeasureGetPhaseCorrCh0
    }

    case MeasureGetPhaseCorrCh1:
    case TriggerMeasureGetPhaseCorrCh1:
    {
        QString pcs;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            pcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.PhaseCorrCh0 = pcs.toFloat();

            switch ( m_ConfData.m_nMeasMode )
            {
            case In_IxDiff:
            case In_IxAbs:
                PCBIFace->readPhaseCorrection(1, Range(m_ConfData.m_sRangeX,m_sXRangeList)->Selector(), ActValues.Frequenz);
                break;
            case In_ECT:
            case In_nConvent:
                PCBIFace->readPhaseCorrection(1, Range(m_ConfData.m_sRangeET,m_sECTRangeList)->Selector(), ActValues.Frequenz);
            }
            AHS++;
        }

        break; // TriggerMeasureGetPhaseCorrCh1
    }

    case MeasureGetOffsetCorrCh0:
    case TriggerMeasureGetOffsetCorrCh0:
    {
        QString pcs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            pcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.PhaseCorrCh1 = pcs.toFloat();

            PCBIFace->readOffsetCorrection(0, Range(m_ConfData.m_sRangeN,m_sNRangeList)->Selector(), ActValues.RMSNSek);
            AHS++;
        }

        break; // TriggerMeasureGetOffsetCorrCh0
    }


    case MeasureGetOffsetCorrCh1:
    case TriggerMeasureGetOffsetCorrCh1:
    {
        QString ocs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            ocs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.OffsetCorrCh0 = ocs.toFloat();

            switch (m_ConfData.m_nMeasMode)
            {
            case In_IxDiff:
            case In_IxAbs:
                PCBIFace->readOffsetCorrection(1, Range(m_ConfData.m_sRangeX,m_sNRangeList)->Selector(), ActValues.RMSXSek);
                break;
            default:
                PCBIFace->readOffsetCorrection(1, Range(m_ConfData.m_sRangeET,m_sECTRangeList)->Selector(), ActValues.RMSXSek);
                break;
            }
            AHS++;
        }

        break; // TriggerMeasureGetOffsetCorrCh1
    }


    case MeasureCorrection:
    case TriggerMeasureCorrection:
    {
        QString ocs;
        if (m_ConfData.m_bSimulation)
        {
        }
        else
        {
            ocs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.OffsetCorrCh1 = ocs.toFloat();

            CorrActValues();
            CmpActValues();
            emit SendActValuesSignal(&ActValues);
            emit MeasureReady();
        }

        AHS = wm3000Idle; // wir sind so oder so fertig
        break; // TriggerMeasureCorrection
    }


    case RestartMeasurementStart:
        if (m_ConfData.m_bSimulation) // im sim. modus wir der meastimer direkt gestartet
        {
            m_MovingWindowFilter.setFilterLength(m_ConfData.m_nIntegrationTime);
            m_measureTimer.start(1*1000); //  ab movingwindowfilter immer 1*1000 msec
            m_measureLPTimer.start(500);
            m_rangeTimer.start(500);
            m_bDspMeasureTriggerActive = false;
            m_bStopped = false;
            AHS = wm3000Idle; // wir sind schon fertig
        }
        else
        {
            AHS++;
            m_ActTimer->start(0,wm3000Continue);
        }

        break;

    case RestartMeasurementRestart:
        if (m_ConfData.m_bSimulation) // fehler
            AHS = wm3000Idle;
        else
        {
            // im real modus wird das filter im dsp gelöscht und darauf synchroniert der meastimer gestartet
            if (m_bStopped) // es kann sein dass wir restart aufrufen obwohl wir gar nicht gestoppt haben
            {
                m_bStopped = false;
                DspIFace->TriggerIntHKSK(3); // die intliste hksk = 3 triggern löscht das filter im dsp
                AHS++;
            }
            else
                AHS = wm3000Idle;
        }
        break; // RestartMeasurementRestart

    case RestartMeasurementFinished:
        AHS = wm3000Idle; // ob fehler oder nicht .... wir haben fertig
        break;

    case MeasureStart:
        if (m_bStopped)
            AHS = wm3000Idle;
        else
            if (m_bDspMeasureIgnore) // falls zwischen durch was geschaltet oder konf. wurde
            {
                m_bDspMeasureIgnore = false; // verwerfen wir die messergebnisse
                AHS = wm3000Idle;
            }
            else
            {
                emit AffectStatus(SetOperStat, OperMeasuring);
                AHS++; // es folgen measuredatacquisition,  measurecomputation
                m_ActTimer->start(0,wm3000Continue);
            }
        break; // MeasureStart


    case MeasureLPStart:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            DspIFace->DataAcquisition(m_dspSetup.getMeasData()->RMSValData); // holt die daten ins dsp interface
            AHS++;
        }
        break; // MeasureLPStart


    case MeasureLPComputation:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
        }
        else
        {
            float *source = DspIFace->data(m_dspSetup.getMeasData()->RMSValData);
            float *dest = (float*) &ActValues.dspRMSValues;
            for (uint i=0; i< sizeof(ActValues.dspRMSValues)/sizeof(float);i++) *dest++ = *source++;

            ActValues.dspRMSValues.fastRMSN *= 1.63299; // hanning fenster korrektur
            ActValues.dspRMSValues.fastRMSN1 = ActValues.dspRMSValues.fastRMSN1 * 2.0 / 1.41421356; // korrektur einheitswurzeln, ammplitude->rms
            ActValues.dspRMSValues.fastRMSX *= 1.63299; // hanning fenster korrektur
            ActValues.dspRMSValues.fastRMSX1 = ActValues.dspRMSValues.fastRMSX1 * 2.0 / 1.41421356; // korrektur einheitswurzeln, ammplitude->rms

            CorrRMSValues();
            CmpRMSValues();
            emit SendLPSignal(&ActValues);

            AHS = wm3000Idle; // fertig
        }

        break; // MeasureLPComputation


    case RangeObsermaticStart:
#ifdef DEBUG
        mRCount++;
        qDebug("RangeObsermatic Start %d", mRCount);
#endif	
        PCBIFace->getStatusOVL(); // query des overload status der hardware
        AHS++;
        break;

    case RangeObsermaticTestHWOVL:
    {
        QString ovl;
        ovl  = PCBIFace->iFaceSock->GetAnswer();
        if ( (bHWOverload = ( ovl == "1")) == true ) // wir haben hw übersteuerung
        {
#ifdef DEBUG
            qDebug("RangeObsermatic HW Overload");
#endif	
        }

#ifdef DEBUG
        qDebug("RangeObsermatic Get MaxData");
#endif

        DspIFace->DataAcquisition(m_dspSetup.getMeasData()->MaxValData); // holt die daten ins dsp interface
        AHS++;
        break;

    }

    case RangeObsermaticGetMaxData:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            float *source = DspIFace->data(m_dspSetup.getMeasData()->MaxValData);
            float *dest = (float*) &MaxValues;
            for (uint i=0; i< sizeof(MaxValues)/sizeof(float);i++) *dest++ = *source++;
            AHS++;
            m_ActTimer->start(0,wm3000Continue);
        }

        break;


    case RangeObsermaticTest:
    {
        mustDo = false;
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if ((MaxValues.maxRdy > 0) || bHWOverload) { // dann müssen wir was tun
                bool bOvln,  bOvlx;
                // egal ob maxrdy oder hwoverload n muss bearbeitet werden
                if (bHWOverload) // wenn
                {
                    bOvln = true;
                    MaxValues.maxn = 0x800000;
                }
                else
                    if (MaxValues.maxRdy > 0)
                        bOvln = TestOverload( m_sNRangeList, m_ConfData.m_sRangeN, MaxValues.maxn);

                switch (m_ConfData.m_nMeasMode)
                {
                case In_IxDiff:
                case In_IxAbs:
                    if (bHWOverload) // egal ob maxrdy oder hwoverload in dieser betriebsart muss xauch bearbeitet werden
                    {
                        bOvlx =true;
                        MaxValues.maxx = 0x800000;
                    }
                    else
                        if (MaxValues.maxRdy > 0)
                            bOvlx = TestOverload( m_sXRangeList, m_ConfData.m_sRangeX, MaxValues.maxx);
                    bOvln = bOvlx = bOvln || bOvlx; // in dieser betriebsart wenn einer dann beide !
                    break;
                case In_ECT:
                    if (MaxValues.maxRdy > 0) // in dieser betriebsart dürfen wir nur testen wenn gültige max data vorhanden waren
                        bOvlx = TestOverload(m_sECTRangeList, m_ConfData.m_sRangeET, MaxValues.maxx);
                    else
                        bOvlx = false;
                    break;
                default :
                    bOvlx = false;
                    break;
                }

                // wenn ein bereich soft übersteuert ist und wir im n/x mode sind tun wir so als wären beide übersteuert
                // warum ?
                // abfrage hard übersteuerung hat ergeben :
                // keine übersteuerung der maximum sucher einer der beiden kanäle hat
                // eine übersteuerung erkannt, also wird der kanal in den grössten bereich geschaltet.
                // im anschluss wird reset hard übersteuerung ausgegeben. die prüfgrösse liegt aber
                // u.U. noch an. also gibts nochmal übersteuerung, jetzt aber im anderen kanal.
                // es gibt aber nur eine hard ü-meldung für beide kanäle. damit ist jetzt der kanal der
                // schon hochgeschaltet wurde, vermeintlich im grössten bereich übersteuert.


                bOverloadMaxOld = bOverloadMax;

#ifdef DEBUG
                qDebug("RangeObsermatic overload=%s", bOverload?"1":"0");
#endif		    
                if ( bOvln || (MaxValues.maxRdy > 0) ) // wir hatten eine übersteuerung egal wie detektiert oder haben neue max daten
                    mustDo |= SelectRange( m_sNRangeList, m_ConfData.m_sRangeN, m_ConfData.m_sRangeNSoll, m_ConfData.m_sRangeNVorgabe, MaxValues.maxn, bOvln);
#ifdef DEBUG
                qDebug("RangeObsermatic overload=%s RangeN=%s", bOvln?"1":"0",m_ConfData.m_sRangeN.latin1());
#endif	

                switch (m_ConfData.m_nMeasMode)
                {
                case In_IxDiff:
                case In_IxAbs:
                    if ( bOvlx || (MaxValues.maxRdy > 0) )
                        mustDo |= SelectRange( m_sXRangeList, m_ConfData.m_sRangeX, m_ConfData.m_sRangeXSoll, m_ConfData.m_sRangeXVorgabe, MaxValues.maxx, bOvlx);
#ifdef DEBUG
                    qDebug("RangeObsermatic overload=%s RangeX=%s", bOvlx?"1":"0",m_ConfData.m_sRangeX.latin1());
#endif	
                    bOverloadMax = ( bOvln && (m_ConfData.m_sRangeN == m_sNRangeList.first()->Name()) ) && ( bOvlx && (m_ConfData.m_sRangeX == m_sXRangeList.first()->Name()) );
                    break;
                case In_ECT:
                    if ( bOvlx || (MaxValues.maxRdy > 0) )
                        mustDo |= SelectRange(m_sECTRangeList, m_ConfData.m_sRangeET, m_ConfData.m_sRangeETSoll, m_ConfData.m_sRangeETVorgabe, MaxValues.maxx, bOvlx);
                    bOverloadMax = ( bOvln && (m_ConfData.m_sRangeN == m_sNRangeList.first()->Name())) || ( bOvlx && (m_ConfData.m_sRangeET == m_sECTRangeList.first()->Name()) );
                    break;

                default :
                    bOverloadMax = ( bOvln && (m_ConfData.m_sRangeN == m_sNRangeList.first()->Name()) );
                    break;
                }

                if ( (bOverload = (bOvln || bOvlx)) == true )
                    emit AffectStatus(SetQuestStat, QuestOverLoad); // und an status system melden
                else
                    emit AffectStatus(ResetQuestStat, QuestOverLoad);

                if (mustDo)
                    AHS++; //  weiter mit bereiche synchronisieren
                else
                    AHS = RangeObsermaticOverload; // bzw. direkt overload behandlung

                m_ActTimer->start(0,wm3000Continue);
            }
            else
            {
                AHS = wm3000Idle; // keine gültigen daten -> fertig
            }
        }
        break; // case RangeObsermaticTest
    }


    case RangeObsermaticOverload:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            if (bOverloadMax && (!bOverloadMaxOld) ) // nur  nach positiver flanke !!!
            {
                m_OVLMsgBox->show();
                emit AffectStatus(SetQuestStat, QuestOverLoadMax);
                PCBIFace->SetSenseProtection(1); // overloadmax -> schutzschaltung aktivieren
#ifdef DEBUG
                StopMeasurement(); // wir hören hier im debug fall auf, so können wir
                AHS = wm3000Idle;  // uns die history auf dem xterm ansehen

#endif		
            }
            else
                m_ActTimer->start(0,wm3000Continue); // rangeobsermaticresetmaximum

            AHS++;
        }

        break;

        //  case RangeObsermaticResetMaximum: // weiter oben realisiert !!!!

        // RangeObsermaticResetOVL findet hier statt

    case RangeObsermaticFinished:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
            emit RangeAutomaticDone();
            AHS = wm3000Idle; // egal ob fehler oder nicht -> obsermatic ist fertig
        }

        break;


        // case SenseProtectionOff weiter oben realisiert (gleich wie init....)

    case SenseProtectionOffRM:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            StopMeasurement(); // messung stop
            AHS++;
            m_ActTimer->start(0,wm3000Continue); // rangeobsermaticresetmaximum
        }

        break;

        // maxima rücksetzten findet hier statt //

    case SenseProtectionOffFinished:
        if (m_ConfData.m_bSimulation) {
            AHS = wm3000Idle;
        }
        else
        {
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
            bOverloadMax = false; // wir nehmen die übersteuerung zurück -> ab jetzt können wieder bereiche geschaltet werden
            AHS = wm3000Idle;
        }

        break;


    case CmpPhaseCoeffStart:
        StopMeasurement(); // die kumulieren jetzt nur
        mWmProgressDialog = new wmProgressDialog( trUtf8("Berechnung läuft ..."), 0, 4/*m_PhaseCalcInfoList.count()*/, g_WMView); // ein progress dialog
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));
        mWmProgressDialog->setCaption("Phasenkorrekturkoeffizienten"); //überschrift
        lprogress = 0; // int. progress counter
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break; // CmpPhCoeffStart


    case CmpPhaseCoeffCh0:
        lprogress++;
        mWmProgressDialog->setValue(lprogress);
        AHS++;
        m_ActTimer->start(3000,wm3000Continue);
        break;

    case CmpPhaseCoeffCh1:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            PCBIFace->cmpPhaseCoefficient("ch0"); // phasenkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpPhaseCoeffCh2:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            PCBIFace->cmpPhaseCoefficient("ch1"); // phasenkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpPhaseSetStatus:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            PhaseCalcInfo = m_CalcInfoList.first();
            PCBIFace->setPhaseStatus(PhaseCalcInfo->m_sChannel, PhaseCalcInfo->m_sRange, 1); // wir setzen den status auf justiert
            AHS++;
        }
        break; // CmpPhaseSetStatus

    case CmpPhaseSetStatus2:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            delete m_CalcInfoList.first();
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                AHS++; // dann sind wir fertig
            }
            else
            {
                AHS--; // sonst zum nächsten
            }
            m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
        }
        break; // CmpPhaseSetStatus2

    case CmpPhaseCoeffFinished:
    case CmpOffsetCoeffFinished:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
            delete mWmProgressDialog;
            AHS = wm3000Idle;
        }
        break; // CmpPhCoeffFinished

    case CmpOffsetCoeffStart:
        StopMeasurement(); // die kumulieren jetzt nur
        //m_pProgressDialog = new QProgressDialog( trUtf8("Berechnung läuft ..."), 0, 4, g_WMView, 0, FALSE, 0 ); // ein progress dialog
        mWmProgressDialog = new wmProgressDialog( trUtf8("Berechnung läuft ..."), 0, 4, g_WMView); // ein progress dialog
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));
        mWmProgressDialog->setCaption("Offsetkorrekturkoeffizienten"); //überschrift
        lprogress = 0; // int. progress counter
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break; // CmpOffsetCoeffStart

    case CmpOffsetCoeffCh0:
        lprogress++;
        mWmProgressDialog->setValue(lprogress);
        AHS++;
        m_ActTimer->start(3000,wm3000Continue);
        break;

    case CmpOffsetCoeffCh1:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            PCBIFace->cmpOffsetCoefficient("ch0"); // offsetkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpOffsetCoeffCh2:
        if (m_ConfData.m_bSimulation)
        {   // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            PCBIFace->cmpOffsetCoefficient("ch1"); // offsetkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpOffsetSetStatus:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            OffsetCalcInfo = m_CalcInfoList.first();
            PCBIFace->setOffsetStatus(OffsetCalcInfo->m_sChannel, OffsetCalcInfo->m_sRange, 1); // wir setzen den status auf justiert
            AHS++;
        }
        break; // CmpoffsetSetStatus

    case CmpOffsetSetStatus2:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            delete m_CalcInfoList.first();
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                AHS++; // dann sind wir fertig
            }
            else
            {
                AHS--; // sonst zum nächsten
            }
            m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
        }
        break; // CmpOffsetSetStatus2


    case PhaseNodeMeasStart:
        m_PhaseJustLogfile.remove(); // beim starten wird das log file gelöscht
        StopMeasurement(); // die kumulieren jetzt nur
        mWmProgressDialog = new wmProgressDialog(g_WMView);
        mWmProgressDialog->setLabelText( trUtf8("Koeffizienten 0 setzen ..."));
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));
        mWmProgressDialog->setMinMaxValue(0, m_PhaseNodeMeasInfoList.size(),0,4,0,4);
        mWmProgressDialog->setTitel(trUtf8("Phasenkorrekturkoeffizienten"));

        lprogress = 0; // int. progress counter
        mWmProgressDialog->setValue(lprogress);
        QObject::connect(mWmProgressDialog,SIGNAL(aborted()),this,SLOT(JustAbortSlot()));
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        N = 0; // durchlaufzähler
        break; // PhaseNodeMeasStart

    case PhaseNodeMeasCoefficientClearNode:
        PhaseCalcInfo = m_CalcInfoList.first();
        PCBIFace->setPhaseNodeInfo(PhaseCalcInfo->m_sChannel, PhaseCalcInfo->m_sRange, 0, 0.0, 0.0); // wir setzen jeweils die 1. stützstelle und lassen im anschluss daran die koeffizienten berechnen
        AHS++;
        break; // PhaseCoefficientClearNode

    case PhaseNodeMeasCoefficientClearStatus:
        PhaseCalcInfo = m_CalcInfoList.first();
        PCBIFace->setPhaseStatus(PhaseCalcInfo->m_sChannel, PhaseCalcInfo->m_sRange, 0);
        AHS++;
        break;

    case PhaseNodeMeasCoefficientClearNodeStatus:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            delete m_CalcInfoList.first();
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                if (!mWmProgressDialog->isAbort()) //  m_pAbortButton->isEnabled())
                {
                    PCBIFace->cmpPhaseCoefficient("ch0"); // berechnung der koeffizienten
                    AHS++;
                }
                else
                {
                    m_ActTimer->start(0,RestartMeasurementStart); // die hatten wir gestoppt
                    AHS = wm3000Idle;
                    delete mWmProgressDialog;
                    m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
                }

            }
            else
            {
                AHS--;
                AHS--;
                m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
            }
        }
        break;

    case PhaseNodeMeasCoefficientClearNFinished:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->cmpPhaseCoefficient("ch1"); // berechnung der koeffizienten
            AHS++;
        }
        break;

    case PhaseNodeMeasBaseConfiguration:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
            break;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            NewConfData = m_ConfData; // zum umsetzen
            SaveConfData = m_ConfData; // wir haben eine kopie der aktuellen konfiguration
            NewConfData.m_bOECorrection = false; // nix korrigieren
            NewConfData.m_fxPhaseShift = 0.0; // auch hier keine korrekturen
            NewConfData.m_fxTimeShift = 0.0; // dito
            NewConfData.m_nSyncSource = Intern; // es muss intern synchronisiert sein
            NewConfData.m_nTSync = 1000; // 1 sec. ist ok
            NewConfData.m_nIntegrationTime = 1; // 1 sec.
            NewConfData.m_nMeasPeriod = 8; //
            N = 0; // 1. stützstelle
            AHS++;
            // kein break wir laufen einfach drüber
        }

    case PhaseNodeMeasNodeConfig:
    {
        StopMeasurement(); // das kumuliert nur ....
        mWmProgressDialog->setLabelText (trUtf8("Konfiguration setzen ..." ));
        //PhaseNodeMeasInfo = m_PhaseNodeMeasInfoList.first(); // info was zu tun ist
        mWmProgressDialog->setValue2(N);
        mWmProgressDialog->setValue(lprogress);
        if (N == 0) PhaseNodeMeasInfo = std::move(m_PhaseNodeMeasInfoList.at(lprogress-1)); // info was zu tun ist

        if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
        {
            QTextStream stream( &m_PhaseJustLogfile );
            stream << QString("RangeN=%1 RangeX=%2 Mode=").arg(PhaseNodeMeasInfo->m_srngN).arg(PhaseNodeMeasInfo->m_srngX);
            switch (PhaseNodeMeasInfo->m_nMMode)
            {
            case In_IxAbs:
                stream << "In/Ix ";
                break;
            case In_ECT:
                stream << "In/ECT ";
                break;
            default:
                stream << "some Error in program ";
                break;
            }

            stream << QString("SenseMode=%1 nS=").arg(SenseModeText[PhaseNodeMeasInfo->m_nSMode]);
            if (PhaseNodeMeasInfo->m_nnS == S80)
                stream << "80\n";
            else
                stream << "256\n";

            m_PhaseJustLogfile.flush();
            m_PhaseJustLogfile.close();
        }

        NewConfData.m_nSenseMode = PhaseNodeMeasInfo->m_nSMode & 3; // test modus
        NewConfData.m_nSRate = PhaseNodeMeasInfo->m_nnS; // samples / periode
        NewConfData.m_fSFreq = PhaseJustFreq[N];
        NewConfData.m_nMeasMode  =  PhaseNodeMeasInfo->m_nMMode;
        NewConfData.m_sRangeNVorgabe = PhaseNodeMeasInfo->m_srngN; // bereich kanal n
        if (PhaseNodeMeasInfo->m_nMMode == In_IxAbs)
            NewConfData.m_sRangeXVorgabe = PhaseNodeMeasInfo->m_srngX; // bereich kanal x
        else
            NewConfData.m_sRangeETVorgabe = PhaseNodeMeasInfo->m_srngX; // bereich kanal x
        m_PhaseNodeMeasState = PhaseNodeMeasExec1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(PhaseJustSyncSlot()));
        SetConfDataSlot(&NewConfData); // und die neue konfiguration
        // die messung wird neu gestartet am ende der konfiguration
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // PhaseNodeMeasNodeConfig
    }

    case PhaseNodeMeasExec1: // konfiguriert ist
        NewConfData.m_sRangeNSoll = NewConfData.m_sRangeN =NewConfData.m_sRangeNVorgabe; // bereich kanal n
        NewConfData.m_sRangeXSoll = NewConfData.m_sRangeX = NewConfData.m_sRangeXVorgabe; // bereich kanal x
        NewConfData.m_sRangeETSoll = NewConfData.m_sRangeET = NewConfData.m_sRangeETVorgabe; // bereich kanal x
        m_PhaseNodeMeasState = PhaseNodeMeasExec2; // hier müssen wir später weitermachen
        mCount = PhaseNodeMeasInfo->m_nIgnore; // einschwingzeit setzen in messdurchläufen
        m_sJustText = trUtf8("Einschwingzeit läuft" );
        mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        mWmProgressDialog->setMinMax2nd(0,4);
        mWmProgressDialog->setMinMax3rd(0,4);
        mWmProgressDialog->setValue3(mCount);
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(PhaseJustSyncSlot()));
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // PhaseNodeMeasExec1

    case PhaseNodeMeasExec2:
        mCount--;
        mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        mWmProgressDialog->setValue3(mCount);
        if (mWmProgressDialog->isAbort()) {
            AHS = PhaseNodeMeasExec5;
            N=4;
            m_ActTimer->start(0,wm3000Continue);
            break;
        }

        if (mCount == 0) { // eingeschwungen
            m_PhaseNodeMeasState = PhaseNodeMeasExec3; // ab jetzt messen wir wirklich
            mCount = PhaseNodeMeasInfo->m_nnMeas; // und setzen den zähler dafür
            mWmProgressDialog->setMinMax3rd(0,mCount);
            switch (PhaseNodeMeasInfo->m_nJMode)
            { // geht anders .... aber ist so übersichtlicher
            case adcNPhase:
                m_sJustText = trUtf8("Messung Kanal N, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            case adcXPhase:
                m_sJustText = trUtf8("Messung Kanal X, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            case sensNadcXPhase:
                m_sJustText = trUtf8("Messung Kanal N, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            case sensXadcNPhase:
            case sensECTadcNPhase:
                m_sJustText = trUtf8("Messung Kanal X, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            default:
                break;
            }
            mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
            mWmProgressDialog->setValue3(mCount);

            JustValueList.clear(); // phasenwinkel werte liste leeren, zur aufnahme der neuen messwerte
        }

        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(PhaseJustSyncSlot()));  // wieder neu verbinden
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; //	PhaseNodeMeasExec2


    case PhaseNodeMeasExec3:
    {
        int i;
        ph0 = ActValues.PHIN;
        ph1 = ActValues.PHIX;
        if (mWmProgressDialog->isAbort()) {
            AHS = PhaseNodeMeasExec5;
            N=4;
            m_ActTimer->start(0,wm3000Continue);
            break;
        }

        switch (PhaseNodeMeasInfo->m_nJMode)
        {
        case adcNPhase:
            break; // wir wollten den kanal n adc messen ->fertig
        case adcXPhase:
            ph0 = ph1;
            break; // wir wollten kanal x adc messen
        case sensNadcXPhase:
            ph0 = ph0 - (ph1 -PI); // -PI ... weil sensorik so ist
            break; // wir wollten kanal n sense messen
        case sensXadcNPhase:
            ph0 = ph1 - (ph0 - PI); // -PI ... wat wohl sonst
            break;
        case sensECTadcNPhase:
            ph0 = ph1 - ph0;
            break; // wir wollten kanal x bzw. ect sense messen
        default:
            break;
        }

        ph0 *= 57.295779; // 360/(2*PI) winkel sind im bogenmass
        JustValueList.append(ph0); // wir schreiben den winkel wert in die liste

        mCount--;
        mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        mWmProgressDialog->setValue3(mCount);
        if (mCount == 0)
        {
            qreal min(359.99), max(0.0), diff(0.0), value(0.0);
            mWmProgressDialog->setLabelText (trUtf8("Berechnung und Datenübertragung ..."));
            mWmProgressDialog->set2ndDisabled();
            mWmProgressDialog->set3rdDisabled();
            ph0 = 0.0;
            if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
            {
                QTextStream stream( &m_PhaseJustLogfile );
                for (i = 0; i < JustValueList.count(); i++){
                    value = JustValueList[i];
                    stream << "Ph0: " << value << "\n" ;
                    if( min > value) min = value;
                    if (max < value) max = value;
                    ph0 -= value;
                }
                diff = (max - min) * 60.0;
                stream << "Diff: " << diff << " Minuten \n";
                m_PhaseJustLogfile.flush();
                m_PhaseJustLogfile.close();
            }
            ph0 /= JustValueList.count(); // der mittelwert aus den messungen
            AHS = PhaseNodeMeasExec4;
            m_ActTimer->start(0,wm3000Continue); // wir starten wieder selbst
        }
        else
        {
            QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(PhaseJustSyncSlot()));
            AHS = wm3000Idle; // wir werden von der nächsten messung getriggert
        }

        break; // PhaseNodeMeasExec3
    }


    case PhaseNodeMeasExec4:
    {
        if (mWmProgressDialog->isAbort()) {
            AHS = PhaseNodeMeasExec5;
            N=4;
            m_ActTimer->start(0,wm3000Continue);
            break;
        }

        switch (PhaseNodeMeasInfo->m_nJMode)
        {
        case adcNPhase:
        case sensNadcXPhase:
            PCBIFace->setPhaseNodeInfo("ch0", PhaseNodeMeasInfo->m_srngStore, N, ph0, PhaseJustFreq[N]);
            break;
        case adcXPhase:
        case sensXadcNPhase:
        case sensECTadcNPhase:
            PCBIFace->setPhaseNodeInfo("ch1", PhaseNodeMeasInfo->m_srngStore, N, ph0, PhaseJustFreq[N]);
            break;
        default:
            break;
        }

        AHS++;
        break; // PhaseNodeMeasExec4
    }
    case PhaseNodeMeasExec5:
    {
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            N++;
            if (N < 4)
            {
                AHS = PhaseNodeMeasNodeConfig;
                m_ActTimer->start(0,wm3000Continue);
            }
            else
            {
                lprogress++;
                //m_PhaseNodeMeasInfoList.removeFirst();

                if ((m_PhaseNodeMeasInfoList.size() == (lprogress-1)) || (mWmProgressDialog->isAbort()) || bOverload ) // entweder normal fertig geworden oder abbruch oder übersteuerung (solls eigentlich nicht geben)
                { // wir sind fertig mit der ermittlung
                    if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                    {
                        QTextStream stream( &m_PhaseJustLogfile );
                        stream << "\nTerminated ";
                        if (bOverload)
                            stream << "because of overload condition !\n";
                        else
                        {
                            if (!mWmProgressDialog->isAbort())// m_pAbortButton->isEnabled())
                                stream << "normally\n";
                            else
                                stream << "by user\n";
                        }

                        m_PhaseJustLogfile.flush();
                        m_PhaseJustLogfile.close();
                    }

                    m_PhaseNodeMeasState = PhaseNodeMeasFinished;
                    QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(PhaseJustSyncSlot()));
                    SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
                    AHS = wm3000Idle; // statemachine kann neu gestartet werden
                }
                else
                {
                    if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                    {
                        QTextStream stream( &m_PhaseJustLogfile );
                        stream << "\n"; // für jeden block eine leerzeile
                        m_PhaseJustLogfile.flush();
                        m_PhaseJustLogfile.close();
                    }

                    N = 0; // durchlaufzähler für ermittlung  der stützstellen
                    AHS = PhaseNodeMeasNodeConfig;
                    m_ActTimer->start(0,wm3000Continue);
                }
            }
        }
        break;
    } // PhaseNodeMeasExec5

    case PhaseNodeMeasFinished:
        delete mWmProgressDialog;
        JustagePhaseBerechnungSlot(); // berechnung noch starten
        AHS = wm3000Idle;
        break;


    case InitScopeDialog:
    {
        mScopeDialog = new wmScopeDialog(g_WMView);
        connect(mScopeDialog,SIGNAL(sigIsVisible(bool)),g_WMView,SIGNAL(UIansichtScopeViewSet(bool)));
        AHS++;
        m_ActTimer->start(0,wm3000Continue); // otherwise we must do something to continue
        break;
    }


    case MeasureAllDataSampleRequest:
        if(mScopeDialog->isShowEnabled()){
            if(mScopeDialog->isVisible()) mScopeDialog->hide();
            AHS = TriggerMeasureStart;
            m_ActTimer->start(0,wm3000Continue);
            break;
        }
        DspIFace->DspMemoryRead(mScopeDialog->getSelectedChannelPointer(0));
        AHS++;
        break;


    case MeasureAllDataSample:
    {
        float *val;
        QString str;
        str = mScopeDialog->getSelectedChannelPointer(0)->VarList();
        val = DspIFace->data(mScopeDialog->getSelectedChannelPointer(0));

/*      float fValues [500] {2,-1.99,-1.98,-1.97,-1.96,-1.95,-1.94,-1.93,-1.92,-1.91,-1.9,-1.89,-1.88,-1.87,-1.86,-1.85,-1.84,-1.83,-1.82,-1.81,-1.8,-1.79,-1.78,-1.77,-1.76,-1.75,-1.74,-1.73,-1.72,-1.71,-1.7,-1.69,-1.68,-1.67,-1.66,-1.65,-1.64,-1.63,-1.62,-1.61,-1.6,-1.59,-1.58,-1.57,-1.56,-1.55,-1.54,-1.53,-1.52,-1.51,-1.5,-1.49,-1.48,-1.47,-1.46,-1.45,-1.44,-1.43,-1.42,-1.41,-1.4,-1.39,-1.38,-1.37,-1.36,-1.35,-1.34,-1.33,-1.32,-1.31,-1.3,-1.29,-1.28,-1.27,-1.26,-1.25,-1.24,-1.23,-1.22,-1.21,-1.2,-1.19,-1.18,-1.17,-1.16,-1.15,-1.14,-1.13,-1.12,-1.11,-1.1,-1.09,-1.08,-1.07,-1.06,-1.05,-1.04,-1.03,-1.02,-1.01,-0.999999999999999,-0.989999999999999,-0.979999999999999,-0.969999999999999,-0.959999999999999,-0.949999999999999,-0.939999999999999,-0.929999999999999,-0.919999999999999,-0.909999999999999,-0.899999999999999,-0.889999999999999,-0.879999999999999,-0.869999999999999,-0.859999999999999,-0.849999999999999,-0.839999999999999,-0.829999999999999,-0.819999999999999,-0.809999999999999,-0.799999999999999,-0.789999999999999,-0.779999999999999,-0.769999999999999,-0.759999999999999,-0.749999999999999,-0.739999999999999,-0.729999999999999,-0.719999999999999,-0.709999999999999,-0.699999999999999,-0.689999999999999,-0.679999999999999,-0.669999999999999,-0.659999999999999,-0.649999999999999,-0.639999999999999,-0.629999999999999,-0.619999999999999,-0.609999999999999,-0.599999999999999,-0.589999999999999,-0.579999999999999,-0.569999999999999,-0.559999999999999,-0.549999999999999,-0.539999999999999,-0.529999999999999,-0.519999999999999,-0.509999999999999,-0.499999999999999,-0.489999999999999,-0.479999999999999,-0.469999999999999,-0.459999999999999,-0.449999999999999,-0.439999999999999,-0.429999999999999,-0.419999999999999,-0.409999999999999,-0.399999999999999,-0.389999999999999,-0.379999999999999,-0.369999999999999,-0.359999999999999,-0.349999999999999,-0.339999999999999,-0.329999999999999,-0.319999999999998,-0.309999999999998,-0.299999999999998,-0.289999999999998,-0.279999999999998,-0.269999999999998,-0.259999999999998,-0.249999999999998,-0.239999999999998,-0.229999999999998,-0.219999999999998,-0.209999999999998,-0.199999999999998,-0.189999999999998,-0.179999999999998,-0.169999999999998,-0.159999999999998,-0.149999999999998,-0.139999999999998,-0.129999999999998,-0.119999999999998,-0.109999999999998,-0.0999999999999984,-0.0899999999999984,-0.0799999999999984,-0.0699999999999984,-0.0599999999999984,-0.0499999999999984,-0.0399999999999984,-0.0299999999999984,-0.0199999999999984,-0.00999999999999836,1.64104840827406E-15,0.0100000000000016,0.0200000000000016,0.0300000000000016,0.0400000000000016,0.0500000000000016,0.0600000000000017,0.0700000000000016,0.0800000000000016,0.0900000000000016,0.100000000000002,0.110000000000002,0.120000000000002,0.130000000000002,0.140000000000002,0.150000000000002,0.160000000000002,0.170000000000002,0.180000000000002,0.190000000000002,0.200000000000002,0.210000000000002,0.220000000000002,0.230000000000002,0.240000000000002,0.250000000000002,0.260000000000002,0.270000000000002,0.280000000000002,0.290000000000002,0.300000000000002,0.310000000000002,0.320000000000002,0.330000000000002,0.340000000000002,0.350000000000002,0.360000000000002,0.370000000000002,0.380000000000002,0.390000000000002,0.400000000000002,0.410000000000002,0.420000000000002,0.430000000000002,0.440000000000002,0.450000000000002,0.460000000000002,0.470000000000002,0.480000000000002,0.490000000000002,0.500000000000002,0.510000000000002,0.520000000000002,0.530000000000002,0.540000000000002,0.550000000000002,0.560000000000002,0.570000000000002,0.580000000000002,0.590000000000002,0.600000000000002,0.610000000000002,0.620000000000002,0.630000000000002,0.640000000000002,0.650000000000002,0.660000000000002,0.670000000000002,0.680000000000002,0.690000000000002,0.700000000000002,0.710000000000002,0.720000000000002,0.730000000000002,0.740000000000002,0.750000000000002,0.760000000000002,0.770000000000002,0.780000000000002,0.790000000000002,0.800000000000002,0.810000000000002,0.820000000000002,0.830000000000002,0.840000000000002,0.850000000000002,0.860000000000002,0.870000000000002,0.880000000000002,0.890000000000002,0.900000000000002,0.910000000000002,0.920000000000002,0.930000000000002,0.940000000000002,0.950000000000002,0.960000000000002,0.970000000000002,0.980000000000002,0.990000000000002,1,1.01,1.02,1.03,1.04,1.05,1.06,1.07,1.08,1.09,1.1,1.11,1.12,1.13,1.14,1.15,1.16,1.17,1.18,1.19,1.2,1.21,1.22,1.23,1.24,1.25,1.26,1.27,1.28,1.29,1.3,1.31,1.32,1.33,1.34,1.35,1.36,1.37,1.38,1.39,1.4,1.41,1.42,1.43,1.44,1.45,1.46,1.47,1.48,1.49,1.5,1.51,1.52,1.53,1.54,1.55,1.56,1.57,1.58,1.59,1.6,1.61,1.62,1.63,1.64,1.65,1.66,1.67,1.68,1.69,1.7,1.71,1.72,1.73,1.74,1.75,1.76,1.77,1.78,1.79,1.8,1.81,1.82,1.83,1.84,1.85,1.86,1.87,1.88,1.89,1.9,1.91,1.92,1.93,1.94,1.95,1.96,1.97,1.98,1.99,2.0};
        float fValues [500] { -2000.01,-1990.01,-1980.01,-1970.01,-1960.01,-1950.01,-1940.01,-1930.01,-1920.01,-1910.01,-1900.01,-1890.01,-1880.01,-1870.01,-1860.01,-1850.01,-1840.01,-1830.01,-1820.01,-1810.01,-1800.01,-1790.01,-1780.01,-1770.01,-1760.01,-1750.01,-1740.01,-1730.01,-1720.01,-1710.01,-1700.01,-1690.01,-1680.01,-1670.01,-1660.01,-1650.01,-1640.01,-1630.01,-1620.01,-1610.01,-1600.01,-1590.01,-1580.01,-1570.01,-1560.01,-1550.01,-1540.01,-1530.01,-1520.01,-1510.01,-1500.01,-1490.01,-1480.01,-1470.01,-1460.01,-1450.01,-1440.01,-1430.01,-1420.01,-1410.01,-1400.01,-1390.01,-1380.01,-1370.01,-1360.01,-1350.01,-1340.01,-1330.01,-1320.01,-1310.01,-1300.01,-1290.01,-1280.01,-1270.01,-1260.01,-1250.01,-1240.01,-1230.01,-1220.01,-1210.01,-1200.01,-1190.01,-1180.01,-1170.01,-1160.01,-1150.01,-1140.01,-1130.01,-1120.01,-1110.01,-1100.01,-1090.01,-1080.01,-1070.01,-1060.01,-1050.01,-1040.01,-1030.01,-1020.01,-1010.01,-1000.01,-990.01,-980.01,-970.01,-960.01,-950.01,-940.01,-930.01,-920.01,-910.01,-900.01,-890.01,-880.01,-870.01,-860.01,-850.01,-840.01,-830.01,-820.01,-810.01,-800.01,-790.01,-780.01,-770.01,-760.01,-750.01,-740.01,-730.01,-720.01,-710.01,-700.01,-690.01,-680.01,-670.01,-660.01,-650.01,-640.01,-630.01,-620.01,-610.01,-600.01,-590.01,-580.01,-570.01,-560.01,-550.01,-540.01,-530.01,-520.01,-510.01,-500.01,-490.01,-480.01,-470.01,-460.01,-450.01,-440.01,-430.01,-420.01,-410.01,-400.01,-390.01,-380.01,-370.01,-360.01,-350.01,-340.01,-330.01,-320.01,-310.01,-300.01,-290.01,-280.01,-270.01,-260.01,-250.01,-240.01,-230.01,-220.01,-210.01,-200.01,-190.01,-180.01,-170.01,-160.01,-150.01,-140.01,-130.01,-120.01,-110.01,-100.01,-90.01,-80.01,-70.01,-60.01,-50.01,-40.01,-30.01,-20.01,-10.01,-0.00999999999999091,9.99000000000001,19.99,29.99,39.99,49.99,59.99,69.99,79.99,89.99,99.99,109.99,119.99,129.99,139.99,149.99,159.99,169.99,179.99,189.99,199.99,209.99,219.99,229.99,239.99,249.99,259.99,269.99,279.99,289.99,299.99,309.99,319.99,329.99,339.99,349.99,359.99,369.99,379.99,389.99,399.99,409.99,419.99,429.99,439.99,449.99,459.99,469.99,479.99,489.99,499.99,509.99,519.99,529.99,539.99,549.99,559.99,569.99,579.99,589.99,599.99,609.99,619.99,629.99,639.99,649.99,659.99,669.99,679.99,689.99,699.99,709.99,719.99,729.99,739.99,749.99,759.99,769.99,779.99,789.99,799.99,809.99,819.99,829.99,839.99,849.99,859.99,869.99,879.99,889.99,899.99,909.99,919.99,929.99,939.99,949.99,959.99,969.99,979.99,989.99,999.99,1009.99,1019.99,1029.99,1039.99,1049.99,1059.99,1069.99,1079.99,1089.99,1099.99,1109.99,1119.99,1129.99,1139.99,1149.99,1159.99,1169.99,1179.99,1189.99,1199.99,1209.99,1219.99,1229.99,1239.99,1249.99,1259.99,1269.99,1279.99,1289.99,1299.99,1309.99,1319.99,1329.99,1339.99,1349.99,1359.99,1369.99,1379.99,1389.99,1399.99,1409.99,1419.99,1429.99,1439.99,1449.99,1459.99,1469.99,1479.99,1489.99,1499.99,1509.99,1519.99,1529.99,1539.99,1549.99,1559.99,1569.99,1579.99,1589.99,1599.99,1609.99,1619.99,1629.99,1639.99,1649.99,1659.99,1669.99,1679.99,1689.99,1699.99,1709.99,1719.99,1729.99,1739.99,1749.99,1759.99,1769.99,1779.99,1789.99,1799.99,1809.99,1819.99,1829.99,1839.99,1849.99,1859.99,1869.99,1879.99,1889.99,1899.99,1909.99,1919.99,1929.99,1939.99,1949.99,1959.99,1969.99,1979.99,1989.99,1999.99 };

        float fValues [500] { -2000000.01,-1990000.01,-1980000.01,-1970000.01,-1960000.01,-1950000.01,-1940000.01,-1930000.01,-1920000.01,-1910000.01,-1900000.01,-1890000.01,-1880000.01,-1870000.01,-1860000.01,-1850000.01,-1840000.01,-1830000.01,-1820000.01,-1810000.01,-1800000.01,-1790000.01,-1780000.01,-1770000.01,-1760000.01,-1750000.01,-1740000.01,-1730000.01,-1720000.01,-1710000.01,-1700000.01,-1690000.01,-1680000.01,-1670000.01,-1660000.01,-1650000.01,-1640000.01,-1630000.01,-1620000.01,-1610000.01,-1600000.01,-1590000.01,-1580000.01,-1570000.01,-1560000.01,-1550000.01,-1540000.01,-1530000.01,-1520000.01,-1510000.01,-1500000.01,-1490000.01,-1480000.01,-1470000.01,-1460000.01,-1450000.01,-1440000.01,-1430000.01,-1420000.01,-1410000.01,-1400000.01,-1390000.01,-1380000.01,-1370000.01,-1360000.01,-1350000.01,-1340000.01,-1330000.01,-1320000.01,-1310000.01,-1300000.01,-1290000.01,-1280000.01,-1270000.01,-1260000.01,-1250000.01,-1240000.01,-1230000.01,-1220000.01,-1210000.01,-1200000.01,-1190000.01,-1180000.01,-1170000.01,-1160000.01,-1150000.01,-1140000.01,-1130000.01,-1120000.01,-1110000.01,-1100000.01,-1090000.01,-1080000.01,-1070000.01,-1060000.01,-1050000.01,-1040000.01,-1030000.01,-1020000.01,-1010000.01,-1000000.01,-990000.01,-980000.01,-970000.01,-960000.01,-950000.01,-940000.01,-930000.01,-920000.01,-910000.01,-900000.01,-890000.01,-880000.01,-870000.01,-860000.01,-850000.01,-840000.01,-830000.01,-820000.01,-810000.01,-800000.01,-790000.01,-780000.01,-770000.01,-760000.01,-750000.01,-740000.01,-730000.01,-720000.01,-710000.01,-700000.01,-690000.01,-680000.01,-670000.01,-660000.01,-650000.01,-640000.01,-630000.01,-620000.01,-610000.01,-600000.01,-590000.01,-580000.01,-570000.01,-560000.01,-550000.01,-540000.01,-530000.01,-520000.01,-510000.01,-500000.01,-490000.01,-480000.01,-470000.01,-460000.01,-450000.01,-440000.01,-430000.01,-420000.01,-410000.01,-400000.01,-390000.01,-380000.01,-370000.01,-360000.01,-350000.01,-340000.01,-330000.01,-320000.01,-310000.01,-300000.01,-290000.01,-280000.01,-270000.01,-260000.01,-250000.01,-240000.01,-230000.01,-220000.01,-210000.01,-200000.01,-190000.01,-180000.01,-170000.01,-160000.01,-150000.01,-140000.01,-130000.01,-120000.01,-110000.01,-100000.01,-90000.01,-80000.01,-70000.01,-60000.01,-50000.01,-40000.01,-30000.01,-20000.01,-10000.01,-0.0100000000093132,9999.98999999999,19999.99,29999.99,39999.99,49999.99,59999.99,69999.99,79999.99,89999.99,99999.99,109999.99,119999.99,129999.99,139999.99,149999.99,159999.99,169999.99,179999.99,189999.99,199999.99,209999.99,219999.99,229999.99,239999.99,249999.99,259999.99,269999.99,279999.99,289999.99,299999.99,309999.99,319999.99,329999.99,339999.99,349999.99,359999.99,369999.99,379999.99,389999.99,399999.99,409999.99,419999.99,429999.99,439999.99,449999.99,459999.99,469999.99,479999.99,489999.99,499999.99,509999.99,519999.99,529999.99,539999.99,549999.99,559999.99,569999.99,579999.99,589999.99,599999.99,609999.99,619999.99,629999.99,639999.99,649999.99,659999.99,669999.99,679999.99,689999.99,699999.99,709999.99,719999.99,729999.99,739999.99,749999.99,759999.99,769999.99,779999.99,789999.99,799999.99,809999.99,819999.99,829999.99,839999.99,849999.99,859999.99,869999.99,879999.99,889999.99,899999.99,909999.99,919999.99,929999.99,939999.99,949999.99,959999.99,969999.99,979999.99,989999.99,999999.99,1009999.99,1019999.99,1029999.99,1039999.99,1049999.99,1059999.99,1069999.99,1079999.99,1089999.99,1099999.99,1109999.99,1119999.99,1129999.99,1139999.99,1149999.99,1159999.99,1169999.99,1179999.99,1189999.99,1199999.99,1209999.99,1219999.99,1229999.99,1239999.99,1249999.99,1259999.99,1269999.99,1279999.99,1289999.99,1299999.99,1309999.99,1319999.99,1329999.99,1339999.99,1349999.99,1359999.99,1369999.99,1379999.99,1389999.99,1399999.99,1409999.99,1419999.99,1429999.99,1439999.99,1449999.99,1459999.99,1469999.99,1479999.99,1489999.99,1499999.99,1509999.99,1519999.99,1529999.99,1539999.99,1549999.99,1559999.99,1569999.99,1579999.99,1589999.99,1599999.99,1609999.99,1619999.99,1629999.99,1639999.99,1649999.99,1659999.99,1669999.99,1679999.99,1689999.99,1699999.99,1709999.99,1719999.99,1729999.99,1739999.99,1749999.99,1759999.99,1769999.99,1779999.99,1789999.99,1799999.99,1809999.99,1819999.99,1829999.99,1839999.99,1849999.99,1859999.99,1869999.99,1879999.99,1889999.99,1899999.99,1909999.99,1919999.99,1929999.99,1939999.99,1949999.99,1959999.99,1969999.99,1979999.99,1989999.99,1999999.99};
        float *fval = &fValues[0];
        str = ("Rampe,400;");
        mScopeDialog->setSingalProperties(str);
        mScopeDialog->setSampleValues(fval);
//        */
//        mScopeDialog->setSampleValues(val);
        mScopeDialog->setSampleValues(0,val,str);
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break;
    }


    case MeasureAllDataSampleRequest2ndCh:

        DspIFace->DspMemoryRead(mScopeDialog->getSelectedChannelPointer(1));
        AHS++;
        break;


    case MeasureAllDataSample2ndCh:
    {
        float *val;
        QString str;
        str = mScopeDialog->getSelectedChannelPointer(1)->VarList();
        val = DspIFace->data(mScopeDialog->getSelectedChannelPointer(1));
        mScopeDialog->setSampleValues(1,val,str);
        mScopeDialog->show();

        AHS = TriggerMeasureStart;
        m_ActTimer->start(0,wm3000Continue);
        break;
    }


    case OffsetMeasWM3000Start:
    case OffsetMeasWM3000StartVar:
        m_OffsetJustLogfile.remove(); // beim starten wird das log file gelöscht
        StopMeasurement(); // die kumulieren jetzt nur
        mWmProgressDialog = new wmProgressDialog( trUtf8("Initialisierung..."), 0, m_OffsetMeasInfoList.count(), g_WMView);
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));
        mWmProgressDialog->setCaption(trUtf8("Offsetkorrekturen"));

        lprogress = 0; // int. progress counter
        QObject::connect(mWmProgressDialog,SIGNAL(aborted()),this,SLOT(JustAbortSlot()));

        NewConfData = m_ConfData; // zum umsetzen
        SaveConfData = m_ConfData; // wir haben eine kopie der aktuellen konfiguration
        NewConfData.m_bOECorrection = false; // nix korrigieren
        NewConfData.m_fxPhaseShift = 0.0; // auch hier keine korrekturen
        NewConfData.m_fxTimeShift = 0.0; // dito
        NewConfData.m_nSyncSource = Intern; // es muss intern synchronisiert sein
        NewConfData.m_nTSync = 1000; // 1 sec. ist ok
        NewConfData.m_nIntegrationTime = 1; // 1 sec.
        NewConfData.m_nMeasPeriod = 8; //
        NewConfData.m_bDCmeasurement = true;

        m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
        AHS++;
        break;

    case OffsetMeasWM3000CoefficientClearNode:
        OffsetCalcInfo = m_CalcInfoList.first();
        PCBIFace->setOffsetNodeInfo(OffsetCalcInfo->m_sChannel, OffsetCalcInfo->m_sRange, 0, 0.0, 0.0); // wir setzen jeweils die 1. stützstelle und lassen im anschluss daran die koeffizienten berechnen
        AHS++;
        break; // OffsetMeasCoefficientClearNode

    case OffsetMeasWM3000CoefficientClearStatus:
        OffsetCalcInfo = m_CalcInfoList.first();
        PCBIFace->setOffsetStatus(OffsetCalcInfo->m_sChannel, OffsetCalcInfo->m_sRange, 0); // wir setzen den status zu 0
        AHS++;
        break; // OffsetMeasCoefficientClearNode

    case OffsetMeasWM3000CoefficientClearNodeStatus:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            delete m_CalcInfoList.first();
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                if (!mWmProgressDialog->isAbort()) //m_pAbortButton->isEnabled())
                {
                    PCBIFace->cmpOffsetCoefficient("ch0"); // berechnung der koeffizienten
                    AHS++;
                }
                else
                {
                    m_ActTimer->start(0,RestartMeasurementStart); // die hatten wir gestoppt
                    AHS = wm3000Idle;
                    delete mWmProgressDialog;
                    m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
                }
            }
            else
            {
                AHS--;
                AHS--;
                m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
            }
        }
        break;

    case OffsetMeasWM3000CoefficientClearNFinished:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->cmpOffsetCoefficient("ch1"); // berechnung der koeffizienten
            AHS++;
        }
        break;


    case OffsetMeasWM3000BaseConfiguration:
    case OffsetMeasWM3000BaseConfigurationVar:
    {
        StopMeasurement(); // das kumuliert nur ....
        mWmProgressDialog->setValue(lprogress); // progess bar setzen
        mWmProgressDialog->setLabelText (trUtf8("Konfiguration setzen ..." ));
        OffsetMeasInfo = m_OffsetMeasInfoList.first(); // info was zu tun ist

        if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
        {
            QTextStream stream( &m_OffsetJustLogfile );

            stream << QString("RangeN=%1 RangeX=%2 Mode=%3 ")
                      .arg(OffsetMeasInfo->m_srngN)
                      .arg(OffsetMeasInfo->m_srngX)
                      .arg(MModeName[OffsetMeasInfo->m_nMMode]);
            stream << QString("SenseMode=%1 nS=").arg(SenseModeText[OffsetMeasInfo->m_nSMode]);
            if (OffsetMeasInfo->m_nnS == S80)
                stream << "80\n";
            else
                stream << "256\n";

            m_OffsetJustLogfile.flush();
            m_OffsetJustLogfile.close();
        }

        NewConfData.m_nSenseMode = OffsetMeasInfo->m_nSMode; // sense modus
        NewConfData.m_nSRate = OffsetMeasInfo->m_nnS; // samples / periode
        NewConfData.m_fSFreq = PhaseJustFreq[2]; // fix 50Hz
        NewConfData.m_nMeasMode  =  OffsetMeasInfo->m_nMMode;
        NewConfData.m_sRangeNVorgabe = OffsetMeasInfo->m_srngN; // bereich kanal n
        if (OffsetMeasInfo->m_nMMode == In_IxAbs)
            NewConfData.m_sRangeXVorgabe = OffsetMeasInfo->m_srngX; // bereich kanal x
        else
            NewConfData.m_sRangeETVorgabe = OffsetMeasInfo->m_srngX; // bereich kanal x
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(OffsetJustSyncSlot()));
        SetConfDataSlot(&NewConfData); // und die neue konfiguration
        // die messung wird neu gestartet am ende der konfiguration
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // OffsetMeasBaseConfiguration OffsetMeasBaseConfigurationVar
    }

    case OffsetMeasWM3000Exec1: // konfiguriert ist
    case OffsetMeasWM3000Exec1Var:
        NewConfData.m_sRangeNSoll = NewConfData.m_sRangeN =NewConfData.m_sRangeNVorgabe; // bereich kanal n
        NewConfData.m_sRangeXSoll = NewConfData.m_sRangeX = NewConfData.m_sRangeXVorgabe; // bereich kanal x
        NewConfData.m_sRangeETSoll = NewConfData.m_sRangeET = NewConfData.m_sRangeETVorgabe; // bereich kanal x
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        mCount = OffsetMeasInfo->m_nIgnore; // einschwingzeit setzen in messdurchläufen
        m_sJustText = trUtf8("Einschwingzeit läuft" );
        mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // OffsetMeasExec1 OffsetMeasExec1Var

    case OffsetMeasWM3000Exec2:
    case OffsetMeasWM3000Exec2Var:
        mCount--;
        mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        if (mCount == 0)
        { // eingeschwungen
            m_OffsetMeasState = AHS + 1; // ab jetzt messen wir wirklich
            mCount = OffsetMeasInfo->m_nnMeas; // und setzen den zähler dafür
            switch (OffsetMeasInfo->m_nJMode)
            {
            case sensNsensXOffset:
                m_sJustText = trUtf8("Messung Kanal N %1, X %2 läuft").arg(OffsetMeasInfo->m_srngN).arg(OffsetMeasInfo->m_srngX);
                break;
            case sensNOffset:
                // für den fall dass messkanäle unterschiedliche bereiche haben, was aber z.z. nicht funktioniert
                m_sJustText = trUtf8("Messung Kanal N %1 läuft").arg(OffsetMeasInfo->m_srngN);
                break;
            case sensXOffset:
            case sensECTOffset:
                // dito
                m_sJustText = trUtf8("Messung Kanal X %1 läuft").arg(OffsetMeasInfo->m_srngX);
                break;
            default:
                break;
            }
            mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
            JustValueList.clear(); // offset werte liste leeren, zur aufnahme der neuen messwerte
        }

        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));  // wieder neu verbinden
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // OffsetMeasExec2 OffsetMeasExec2Var

    case OffsetMeasWM3000Exec3:
    case OffsetMeasWM3000Exec3Var:
    {
        int i;

        JustValueList.append(ActValues.dspActValues.ampl1nf); // wir schreiben offset wert in die liste
        JustValueList.append(ActValues.dspActValues.ampl1xf); // dito

        mCount--;
        mWmProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        if (mCount == 0)
        {
            mWmProgressDialog->setLabelText (trUtf8("Berechnung und Datenübertragung ..."));
            offs0 = 0.0;
            offs1 = 0.0;
            int n;

            n = JustValueList.count() >> 1;
            for (i = 0; i < n; i++)
            {
                offs0 -= JustValueList[i*2];
                offs1 -= JustValueList[i*2+1];
            }

            offs0 /= n; // der mittelwert aus den messungen
            offs1 /= n;

            AHS++; // wir laufen einfach drüber und kommen damit zur datenübertragung oder ablage in hash
            m_ActTimer->start(0,wm3000Continue);
        }
        else
        {
            QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
            AHS = wm3000Idle; // wir werden von der nächsten messung getriggert
        }

        break; // OffsetMeasExec3 OffsetMeasExec3Var
    }

    case OffsetMeasWM3000Exec4:
    {
        CWMRange* lr;
        QString sel;

        switch (OffsetMeasInfo->m_nJMode)
        {
        case sensNsensXOffset:
            PCBIFace->setOffsetNodeInfo("ch0", OffsetMeasInfo->m_srngN, 0, offs0, 1.0);
            AHS++; // wir müssen noch den 2. wert speichern
            break;
        case sensNOffset:
            PCBIFace->setOffsetNodeInfo("ch0", OffsetMeasInfo->m_srngN, 0, offs0, 1.0);
            AHS += 2; // wir überspringen das speicher des 2. wertes
            break;
        case sensXOffset:
            PCBIFace->setOffsetNodeInfo("ch1", OffsetMeasInfo->m_srngX, 0, offs1, 1.0);
            AHS += 2; // dito
            break;
        case sensECTOffset:
            lr = Range(OffsetMeasInfo->m_srngX,m_sECTRangeList);
            sel = lr->Selector();
            PCBIFace->setOffsetNodeInfo("ch1", sel, 0, offs1, 1.0);
            AHS += 2; // dito
            break;
        default:
            break;
        }

        break; // OffsetMeasExec4
    }

    case OffsetMeasWM3000Exec4Var:
    {
        // speichern der offset justage werte in hash
        QString key;
        switch (OffsetMeasInfo->m_nJMode)
        {
        case sensNsensXOffset:
            key = Range(OffsetMeasInfo->m_srngN, m_sNRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs0;
            key = Range(OffsetMeasInfo->m_srngX, m_sXRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs1;
            break;
        case sensNOffset:
            key = Range(OffsetMeasInfo->m_srngN, m_sNRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs0;
            break;
        case sensXOffset:
            key = Range(OffsetMeasInfo->m_srngX, m_sXRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs1;
            break;
        case sensECTOffset:
            key = Range(OffsetMeasInfo->m_srngX, m_sECTRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs1;
            break;
        default:
            break;
        }

        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break;
    }

    case OffsetMeasWM3000Exec5:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            // wir speichern noch den 2. ermittelten wert
            PCBIFace->setOffsetNodeInfo("ch1", OffsetMeasInfo->m_srngX, 0, offs1, 1.0);
            AHS++;
        }

        break;

    case OffsetMeasWM3000Exec6:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            mWmProgressDialog->setValue(lprogress);
            m_OffsetMeasInfoList.removeFirst();

            if (m_OffsetMeasInfoList.isEmpty() || (mWmProgressDialog->isAbort()) || bOverload ) // entweder normal fertig geworden oder abbruch oder übersteuerung (solls eigentlich nicht geben)
            { // wir sind fertig mit der ermittlung
                if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                {
                    QTextStream stream( &m_OffsetJustLogfile );
                    stream << "\nTerminated ";
                    if (bOverload)
                        stream << "because of overload condition !\n";
                    else
                    {
                        if (!mWmProgressDialog->isAbort()) //  m_pAbortButton->isEnabled())
                            stream << "normally\n";
                        else
                            stream << "by user\n";
                    }

                    m_OffsetJustLogfile.flush();
                    m_OffsetJustLogfile.close();
                }

                m_OffsetMeasState = AHS + 1; // OffsetMeasFinished;
                QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(OffsetJustSyncSlot()));
                SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
                AHS = wm3000Idle; // statemachine kann neu gestartet werden
            }
            else
            {
                if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                {
                    QTextStream stream( &m_OffsetJustLogfile );
                    stream << "\n"; // für jeden block eine leerzeile
                    m_OffsetJustLogfile.flush();
                    m_OffsetJustLogfile.close();
                }

                AHS = OffsetMeasWM3000BaseConfiguration;
                m_ActTimer->start(0,wm3000Continue);
            }
        }

        break;


    case OffsetMeasWM3000Finished:
        delete mWmProgressDialog;
        JustageOffsetBerechnungSlot(); // berechnung noch starten
        AHS = wm3000Idle;
        break;


    case OffsetMeasWM3000Exec5Var:
        lprogress++;
        mWmProgressDialog->setValue(lprogress);
        m_OffsetMeasInfoList.removeFirst();

        if (m_OffsetMeasInfoList.isEmpty() || (mWmProgressDialog->isAbort()) || bOverload ) // entweder normal fertig geworden oder abbruch oder übersteuerung (solls eigentlich nicht geben)
        { // wir sind fertig mit der ermittlung
            if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
            {
                QTextStream stream( &m_OffsetJustLogfile );
                stream << "\nTerminated ";
                if (bOverload)
                    stream << "because of overload condition !\n";
                else
                {
                    if (!mWmProgressDialog->isAbort())  //m_pAbortButton->isEnabled())
                        stream << "normally\n";
                    else
                        stream << "by user\n";
                }

                m_OffsetJustLogfile.flush();
                m_OffsetJustLogfile.close();
            }

            m_OffsetMeasState = AHS + 1; // OffsetMeasFinishedVar;
            QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(OffsetJustSyncSlot()));
            SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
            AHS = wm3000Idle; // statemachine kann neu gestartet werden
        }
        else
        {
            if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
            {
                QTextStream stream( &m_OffsetJustLogfile );
                stream << "\n"; // für jeden block eine leerzeile
                m_OffsetJustLogfile.flush();
                m_OffsetJustLogfile.close();
            }

            AHS = OffsetMeasWM3000BaseConfigurationVar;
            m_ActTimer->start(0,wm3000Continue);
        }

        break;

    case OffsetMeasWM3000FinishedVar:
        delete mWmProgressDialog;
        measOffsetCorrectionHash = adjOffsetCorrectionHash;
        offsetCorrectionHash2File();
        AHS = wm3000Idle;
        break;


    case OffsetMeasChannelNStart:
        mWmProgressDialog = new wmProgressDialog( trUtf8("Messung..."),0,2,g_WMView);
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));
        mWmProgressDialog->setCaption(trUtf8("Offsetmessung Kanal N"));

        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelNSync:
    case OffsetMeasChannelXSync:
        mWmProgressDialog->setValue(1);
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelNFinished:
        mWmProgressDialog->setValue(2);
        offs0 = ActValues.VekNSek.re();
        if (m_ConfData.m_bOffsetCorrectionN)
            offs0 += m_JustValues.OffsetCorrDevN;
        m_JustValues.OffsetCorrDevN = offs0;
        emit OffsetValue(offs0);
        delete mWmProgressDialog;
        emit SendJustValuesSignal(&m_JustValues);
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelXStart:
        mWmProgressDialog = new wmProgressDialog( trUtf8("Messung..."), 0, 2, g_WMView);
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));

        mWmProgressDialog->setCaption(trUtf8("Offsetmessung Kanal X"));
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelXFinished:
        mWmProgressDialog->setValue(2);
        offs0 = ActValues.VekXSek.re();
        if (m_ConfData.m_bOffsetCorrectionX)
            offs0 += m_JustValues.OffsetCorrDevX;
        m_JustValues.OffsetCorrDevX = offs0;
        emit OffsetValue(offs0);
        delete mWmProgressDialog;
        emit SendJustValuesSignal(&m_JustValues);
        AHS = wm3000Idle;
        break;

    case JustageFlashProgStart:
        PCBIFace->JustFlashProgram();
        AHS++;
        break;

    case JustageFlashProgFinished:
        AHS = wm3000Idle; // ob fehler oder nicht wir sind fertig
        break;

    case JustageFlashExportStart:
        PCBIFace->JustFlashExport(JDataFile);
        AHS++;
        break;

    case JustageFlashExportFinished:
        AHS = wm3000Idle; // ob fehler oder nicht wir sind fertig
        break;

    case JustageFlashImportStart:
        PCBIFace->JustFlashImport(JDataFile);
        AHS++;
        break;

    case JustageFlashImportFinished:
        AHS = wm3000Idle; // ob fehler oder nicht wir sind fertig
        break;

    case EN61850ReadStatusStart:
        if (DspIFace->connected())
        {
            DspIFace->DspMemoryRead(m_dspSetup.getEthData()->ETHStatusHandle); // holt die daten ins dsp interface
            AHS++;
        }
        else
        {
            AHS = wm3000Idle;
        }

        break;

    case EN61850ReadStatusFinished:
        if ( !DspIFace->IFaceError() ) // wenn kein fehler aufgetreten
        {
            cEN61850Info EnStat; // holen der werte
            quint32 *source = (quint32*) DspIFace->data(m_dspSetup.getEthData()->ETHStatusHandle);
            quint32 *dest = &(EnStat.ByteCount[0]);
            for (uint i=0; i< sizeof(EnStat)/sizeof(quint32);i++) *dest++ = *source++;
            emit EN61850StatusSignal(&EnStat); // und senden
        }

        AHS = wm3000Idle; // wir sind so oder so fertig
        break; // EN61850ReadStatusFinished

    case EN61850WriteStatusStart:
        DspIFace->DspMemoryWrite(m_dspSetup.getEthData()->ETHStatusResetHandle); // schreibt die daten in den dsp
        AHS++;
        break; // EN61850WriteStatusStart

    case EN61850WriteStatusFinished:
        AHS = wm3000Idle; // wir sind fertig...ob fehler oder nicht
        break; // EN61850WriteStatusFinished

    case SelftestStart:
        StopMeasurement(); // die kumulieren jetzt nur
        mWmProgressDialog = new wmProgressDialog( trUtf8("Selbstest ..."), 0, m_SelftestInfoList.count(), g_WMView);
        mWmProgressDialog->setAbortButtonText(trUtf8("Abbruch"));
        mWmProgressDialog->setCaption(trUtf8("Selbsttest"));
        lprogress = 0; // int. progress counter
        mWmProgressDialog->setValue(lprogress);
        QObject::connect(mWmProgressDialog,SIGNAL(aborted()),this,SLOT(SelftestAbortSlot()));
        m_SelftestLogfile.remove(); // beim starten wird das log file gelöscht
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break; // SelftestStart

    case SelftestBaseConfiguration:
        NewConfData = m_ConfData; // zum umsetzen
        SaveConfData = m_ConfData; // wir haben eine kopie der aktuellen konfiguration
        NewConfData.m_bOECorrection = false; // nix korrigieren
        NewConfData.m_fxPhaseShift = 0.0; // auch hier keine korrekturen
        NewConfData.m_fxTimeShift = 0.0; // dito
        NewConfData.m_nSyncSource = Intern; // es muss intern synchronisiert sein
        NewConfData.m_nTSync = 1000; // 1 sec. ist ok
        NewConfData.m_nIntegrationTime = 1; // 1 sec.
        NewConfData.m_nMeasPeriod = 16; // wir begrenzen auf 16 signal perioden ....
        NewConfData.m_fSFreq = 50.0; // alles bei 50 hz
        NewConfData.m_nSRate = S80;
        NewConfData.m_nMeasMode  =  In_IxAbs;
        NewConfData.m_nSenseMode = sensNadcX; // wir starten mit messen n gegen adcx
        AHS++;

        // kein break wir laufen einfach drüber

    case SelftestMeasConfiguration:
        mWmProgressDialog->setLabelText (trUtf8("Konfiguration setzen ..." ));
        m_SelftestState = SelftestSetRangeNX; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
        SetConfDataSlot(&NewConfData); // und die neue konfiguration
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // SelftestMeasConfiguration


    case SelftestSetRangeNX:
        mWmProgressDialog->setLabelText (trUtf8("Bereiche setzen ..." ));
        NewConfData.m_nSenseMode = sensNadcX; // wir starten mit messen n gegen adcx
        NewConfData.m_sRangeNVorgabe = NewConfData.m_sRangeXVorgabe = m_SelftestInfoList.first();
        m_SelftestState = SelftestMeasureNSync; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
        SetConfDataSlot(&NewConfData); // und die neue konfiguration

        AHS = wm3000Idle;
        break; // SelftestSetRangeNX

    case SelftestMeasureNSync:
        NewConfData.m_sRangeNSoll = NewConfData.m_sRangeN =NewConfData.m_sRangeNVorgabe; // bereich kanal n
        NewConfData.m_sRangeXSoll = NewConfData.m_sRangeX = NewConfData.m_sRangeXVorgabe; // bereich kanal x

        mWmProgressDialog->setLabelText (trUtf8("Messung ..." ));
        m_SelftestState = SelftestMeasureN; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(SelftestSyncSlot()));

        AHS = wm3000Idle; // ob fehler odernicht wir sind fertig
        break; // SelftestMeasureNSync

    case SelftestMeasureN: // wir haben aktuelle messwerte
        SenseNVektor = ActValues.VekNSek;
        ADCXVektor = ActValues.VekXSek; //   messwerte speichern
        mWmProgressDialog->setLabelText (trUtf8("Modus setzen ..." ));

        m_SelftestState = SelftestMeasureXSync; // hier müssen wir später weitermachen
        NewConfData.m_nSenseMode = sensXadcN; // als nächstes messen wir x gegen adcn
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
        SetConfDataSlot(&NewConfData); // und die neue konfiguration

        AHS = wm3000Idle;
        break; // SelftestMeasureN

    case SelftestMeasureXSync:
        mWmProgressDialog->setLabelText (trUtf8("Messung ..." ));
        m_SelftestState = SelftestMeasureX; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(SelftestSyncSlot()));

        AHS = wm3000Idle;
        break; // SelftestMeasureXSync

    case SelftestMeasureX:
    {
        if (m_SelftestLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) )
        {
            QTextStream stream( &m_SelftestLogfile );
            stream << QString("Range=%1").arg(m_SelftestInfoList.first())
                   << QString("  N=(%1,%2)").arg(SenseNVektor.re()).arg(SenseNVektor.im())
                   << QString("  X=(%1,%2)").arg(ActValues.VekXSek.re()).arg(ActValues.VekXSek.im())
                   << QString("  ADCX=(%1,%2)").arg(ADCXVektor.re()).arg(ADCXVektor.im())
                   << QString("  ADCN=(%1,%2)").arg(ActValues.VekNSek.re()).arg(ActValues.VekNSek.im());
            m_SelftestLogfile.flush();
            m_SelftestLogfile.close();
        }

        // achtung komplexe division !!!!!!
        //if (  ((fabs (1.0 - fabs(SenseNVektor/ActValues.VekXSek))) < 0.4) &&
        //      ((fabs (1.0 - fabs(ADCXVektor/ActValues.VekNSek))) < 0.1) )

        // durch änderung der sensorik ist ein direkter vergleich der sense- bzw. adc-werte nicht mehr möglich
        // stattdessen müssen die verhältnisse von sense- und adcwerten von x und n ins verhältnis gesetzt werden
        SenseXVektor = ActValues.VekXSek;
        ADCNVektor = ActValues.VekNSek;

        if ( fabs(1.0 - fabs((SenseNVektor * ADCNVektor) / (SenseXVektor * ADCXVektor))) < 0.01 )

        {

            if (m_SelftestLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) )
            {
                QTextStream stream( &m_SelftestLogfile );
                stream << "  good\n";
                m_SelftestLogfile.flush();
                m_SelftestLogfile.close();
            }

            // unjustiert lassen wir 1% abweichung zu
            lprogress++; // int. progress counter
            mWmProgressDialog->setValue(lprogress);

            // loggen

            // prüfen ob noch mehr zu testen ist
            m_SelftestInfoList.pop_front();
            if ( m_SelftestInfoList.empty() ) // wir sind fertig mit dem selbsttest
            {
                m_SelftestState = SelftestFinished;
                QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
                SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
                AHS = wm3000Idle; // statemachine kann neu gestartet werden
                emit SelftestDone(0);
            }
            else
            {
                AHS = SelftestSetRangeNX; // wir machen hier weiter
                m_ActTimer->start(0,wm3000Continue);
            }
        }
        else
        {
            // fehler loggen
            // selbst test stop

            if (m_SelftestLogfile.open( QIODevice::WriteOnly | QIODevice::Append) )
            {
                QTextStream stream( &m_SelftestLogfile );
                stream << "  bad\n";
                m_SelftestLogfile.flush();
                m_SelftestLogfile.close();
                SelftestDone(-1); // fehler
            }

            m_SelftestState = SelftestFinished;
            QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
            SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
            AHS = wm3000Idle;
        }

        break; // SelftestMeasureX
    }
    case SelftestFinished:
        delete mWmProgressDialog; // progress dialog schliessen
        m_SelftestMsgBox->show();
        AHS = wm3000Idle;
        break; // SelftestFinished

    default: break;

    } //  switch (AHS)

    if (AHS == wm3000Idle)  // wenn wir fertig sind
        if ( !AHSFifo.empty() ) { // und das kommando fifo nicht leer ist
            int state;
            state = AHSFifo.first();
            AHSFifo.pop_front();
            m_ActTimer->start(0,state); // starten wir selbst das neue kommando
        }
}


void cWM3000I::ServerIFaceErrorHandling(int error, QString host, int port)
{
    int userRM;
    bool binitdone = m_binitDone; // wir merken uns den init. zustand
    m_binitDone = false; // so laufen keine weiteren ereignisse auf

    emit AffectStatus(SetQuestStat, QuestServerProblems); // questionable status setzen
    
    if (error & (myErrConnectionRefused | myErrHostNotFound | myErrSocketConnectionTimeOut) )
    {
        QString m = tr("Keine Verbindung zu %1:%2\n") .arg(host).arg(port);
        if (error & ! myErrHostNotFound)
            m+=tr("Host nicht gefunden.\n");
        else
            m+=tr("Host gefunden. Keine Verbindung zu Server.\n");

        m+=tr("Das Programm kann ohne Server nur\n"
              "im Simulations Modus betrieben werden.\n");

        userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),m,
                                       tr("Programm&Abbruch"),
                                       tr("&Wiederholen"),
                                       tr("&Simulation"),
                                       1,-1 );
    }

    else if (error & myErrSocketWrite) {
        userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),
                                       tr("Fehler beim Schreiben von Daten\n"
                                          "für %1:%2 .\n"
                                          "Details stehen in LogFile.").arg(host) .arg(port),
                                       tr("Programm&Abbruch"),
                                       tr("&Wiederholen"),
                                       tr("&Simulation"),
                                       1,-1 );
    }

    else if (error & (myErrSocketUnexpectedAnswer | myErrSocketReadTimeOut) ) {
        userRM =  QMessageBox::warning( 0, tr("TCP-Verbindung"),
                                        tr("Unerwartete Antwort beim Lesen\n"
                                           "von %1:%2 erhalten.\n"
                                           "Details stehen in LogFile.").arg(host).arg(port),
                                        tr("Programm&Abbruch"),
                                        tr("&Wiederholen"),
                                        tr("&Simulation"),1,-1 );
    }
    
    else if (error & myErrDeviceBusy) {
        userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),
                                       tr("Device ist busy\n"
                                          "( %1:%2 ).\n"
                                          "Details stehen in LogFile.").arg(host) .arg(port),
                                       tr("Programm&Abbruch"),
                                       tr("&Wiederholen"),
                                       tr("&Simulation"),
                                       1,-1 );
    }

    else
    {
        qDebug("Socket Error unknown\n");
        return;
    }
    
    switch (userRM) {
    case 0 :
        emit AbortProgramSignal(); // benutzer hat programm abruch gewählt
        break;
    case 1 :
        m_ActTimer->start(0,wm3000Repeat); // wir wiederholen den versuch
        m_binitDone = binitdone; // wir setzen init. zustand zurück
        break;
    case 2 :
        m_ActTimer->start(0,EnterSimulationMode);
        break;
    }
}


void cWM3000I::InitWM3000()
{
    if (isConventional())
        SerialVersions.DeviceName  = tr("Wandlermesseinrichtung WM1000I");
    else
        SerialVersions.DeviceName  = tr("Wandlermesseinrichtung WM3000I");
    float f=50.0;
    switch (m_ConfData.m_nSFreq) {  // wir setzen den realen frequenzwert
    case F16: f = 50.0/3;break;
    case F50: f = 50.0;break;
    case F60: f = 60.0;
    }
    
    m_ConfData.m_fSFreq = f;
    emit SendConfDataSignal(&m_ConfData); // wir senden die konfiguration an alle ...
    m_ActTimer->start(0,InitializationStart);
}


void cWM3000I::setConventional(bool b)
{
    m_bConventional = b;
}


bool cWM3000I::isConventional()
{
    return m_bConventional;
}


void cWM3000I::setDC(bool b)
{
    m_bDC = b;
}


bool cWM3000I::isDC()
{
    return m_bDC;
}


void cWM3000I::setNewSamplerates(bool b)
{
    m_bNewSamplerates = b;
}


bool cWM3000I::isNewSamplerates()
{
    return m_bNewSamplerates;
}


void cWM3000I::setIpAddress(QString address)
{
    m_IpAddress = address;
    setupServers();
}


//------------------------------------------- ab hier stehen alle SLOTs--------------------------------------------------------

void cWM3000I::GetOETAnalizeDone(void)
{
    m_ConfData.m_bOENewLoad = false;
    emit SendConfDialogInfoSignal(m_ownError.GetNPrimList(), m_ownError.GetNSekList());
    // daten werden von cOwnError gelesen, gehalten, zur verfügung gestellt
}


void cWM3000I::JustageAmplitudeSlot(void)
{
    QMessageBox::information( 0, tr("Amplituden Justage"),tr("Noch nicht implementiert"),QMessageBox::Ok);
}


void cWM3000I::JustagePhaseSlot(void)
{
    SetPhaseCalcInfo(); // zum löschen der koeffizienten
    SetPhaseNodeMeasInfo(); // zum ermitteln der nodes
    emit StartStateMachine(PhaseNodeMeasStart);
}


void cWM3000I::JustagePhaseBerechnungSlot(void)
{
    SetPhaseCalcInfo();
    emit StartStateMachine(CmpPhaseCoeffStart);
}


void cWM3000I::JustageFlashProgSlot(void)
{
    emit StartStateMachine(JustageFlashProgStart);
}


void cWM3000I::JustageFlashExportSlot(QString s)
{
    QFileInfo fi( s );
    s.replace("."+fi.extension(false),"");
    JDataFile = s;
    emit StartStateMachine(JustageFlashExportStart);
}


void cWM3000I::JustageFlashImportSlot(QString s)
{
    QFileInfo fi( s );
    s.replace("."+fi.extension(false),"");
    JDataFile = s;
    emit StartStateMachine(JustageFlashImportStart);
}


void cWM3000I::JustageOffsetSlot()
{
    SetOffsetCalcInfo();
    SetOffsetMeasInfo(4,20); // zum ermitteln der offsetkorrekturen
    emit StartStateMachine(OffsetMeasWM3000Start);
}


void cWM3000I::JustageOffsetVarSlot()
{
    SetOffsetMeasInfo(2,4);
    emit StartStateMachine(OffsetMeasWM3000StartVar);
}


void cWM3000I::JustageOffsetBerechnungSlot()
{
    SetOffsetCalcInfo();
    emit StartStateMachine(CmpOffsetCoeffStart);
}


void cWM3000I::OffsetMessungChannelNSlot()
{
    emit StartStateMachine(OffsetMeasChannelNStart);
}


void cWM3000I::OffsetMessungChannelXSlot()
{
    emit StartStateMachine(OffsetMeasChannelXStart);
}



void cWM3000I::SelfTestManuell()
{
    SetSelfTestInfo(false);
///    m_pAbortButton = new QPushButton(trUtf8("Abbruch"),0,0);
    emit StartStateMachine(SelftestStart);
}


void cWM3000I::SelfTestRemote(void)
{
    SetSelfTestInfo(true);
//    m_pAbortButton = 0; // kein abbruch möglich
    emit StartStateMachine(SelftestStart);
}


void cWM3000I::OffsetMessungChannelNRemote()
{
    emit StartStateMachine(OffsetMeasChannelNStart);
}


void cWM3000I::OffsetMessungChannelXRemote()
{
    emit StartStateMachine(OffsetMeasChannelXStart);
}


void cWM3000I::EN61850ResetStatusSlot()
{
    emit StartStateMachine(EN61850WriteStatusStart);
}


void cWM3000I::EN61850InfoRequestSlot()
{
    emit StartStateMachine(EN61850ReadStatusStart);
}


void cWM3000I::DefaultSettingsSlot()
{
    cConfData tmpConfData;
    DefaultSettings(tmpConfData);
    SetConfDataSlot(&tmpConfData);
}


void cWM3000I::DefaultSettingsMeasurementSlot() // wird nach *rst aufgerufen
{
    DefaultSettingsMeasurement(m_ConfData);
    emit AffectStatus(ResetQuestStat, QuestOverLoadMax);
    m_ActTimer->start(0,SenseProtectionOff);
    m_ActTimer->start(10,InitializationStart);
    m_OVLMsgBox->hide(); // falls die überlast message box offen ist schliessen wir sie
}

void cWM3000I::wmScopeViewEnabled(bool bshow)
{
    if(mScopeDialog)
        mScopeDialog->setViewEnabled(bshow);
}


void cWM3000I::DefaultSettings(cConfData& cdata) // alle einstellungen default
{
    cdata.setConfVersion();
    cdata.m_bRunning = true; // läuft oder läuft nicht
    cdata.m_bSimulation = false;
    cdata.Language = de; // default deutsch

    DefaultSettingsMeasurement(cdata);
    
    cdata.m_sOETFile = "";
    cdata.m_sResultFile = "";
    cdata.m_sADSPFile = "/opt/zera/bin/zdsp21262.ldr"; // dsp boot file
    cdata.m_bOENewLoad = false;
    cdata.m_nLogFileMax = 8192;
    
    cdata.m_sRangeN = m_ConfData.m_sRangeNVorgabe = m_ConfData.m_sRangeNSoll  = "15.0A";
    cdata.m_sRangeX = m_ConfData.m_sRangeXVorgabe = m_ConfData.m_sRangeXSoll  = "15.0A";
    cdata.m_sRangeET = m_ConfData.m_sRangeETVorgabe = m_ConfData.m_sRangeETSoll = "15.0V";
    
    cdata.FirstASDU = 1;
    cdata.LastASDU = 1;
    cdata.DataSet = 1;

    for(int i = 0; i < 6; i++) // default mac adressen
    {
        cdata.m_MacSourceAdr.MacAdrByte[i] = 58;
        cdata.m_MacDestAdr.MacAdrByte[i] = 59;
    }
    
    cdata.m_nPriorityTagged = (0x8100 << 16) + 0x8000; // TPID + UserPriority + CFI + VID
    cdata.m_nEthTypeHeader = (0x88BA << 16) + 0x4000; // Ethertype + APPID
    cdata.m_bStrongEthSynchronisation = false;
    
    // ende default konf. setzen
}


void cWM3000I::DefaultSettingsMeasurement(cConfData& cdata) // alle mess einstellungen default
{
    cdata.m_bOECorrection = false;
    cdata.m_bDCmeasurement = false;
    cdata.m_bOffsetCorrectionN = false;
    cdata.m_bOffsetCorrectionX = false;
    cdata.m_nMeasMode = In_IxAbs;
    cdata.m_fxPhaseShift = 0.0;
    cdata.m_fxTimeShift = 0.0;
    cdata.m_nMeasPeriod = 16; // 16 perioden
    cdata.m_nIntegrationTime = 1; // 1 sec.
    cdata.m_nSFreq = F50;
    cdata.m_nSRate = S80;
    cdata.m_nSenseMode = sensNsensX; // normale messung
    cdata.m_nSyncSource = Intern;
    cdata.m_nTSync = 1000;

    cdata.m_NPrimary = "15A";
    cdata.m_NSecondary = "15A";
    cdata.m_XPrimary = "15A";
    cdata.m_XSecondary = "15A";
    cdata.m_ETPrimary = "15A";
    cdata.m_ETSecondary = "15V";
}


void cWM3000I::PhaseJustSyncSlot()
{
    QObject::disconnect(this,0,this,SLOT(PhaseJustSyncSlot()));
    emit StartStateMachine(m_PhaseNodeMeasState); // bei der justage weitermachen
}


void cWM3000I::OffsetJustSyncSlot()
{
    QObject::disconnect(this,0,this,SLOT(OffsetJustSyncSlot()));
    emit StartStateMachine(m_OffsetMeasState); // bei der justage weitermachen
}


void cWM3000I::JustAbortSlot()
{
//    m_pAbortButton->setEnabled(false);
    /* abbbruch in state machine behandelt
    while (m_PhaseNodeMeasInfoList.count() > 1)
    m_PhaseNodeMeasInfoList.removeLast();
*/
}

void cWM3000I::SelftestSyncSlot()
{
    QObject::disconnect(this,0,this,SLOT(SelftestSyncSlot()));
    emit StartStateMachine(m_SelftestState); // selbsttest weiter durchführen
}

void cWM3000I::SelftestAbortSlot()
{
//    m_pAbortButton->setEnabled(false);
    while (m_SelftestInfoList.count() > 1)
        m_SelftestInfoList.pop_back();
}

void cWM3000I::OverLoadMaxQuitSlot()
{
    emit AffectStatus(ResetQuestStat, QuestOverLoadMax);
    emit StartStateMachine(SenseProtectionOff);
    AHSFifo.remove(RangeObsermaticStart); // alle events zur bereichüberwachung löschen
}


void cWM3000I::externalTriggerTimeoutTriggerd()
{
    if (m_ConfData.m_nSyncSource == Extern) {
        m_ConfData.m_nSyncSource = Intern;
        m_ActTimer->start(0,ConfigurationSetSyncSource);
        emit PPSQuestionable(true);
    }
}


void cWM3000I::SetPhaseCalcInfo() // wir init. die liste damit die statemachine weiß was zu tun ist
{
    QString chn;
    m_CalcInfoList.clear();
    //m_CalcInfoList.setAutoDelete( TRUE );
    chn = "ch0";
    // ad-wandler abgleich findet nicht mehr statt
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw80"));
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw256"));

    // jetzt doch wieder .....
    // wir benötigen phasenkorrekturwerte in abhängigkeit von der adwandler samplerate
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.16"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.60"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.16"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.60"));
    if (m_bNewSamplerates) {
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW96.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW96.60"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW288.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW240.60"));
    }
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
        m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));

    chn = "ch1";
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw80"));
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw256"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.16"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.60"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.16"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.60"));
    if (m_bNewSamplerates) {
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW96.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW96.60"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW288.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW240.60"));
    }
    for (uint i = 0; i < m_sXRangeList.count()-1; i++)
        m_CalcInfoList.append(new cCalcInfo(chn, m_sXRangeList.at(i)->Selector()));

    for (uint i = 0; i < m_sECTRangeList.count()-1; i++)
        m_CalcInfoList.append(new cCalcInfo(chn, m_sECTRangeList.at(i)->Selector()));
}    


void cWM3000I::SetPhaseNodeMeasInfo() // wir init. die liste damit die statemachine weiß was zu tun ist
{
    m_PhaseNodeMeasInfoList.clear();
  //  qDeleteAll(m_PhaseNodeMeasInfoList);
    //m_PhaseNodeMeasInfoList.setAutoDelete( TRUE );

    /* wir gleichen die ad-wandler nicht mehr ab
        // zuerst die adwandler abgleichen
        m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcNadcX, Un_UxAbs, S80, 4, 10)); // bereiche optimal für hw freq messung, modus adc/adc, für 80 samples/periode und 2 messungen einschwingzeit, 6 messungen für stützstellenermittlung
        m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcNadcX, Un_UxAbs, S256, 4, 10));
        m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcXadcN, Un_UxAbs, S80, 4, 10)); // bereiche optimal für hw freq messung, modus adc/adc, für 80 samples/periode und 2 messungen einschwingzeit, 6 messungen für stützstellenermittlung
        m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcXadcN, Un_UxAbs, S256, 4, 10));
    */
    // jetzt doch wieder
    m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW80.50", adcNadcX, In_IxAbs, adcNPhase, S80, 4, 20)))); // bereiche optimal für hw freq messung, modus adc/adc, für 80 samples/periode und 4 messungen einschwingzeit, 10 messungen für stützstellenermittlung
    m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW256.50", adcNadcX, In_IxAbs, adcNPhase, S256, 4, 20))));
    m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW80.50", adcNadcX, In_IxAbs, adcXPhase, S80, 4, 20))));
    m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW256.50", adcNadcX, In_IxAbs, adcXPhase, S256, 4, 20))));
    if (m_bNewSamplerates) {         // new sample rates
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW96.50", adcNadcX, In_IxAbs, adcNPhase, S96, 4, 20))));
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW96.50", adcNadcX, In_IxAbs, adcXPhase, S96, 4, 20))));
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW288.50", adcNadcX, In_IxAbs, adcNPhase, S288, 4, 20))));
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW288.50", adcNadcX, In_IxAbs, adcXPhase, S288, 4, 20))));
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW240.60", adcNadcX, In_IxAbs, adcNPhase, S240, 4, 20))));
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( "5mA", "5mA", "ADW240.60", adcNadcX, In_IxAbs, adcXPhase, S240, 4, 20))));
    }

    // die liste für alle konv. bereiche in kanal n
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo( m_sNRangeList.at(i)->Name(), "5mA", m_sNRangeList.at(i)->Name(), sensNadcX, In_IxAbs, sensNadcXPhase, S80, 4, 20))));

    // die liste für alle konv. bereiche in kanal x
    for (uint i = 0; i < m_sXRangeList.count()-1; i++)
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo("5mA", m_sXRangeList.at(i)->Name(), m_sXRangeList.at(i)->Name(), sensXadcN, In_IxAbs, sensXadcNPhase, S80, 4, 20))));

    // + die liste der evt bereiche in kanal x
    for (uint i = 0; i < m_sECTRangeList.count()-1; i++) // i = 0 wäre der safety range.... jetzt nicht mehr
        m_PhaseNodeMeasInfoList.push_back(( std::unique_ptr<cJustMeasInfo>(new cJustMeasInfo("5mA", m_sECTRangeList.at(i)->Name(), m_sECTRangeList.at(i)->Selector(), sensXadcN, In_ECT, sensECTadcNPhase, S80, 4, 20))));

}    


void cWM3000I::SetOffsetCalcInfo()
{
    QString chn;
    m_CalcInfoList.clear();
    //m_CalcInfoList.setAutoDelete( TRUE );
    chn = "ch0";
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
        m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));
    chn = "ch1";
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
        m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));
    for (uint i = 0; i < m_sECTRangeList.count()-1; i++)
        m_CalcInfoList.append(new cCalcInfo(chn, m_sECTRangeList.at(i)->Selector()));
}


void cWM3000I::SetOffsetMeasInfo(int te, int tm)
{
    QString key;

    m_OffsetMeasInfoList.clear();
    //qDeleteAll(m_OffsetMeasInfoList);

    adjOffsetCorrectionHash.clear();
    measOffsetCorrectionHash.clear();

    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    {
        key = m_sNRangeList.at(i)->getOffsKorrKey();
        if (! adjOffsetCorrectionHash.contains(key))
        {
            // für diesen bereich haben wir noch keine eintrag in die liste , was zu tun ist
            adjOffsetCorrectionHash[key] = 0.0; // platzhalter
            m_OffsetMeasInfoList.append(new cJustMeasInfo(m_sNRangeList.at(i)->Name(), m_sXRangeList.at(0)->Name(),m_sNRangeList.at(i)->Name(), sensNsensX0V, In_IxAbs, sensNOffset, S80, te, tm));
        }
    }

    for (uint i = 0, j = adjOffsetCorrectionHash.count(); i < m_sXRangeList.count()-1; i++ )
    {
        key = m_sXRangeList.at(i)->getOffsKorrKey();
        if (!adjOffsetCorrectionHash.contains(key))
        {
            // für diesen bereich haben wir noch keine eintrag in die liste , was zu tun ist
            adjOffsetCorrectionHash[key] = 0.0; // platzhalter
            if (j > 0)
            {
                // wir nehmen einen bereits vorhanden eintrag in der offset mess liste
                cJustMeasInfo *jmi;
                jmi = m_OffsetMeasInfoList.at(i);
                jmi->m_nJMode = sensNsensXOffset;
                jmi->m_srngX = m_sXRangeList.at(i)->Name();
                j--;
            }
            else
                m_OffsetMeasInfoList.append(new cJustMeasInfo(m_sNRangeList.at(0)->Name(), m_sXRangeList.at(i)->Name(),m_sXRangeList.at(i)->Name(), sensNsensX0V, In_IxAbs, sensXOffset, S80, te, tm));
        }
    }

    for (uint i = 0; i < m_sECTRangeList.count()-1; i++)
    {
        key = m_sECTRangeList.at(i)->getOffsKorrKey();
        if (! adjOffsetCorrectionHash.contains(key))
        {
            // für diesen bereich haben wir noch keine eintrag in die liste , was zu tun ist
            adjOffsetCorrectionHash[key] = 0.0; // platzhalter
            m_OffsetMeasInfoList.append(new cJustMeasInfo(m_sNRangeList.at(0)->Name(), m_sECTRangeList.at(i)->Name(),m_sECTRangeList.at(i)->Selector(), sensNsensX0V, In_ECT, sensECTOffset, S80, te, tm));
        }
    }
}


void cWM3000I::offsetCorrectionHash2File()
{
    m_OffsetDatafile.remove();
    m_NSAOffsetDatafile.remove();

    if (m_OffsetDatafile.open(QIODevice::WriteOnly))
    {
        QDataStream st(&m_OffsetDatafile);
        st.setVersion(QDataStream::Qt_4_0);

        QList<QString> keylist;
        keylist = measOffsetCorrectionHash.keys();

        for (int i = 0; i < keylist.count(); i++)
        {
            QString key;

            key = keylist.at(i);
            st << key << measOffsetCorrectionHash[key];
        }
        m_OffsetDatafile.flush();
        m_OffsetDatafile.close();
    }

    if (m_NSAOffsetDatafile.open(QIODevice::WriteOnly))
    {
        QTextStream st(&m_NSAOffsetDatafile);

        QList<QString> keylist;
        keylist = measOffsetCorrectionHash.keys();

        for (int i = 0; i < keylist.count(); i++)
        {
            QString key;
            QString output;

            key = keylist.at(i);
            output = QString("%1;%2\n").arg(key).arg(measOffsetCorrectionHash[key]);
            st << output;
        }

        m_NSAOffsetDatafile.flush();
        m_NSAOffsetDatafile.close();
    }
}


void cWM3000I::readOffsetCorrectionFile()
{
    adjOffsetCorrectionHash.clear();
    measOffsetCorrectionHash.clear();

    if (m_OffsetDatafile.open(QIODevice::ReadOnly))
    {
        QString key;
        double val;
        QDataStream st(&m_OffsetDatafile);
        st.setVersion(QDataStream::Qt_4_0);
        while (!m_OffsetDatafile.atEnd())
        {
            st >> key >> val;
            adjOffsetCorrectionHash[key] = val;
        }

        measOffsetCorrectionHash = adjOffsetCorrectionHash;
    }
}


void cWM3000I::SetSelfTestInfo(bool remote)
{
    uint i;
    
    m_SelftestInfoList.clear();
    for (i = 0; i < m_sNRangeList.count()-1; i++)
    {
        m_SelftestInfoList.append(m_sNRangeList.at(i)->Name());
        if (remote) break;
    }
    
    // wir testen nur die nx bereiche beim selbst test .... zu kompliziert die ect´s mit zu testen
    
}


int cWM3000I::signum(double value)
{
    return (value>0)?1:((value<0)?-1:0);
}


void cWM3000I::SetConfDataSlot(cConfData *cd) // signal kommt vom konfigurations dialog oder interface
{ // oder aus statemachine 
    // we limit the number of
    if ((cd->m_nSRate == S80) && (cd->m_nMeasPeriod > nmaxS80MeasPeriod))
        cd->m_nMeasPeriod = nmaxS80MeasPeriod;
    if ((cd->m_nSRate == S256) && (cd->m_nMeasPeriod > nmaxS256MeasPeriod))
        cd->m_nMeasPeriod = nmaxS256MeasPeriod;

    m_ConfDataCopy = m_ConfData; // alte konfiguration
    m_ConfData = *cd;
    emit SendConfDataSignal(&m_ConfData); // die anderen auch informieren
    emit StartStateMachine(ConfigurationStart);
}


void cWM3000I::SetRangeSlot(cConfData *cd)
{
    m_ConfData = *cd;
    emit StartStateMachine(SetRangeStart);
}    


void cWM3000I::MeasureSlot()
{   
    if (m_ConfData.m_bRunning)
    {
        if (m_ConfData.m_bSimulation) {
            SimulatedMeasurement();
        }
        else
        {
            if (m_binitDone)
                emit StartStateMachine(MeasureAllDataSampleRequest); // was TriggerMeasureStart);
        }
    }
}


void cWM3000I::MeasureLPSlot()
{   
    if (m_ConfData.m_bRunning)
    {
        if ( !m_ConfData.m_bSimulation && m_binitDone)
            emit StartStateMachine(MeasureLPStart); // wir holen frmsn und berechnen den lastpunkt
    }
}



void cWM3000I::RangeObsermaticSlot()
{
    if (m_binitDone)
        emit StartStateMachine(RangeObsermaticStart);
}


void cWM3000I::DspIFaceAsyncDataSlot(const QString& s) // für asynchrone meldungen vom dsp server
{
    QString sintnr;
    sintnr = s.section( ':', 1, 1);
    
    int service = sintnr.toInt();
    switch (service)
    {
    case 1:  m_AsyncTimer->start(0,MeasureStart); // starten der statemachine für messwert aufnahme
        break;
    case 3:  m_MovingWindowFilter.setFilterLength(m_ConfData.m_nIntegrationTime);
        m_measureTimer.start(1*1000); //  ab movingwindowfilter immer 1*1000 msec
        m_measureLPTimer.start(500);
        m_rangeTimer.start(500);
        m_bStopped = false;
        break;
    }
}


void cWM3000I::XIFaceDoneSlot() // wenn gültige daten da sind vom einen oder anderen server
{
    m_ActTimer->start(0,wm3000Continue); // starten der statemachine um weiter zu machen
}


void cWM3000I::DspIFaceErrorSlot() // wenn fehler aufgetreten sind am dsp server
{
    int error =  DspIFace->iFaceSock->GetError(); // erstmal fehler abholen
    DspIFace->iFaceSock->ResetError();
    ServerIFaceErrorHandling(error, TCPConfig.dspHost, TCPConfig.dspPort);
}


void cWM3000I::pcbIFaceErrorSlot() // dito für leitenkarten server
{
    int error =  PCBIFace->iFaceSock->GetError(); // erstmal fehler abholen
    PCBIFace->iFaceSock->ResetError();
    ServerIFaceErrorHandling(error, TCPConfig.pcbHost, TCPConfig.pcbPort);
}


bool cWM3000I::LoadSettings(QString session)
{
    bool ret;
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000i/wm3000i%2").arg(QDir::homePath()).arg(fi.fileName());
    QFile file(ls);
    if ((ret = file.open( QIODevice::ReadOnly )) == true ) {
        QDataStream stream(&file);
        ret &= m_ConfData.deserialize(stream);
        file.close();
    }
    emit SendConfDataSignal(&m_ConfData); // neue conf. noch senden
    return ret;
}


void cWM3000I::WriteSettings(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000i/wm3000i%2").arg(QDir::homePath()).arg(fi.fileName());
    QFile file(ls);
    //    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
        file.at(0);
        QDataStream stream(&file);
        m_ConfData.serialize(stream);
        file.close();
    }
}


void cWM3000I::StoreResultsSlot()
{
    if (m_ConfData.m_bRunning) { // nur wenn messung läuft speichern
        bool ok = false;
        QDomDocument resultDoc("WM3000ResultData");
        QFile rfile(m_ConfData.m_sResultFile);
        if (rfile.open( QIODevice::ReadOnly ) ) { // ? xml file lesen
            ok = resultDoc.setContent(&rfile);
            rfile.close();
        }

        QDomDocumentType TheDocType = resultDoc.doctype ();

        if  (!ok || ( TheDocType.name() != "WM3000ResultData")) {
            QDomDocument tmpDoc("WM3000ResultData");

            QDomElement rootTag;
            rootTag = tmpDoc.createElement( "RESULTEXPORT" );
            tmpDoc.appendChild( rootTag );

            QDomElement deviceTag = tmpDoc.createElement( "DEVICE" );
            rootTag.appendChild( deviceTag );

            QDomElement tag = tmpDoc.createElement( "Type" );
            deviceTag.appendChild( tag );
            QDomText t = tmpDoc.createTextNode( SerialVersions.DeviceName );
            tag.appendChild( t );

            tag = tmpDoc.createElement( "VersionNumber" );
            deviceTag.appendChild( tag );
            t = tmpDoc.createTextNode(SerialVersions.DeviceVersion );
            tag.appendChild( t );

            tag = tmpDoc.createElement( "SerialNumber" );
            deviceTag.appendChild( tag );
            t = tmpDoc.createTextNode( SerialVersions.PCBSerialNr );
            tag.appendChild( t );

            tag = tmpDoc.createElement( "DeviceStatus" );
            deviceTag.appendChild( tag );
            if (m_bJust)
                t = tmpDoc.createTextNode( "Justified" );
            else
                t = tmpDoc.createTextNode( "Not justified" );
            tag.appendChild( t );

            tag = tmpDoc.createElement( "RESULTLIST" );
            rootTag.appendChild( tag );

            QString s = tmpDoc.toString();
            resultDoc.setContent(tmpDoc.toString());
        }


        QDomElement docElem = resultDoc.documentElement();
        QDomNode n = docElem.lastChild();

        QDomElement resultTag = resultDoc.createElement( "RESULT" );
        n.appendChild( resultTag );

        QDomElement tag = resultDoc.createElement( "LoadPointN" );
        resultTag.appendChild( tag );
        QDomText t = resultDoc.createTextNode( QString("%1%;%2%").arg(ActValues.LoadPoint,7,'f',3).arg(ActValues.LoadPoint1,7,'f',3) ); // lastpunkt bezogen auf rms und rms grundschwingung
        tag.appendChild( t );

        tag = resultDoc.createElement( "LoadPointX" );
        resultTag.appendChild( tag );
        t = resultDoc.createTextNode( QString("%1%;%2%").arg(ActValues.LoadPointX,7,'f',3).arg(ActValues.LoadPoint1X,7,'f',3) ); // lastpunkt bezogen auf rms und rms grundschwingung
        tag.appendChild( t );

        tag = resultDoc.createElement( "AmplError" );
        resultTag.appendChild( tag );
        t = resultDoc.createTextNode( QString("%1%").arg(ActValues.AmplErrorIEC,7,'f',3) );
        tag.appendChild( t );

        tag = resultDoc.createElement( "AngleError" );
        resultTag.appendChild( tag );
        t = resultDoc.createTextNode( QString("%1rad").arg(ActValues.AngleError,8,'f',5) );
        tag.appendChild( t );

        tag = resultDoc.createElement( "ANSIError" );
        resultTag.appendChild( tag );
        t = resultDoc.createTextNode( QString("%1%").arg(ActValues.AmplErrorANSI,7,'f',3) );
        tag.appendChild( t );

        tag = resultDoc.createElement( "Date" );
        resultTag.appendChild( tag );
        QDate d = QDate::currentDate();
        t = resultDoc.createTextNode(d.toString(Qt::TextDate));
        tag.appendChild( t );

        tag = resultDoc.createElement( "Time" );
        resultTag.appendChild( tag );
        QTime ti = QTime::currentTime();
        t = resultDoc.createTextNode(ti.toString(Qt::TextDate));
        tag.appendChild( t );

        QDomElement conditionsTag = resultDoc.createElement( "Conditions" );
        resultTag.appendChild( conditionsTag );

        tag = resultDoc.createElement( "Mode" );
        conditionsTag.appendChild( tag );
        char* modeName[4] = {(char*)"In/IxDiff", (char*)"In/ECT",(char*)"In/nConvent",(char*)"In/Ix"};
        char* simName[2] = {(char*)"real",(char*)"simulated"};
        t = resultDoc.createTextNode(QString("%1 %2").arg(QString(simName[(int)m_ConfData.m_bSimulation]))
                .arg(QString(modeName[m_ConfData.m_nMeasMode])));
        tag.appendChild( t );

        tag = resultDoc.createElement( "NRange" );
        conditionsTag.appendChild( tag );
        t = resultDoc.createTextNode(m_ConfData.m_sRangeN);
        tag.appendChild( t );

        tag = resultDoc.createElement( "XRange" );
        conditionsTag.appendChild( tag );

        QString s;
        switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
        {
        case In_IxDiff:
        case In_IxAbs:
            s = m_ConfData.m_sRangeX;
            break;
        case In_ECT:
            s = m_ConfData.m_sRangeET;
            break;
        case In_nConvent:
            s = "-----";
        }
        t = resultDoc.createTextNode(s);
        tag.appendChild( t );

        QDomElement uncorrTag  = resultDoc.createElement( "INCorrection" );
        conditionsTag.appendChild( uncorrTag );

        tag = resultDoc.createElement( "Amplitude" );
        uncorrTag.appendChild( tag );
        t = resultDoc.createTextNode(QString("%1").arg(fabs(ActValues.UInCorr),7,'f',3));
        tag.appendChild( t );

        tag = resultDoc.createElement( "Angle" );
        uncorrTag.appendChild( tag );
        t = resultDoc.createTextNode(QString("%1").arg(UserAtan(ActValues.UInCorr.im(),ActValues.UInCorr.re()),7,'f',3));
        tag.appendChild( t );

        rfile.remove();
        if (rfile.open( QIODevice::WriteOnly ) ) {
            QString xml = resultDoc.toString();
            QTextStream stream( &rfile );
            stream << xml;
            rfile.close();
        }
    }
}


bool cWM3000I::TestOverload(cWMRangeList& RangeList, QString& sRange, float max)
{
    CWMRange* prng = Range(sRange,RangeList); // zeiger auf akt. bereich
    return ( (max > (1.20 * 1.414 * prng->Rejection())) || (max ==8388608) );
}


bool cWM3000I::SelectRange(cWMRangeList& RangeList, QString& sRange, QString& sRangeSoll,QString& sRangeVorgabe,float max, bool &ovl)
{
    CWMRange* prng = Range(sRange,RangeList); // zeiger auf akt. bereich
    if ( (max > (1.20 * 1.414 * prng->Rejection())) || (max ==8388608) ) { // max überschritten -> größter bereich wird sollbereich
        ovl = true; // übersteuerung eintragen
        sRangeSoll = RangeList.first()->Name();
        if ( sRangeVorgabe != "Auto" ) sRangeVorgabe = sRangeSoll; // wir ändern auch den vorgabe wert falls wir keine automatik haben
        return (sRange != sRangeSoll); // muss bearbeitet werden
    }
    
    // const float LinearityLimit = 0.1;
    // kommt jetzt individuell aus bereichinfo
    // 10% linearitäts limit.... ist der wert oberhalb dieser grenze nehmen wir den wert, ansonsten nehmen wir die grenze
    if (sRangeVorgabe == "Auto") { // wir haben automatik
        ovl = false; // übersteuerung eintragen
        float minRValue = (max > (prng->LinearityLimit() * prng->Rejection()*1.414) ) ? (prng->Value() * max / (prng->Rejection() * 1.414)) : (prng->Value() * prng->LinearityLimit());
        //  der wert für den der bereich gesucht wird ist max. die linearitätsgrenze
        CWMRange* foundRange = Range(minRValue,RangeList);
        if ( foundRange->Value() < prng->Value() )
            sRangeSoll = foundRange->Name();
        return (sRange != sRangeSoll); // dito
    }

    return false;
}


CWMRange* cWM3000I::Range(cRSelectString selector,cWMRangeList &rlist)  // sucht den bereich in der liste zu selector
{    
    Q3PtrListIterator<CWMRange> it(rlist);
    CWMRange *range;
    
    while ( (range = it.current()) != 0 ) {
        ++it;
        if ( (selector == range->Selector()) || (selector == range->Selector().upper()) ) break;
    }
    if (range == 0)
        range = DummyRange;
    return range;
}


CWMRange* cWM3000I::Range(QString name,cWMRangeList &rlist) // sucht bereich in der liste zu name
{    
    Q3PtrListIterator<CWMRange> it(rlist);
    CWMRange *range;
    
    while ( (range = it.current()) != 0 ) {
        ++it;
        if ( (name == range->Name()) || ( name == range->Name().upper()) ) break;
    }
    if (range == 0)
        range = DummyRange;
    return range;
}


CWMRange* cWM3000I::Range(float mw,cWMRangeList& rlist) 
{
    Q3PtrListIterator<CWMRange> it(rlist);
    CWMRange *range;
    
    it.toLast(); // zeigt auf den "kleinsten bereich
    while ( (range = it.current()) != 0 ) {
        --it;
        if ( mw <= range->Value() ) break;
    }
    if (range == 0) range = rlist.first(); // der 1. bereich ist der grösste
    return range;
}


int cWM3000I::getSampleRate(int sr)
{
    int SRates[5]={80,96,240,256,288};
    return SRates[sr];
}


// habe die reihenfolge der variablen so geändert, dass wenn sich das samplingsystem ändert
// die indices und ergebnisse an den gleichen stellen bleiben
// alternative wäre gewesen generell das programm
void cWM3000I::SetDspWMVarList() // variablen des dsp zusammenbauen 
{
    if (!m_ConfData.m_bSimulation) {
        int sampleCount = getSampleRate(m_ConfData.m_nSRate);
        m_dspSetup.setDspVarList(&m_ConfData, DspIFace, sampleCount);
    }
}

void cWM3000I::SetDspWMCmdList()
{
    if (!m_ConfData.m_bSimulation) {
        int sampleCount = getSampleRate(m_ConfData.m_nSRate);
        m_dspSetup.SetDspWMCmdList(&m_ConfData, DspIFace, sampleCount, m_ConfData.m_nMeasMode != In_nConvent);
    }
}


void cWM3000I::StopMeasurement()
{
    m_rangeTimer.stop();
    m_measureTimer.stop();
    m_measureLPTimer.stop();
    AHSFifo.remove(TriggerMeasureStart); // alle trigger measure event löschen
    AHSFifo.remove(MeasureStart); // alle measure event löschen
    AHSFifo.remove(MeasureLPStart);
    AHSFifo.remove(RangeObsermaticStart); // alle events zur bereichüberwachung löschen
    m_bStopped = true;
    m_bDspMeasureIgnore = true; // m_bDspMeasureTriggerActive; // wenn der trigger schon raus ist ignorieren wir die ergebnisse
    ActValues.bvalid = false; // wir schalten die fehleranzeige inaktiv
    emit SendActValuesSignal(&ActValues);
}


// simulationsdaten werden so generiert dass der benutzer und damit der programmierer feststellen kann
// ob die durchgeführten berechnungen richtig sind:
// die frequenz ist immer +5% neben der eingestellten signalfrequenz
// die amplitude ist immer zwischwn +/- 2% vom eingestellten Un
// der fehler des x wandlers ist immer 0
// durch verändern der eigenfehlertabelle und durch prüfling manipulation (abtastverzögerung und phasenfehler
// im konfigurationsmenu)  können die berechnungen kontrolliert werden.

void cWM3000I::SimulatedMeasurement()
{
    ActValues.dspActValues.kfkorrf = 1.005; // wenn 50 -> f =50.25
    ActValues.dspActValues.tdsync = 0.0;
    
    double rnd = 1.0+ (0.02 * random() / RAND_MAX);
    
    WmParameter e = m_ConfData.m_NSecondary;

    ActValues.dspActValues.rmsnf = e.toDouble() * rnd;
    CWMRange* r = Range(m_ConfData.m_sRangeN,m_sNRangeList);
    ActValues.dspActValues.rmsnf *= r->Rejection()/r->Value();
    ActValues.RMSN1Sek = ActValues.dspActValues.rmsnf;
    ActValues.dspActValues.ampl1nf = ActValues.dspActValues.rmsnf * 1.414213562; // klirrfaktor = 0
    ActValues.dspActValues.phin = 0.0+PI/2; // vektor hat 0°
    
    float rej,val;
    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case In_IxAbs:
        e =  m_ConfData.m_XSecondary;
        r = Range(m_ConfData.m_sRangeX,m_sXRangeList);
        val = r->Value();
        rej = r->Rejection();
        ActValues.dspActValues.rmsxf = e.toDouble() * rnd;
        break;
    case In_IxDiff:
        e =  m_ConfData.m_XSecondary;
        r = Range(m_ConfData.m_sRangeX,m_sXRangeList);
        val = r->Value();
        rej = r->Rejection();
        ActValues.dspActValues.rmsxf = 0.0; // differenz vektor = 0
        break;
    case  In_ECT:
        e =  m_ConfData.m_ETSecondary;
        r = Range(m_ConfData.m_sRangeET,m_sECTRangeList);
        val = r->Value();
        rej = r->Rejection();
        ActValues.dspActValues.rmsxf = e.toDouble() * rnd;
        break;
    case In_nConvent:

        e =  m_ConfData.m_XSecondary;
        val = 1.0;
        rej = 1e-3;
        ActValues.dspActValues.rmsxf = e.toDouble() * rnd;
    default:
        val = r->Value();
        rej = r->Rejection();
    }
    
    ActValues.dspActValues.rmsxf *= rej/val;
    ActValues.RMSX1Sek = ActValues.dspActValues.rmsxf;
    ActValues.dspActValues.ampl1xf = ActValues.dspActValues.rmsxf * 1.414213562; // klirrfaktor = 0
    ActValues.dspActValues.phix = 0.0+PI/2; // vektor hat 0°

    ActValues.dspActValues.dphif = 0.0; // differenz vektor hat 0°
    MaxValues.maxn = ActValues.dspActValues.ampl1nf;
    MaxValues.maxx = ActValues.dspActValues.ampl1xf;
    MaxValues.maxRdy = 1.0; // maxima sind verfügbar
    
    CmpActValues();
    CmpRMSValues();
    emit SendActValuesSignal(&ActValues);
    emit SendLPSignal(&ActValues);
}

void cWM3000I::CmpActFrequency()
{

    double fsoll;
    switch (m_ConfData.m_nSFreq)
    {
    case F16 : fsoll = 50.0/3.0;break;
    case F50 : fsoll = 50.0;break;
    case F60 : fsoll = 60.0;
    }

    ActValues.Frequenz = m_ConfData.m_fSFreq * ActValues.dspActValues.kfkorrf;
    if (ActValues.Frequenz < 6.0) //
        ActValues.Frequenz = 0.0;


    bool bFreqQuestionable = (fabs(ActValues.Frequenz-fsoll) > 1.0);
    emit FreqQuestionable(bFreqQuestionable);
    if (bFreqQuestionable)
        emit AffectStatus(SetQuestStat, QuestFrequency); // questionable status setzen
    else
        emit AffectStatus(ResetQuestStat, QuestFrequency);
}


void cWM3000I::CmpActValues() {  // here we will do all the necessary computations
    // korrektur der rohwinkel weil fft phi = 0 für cosinus
    
    ActValues.PHIN = normWinkelrad02PI(ActValues.dspActValues.phin - PI/2);  // winkel zwischen 0 und 2PI
    ActValues.PHIX = normWinkelrad02PI(ActValues.dspActValues.phix - PI/2); // wenn IxdIx PHIX ist der winkel von dIX !!!!!!!
    
    // frequenz und abtastverzögerung berechnen

    CmpActFrequency();
    ActValues.TDSync = ActValues.dspActValues.tdsync / TDBase;

    CWMRange* r = Range(m_ConfData.m_sRangeN,m_sNRangeList);

    double val,rej,im,re;
    val = r->Value();
    rej = r->Rejection();
    
    // setzen der übersetzungsverhältnisse
    
    WmParameter PrimN, PrimX;
    WmParameter SekN, SekX;
    
    PrimN = m_ConfData.m_NPrimary;
    SekN = m_ConfData.m_NSecondary;
    double kn = PrimN.toDouble() / SekN.toDouble();
    
    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case In_IxDiff:
    case In_IxAbs:
    case In_nConvent: // wir brauchen jetzt das ü-verhältnis um die sekundär grösse zu ermitteln
        PrimX = m_ConfData.m_XPrimary;
        SekX = m_ConfData.m_XSecondary;
        break;
    case In_ECT:
        PrimX = m_ConfData.m_ETPrimary;
        SekX = m_ConfData.m_ETSecondary;
        break;
    }
    
    double kx = PrimX.toDouble() / SekX.toDouble();
    
    // berechnen der sekundär grössen
    // amplitude n,x sind gefilterte messwerte, die winkel nicht da feste abtastfrequenz

    re = ( val * ActValues.dspActValues.ampl1nf ) / rej;
    ActValues.VekNSek = complex (re,0.0); // der winkel für vekn ist 0 .... definition !!!

    // wir haben u.u. eine offsetkorrektur für kanal n "A" die wir nach der skalierung berücksichtigen müssen

    if (m_ConfData.m_bOffsetCorrectionN && m_ConfData.m_bDCmeasurement)
        ActValues.VekNSek -= m_JustValues.OffsetCorrDevN;

    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case In_IxDiff:
    case In_IxAbs:
        r = Range(m_ConfData.m_sRangeX,m_sXRangeList);
        val = r->Value();
        rej = r->Rejection();
        break;
    case In_ECT:
        r = Range(m_ConfData.m_sRangeET,m_sECTRangeList);
        val = r->Value();
        rej = r->Rejection();
        break;
    case In_nConvent:
        val = 1e-3; // 1lsb -> 1mA
        rej = kx; // wir wollen die sekundär grösse des nConvent haben ... neue forderung dies anzuzeigen
        break;
    }
    
    re = im = ( val * ActValues.dspActValues.ampl1xf ) / rej;
    im *= sin(ActValues.dspActValues.dphif);
    re *= cos(ActValues.dspActValues.dphif);
    
    switch (m_ConfData.m_nMeasMode)
    {
    case In_IxDiff:
        ActValues.VekDXSek = complex(re,im); // dsp hat den differenz vektor gemessen
        ActValues.VekXSek = ActValues.VekDXSek + ActValues.VekNSek; // wir berechnen den x vektor
        break;
    case In_IxAbs:
    case In_ECT:
    case In_nConvent: // durch setzen rej = kx bei nconvent haben wir auch hier den sekundär vektor
        // ActValues.VekDX = complex(0.0,0.0);
        ActValues.VekXSek = complex(re,im);
        ActValues.VekDXSek = ActValues.VekXSek - ActValues.VekNSek;
        break;
    }

    if (m_ConfData.m_bOffsetCorrectionX && m_ConfData.m_bDCmeasurement)
        ActValues.VekXSek -= m_JustValues.OffsetCorrDevX;

    // alle winkel werden vom dsp ermittelt und  dphi ist schon mit tdsync frequenzabhängig korrigiert
    //  die vektoren n und x würden normalerweise mit der differenzfrequenz aus abtast- und signalfrequenz
    // rotieren, wir legen vekn auf die ordinate und vekx hat den winkel dphif = konstant
    // winkel korrekturen aus abtastverzögerungen und phasenkorrekturwerten

    double phik;
    //  korrektur des kanal X vektors mit der bekannten abtastverzögerung und dem bekannten phasenfehler des prüflings
    phik = ( ( -360.0  * ActValues.Frequenz * m_ConfData.m_fxTimeShift * 1.0e-3 ) - m_ConfData.m_fxPhaseShift) * PI_180;
    
    ActValues.VekXSek *= complex( cos(phik),sin(phik) );

    // eigenfehler korrektur des normwandlers

    ActValues.UInCorr = m_ownError.GetOECorrVector(); // achtung complex !!!!!
    ActValues.VekNSek *= ActValues.UInCorr;

    // umrechnen auf primärgrößen

    ActValues.VekN = ActValues.VekNSek * kn;
    ActValues.VekX = ActValues.VekXSek * kx;
    ActValues.VekDX = ActValues.VekX - ActValues.VekN;

    //  fehler berechnung

    ActValues.AngleError = UserAtan(ActValues.VekX) - UserAtan(ActValues.VekN);
    ActValues.AngleError = normWinkelrad_PIPI(ActValues.AngleError);

    double err;

    if (m_ConfData.m_bDCmeasurement)
        err = (ActValues.VekX.re() - ActValues.VekN.re()) / ActValues.VekN.re();
    else
    {
        double absN;
        absN = fabs(ActValues.VekN);
        err = (fabs(ActValues.VekX) -absN) / absN;
    }

    ActValues.AmplErrorIEC = 100.0 * err;
    ActValues.RCF = 1.0 / (1.0 + err);
    ActValues.AmplErrorANSI = (ActValues.AmplErrorIEC/100.0 + ( (1.0+ActValues.AmplErrorIEC/100.0) * (4.0 / 3.0) * ActValues.AngleError )) * 100.0;

    ActValues.bvalid = true; // aktivieren der fehleranzeige
}


void cWM3000I::CmpRMSValues()
{
    CWMRange* range;
    double val,rej;
    double kn,kx;

    // setzen der übersetzungsverhältnisse

    WmParameter PrimN, PrimX;
    WmParameter SekN, SekX;

    PrimN = m_ConfData.m_NPrimary;
    SekN = m_ConfData.m_NSecondary;
    kn = PrimN.toDouble() / SekN.toDouble();

    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case In_IxDiff:
    case In_IxAbs:
    case In_nConvent: // wir brauchen jetzt das ü-verhältnis um die sekundär grösse zu ermitteln
        PrimX = m_ConfData.m_XPrimary;
        SekX = m_ConfData.m_XSecondary;
        break;
    case In_ECT:
        PrimX = m_ConfData.m_ETPrimary;
        SekX = m_ConfData.m_ETSecondary;
        break;
    }

    kx = PrimX.toDouble() / SekX.toDouble();

    range = Range(m_ConfData.m_sRangeN,m_sNRangeList);
    val = range->Value();
    rej = range->Rejection();

    ActValues.RMSNSek = ( val * ActValues.dspRMSValues.fastRMSN ) / rej;
    ActValues.RMSN1Sek = ( val * ActValues.dspRMSValues.fastRMSN1 ) / rej;

    // wir haben u.u. eine offsetkorrektur für kanal n in "A" die wir nach der skalierung berücksichtigen müssen
    if (m_ConfData.m_bOffsetCorrectionN && m_ConfData.m_bDCmeasurement)
        ActValues.RMSNSek -= fabs(m_JustValues.OffsetCorrDevN);

    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case In_IxDiff:
    case In_IxAbs:
        range = Range(m_ConfData.m_sRangeX,m_sXRangeList);
        val = range->Value();
        rej = range->Rejection();
        break;
    case In_ECT:
        range = Range(m_ConfData.m_sRangeET,m_sECTRangeList);
        val = range->Value();
        rej = range->Rejection();
        break;
    case In_nConvent:
        val = 1e-3; // 1lsb -> 1mA
        rej = kx; // wir wollen die sekundär grösse des nConvent haben ... neue forderung dies anzuzeigen
        break;
    }

    ActValues.RMSXSek = ( val * ActValues.dspRMSValues.fastRMSX ) / rej;
    ActValues.RMSX1Sek = ( val * ActValues.dspRMSValues.fastRMSX1 ) / rej;

    if (m_ConfData.m_nMeasMode == In_IxDiff)
    {
        ActValues.RMSX1Sek = ActValues.RMSN1Sek + ActValues.RMSX1Sek;
        ActValues.RMSXSek = ActValues.RMSNSek + ActValues.RMSXSek;
    }

    // wir haben u.u. eine offsetkorrektur für kanal x in "A" die wir nach der skalierung berücksichtigen müssen
    if (m_ConfData.m_bOffsetCorrectionX && m_ConfData.m_bDCmeasurement)
        ActValues.RMSXSek -= fabs(m_JustValues.OffsetCorrDevX);

    // eigenfehler korrektur des normwandlers

    ActValues.UInCorr = m_ownError.GetOECorrVector(); // achtung complex !!!!!
    ActValues.RMSNSek *= fabs(ActValues.UInCorr);
    ActValues.RMSN1Sek *= fabs(ActValues.UInCorr);

    // umrechnen auf primärgrößen

    ActValues.RMSN = ActValues.RMSNSek * kn;
    ActValues.RMSN1 = ActValues.RMSN1Sek * kn;

    ActValues.RMSX = ActValues.RMSXSek * kx;
    ActValues.RMSX1 = ActValues.RMSX1Sek * kx;

    //  lastpunkt berechnung

    ActValues.LoadPoint = 100.0 * ActValues.RMSN/PrimN.toDouble();
    ActValues.LoadPoint1 = 100.0 * ActValues.RMSN1/PrimN.toDouble();
    ActValues.LoadPointX = 100.0 * ActValues.RMSN/PrimX.toDouble();
    ActValues.LoadPoint1X = 100.0 * ActValues.RMSN1/PrimX.toDouble();
}


void cWM3000I::CorrActValues() {  // here we will do all the necessary corrections with use of adjustment values
    const float PI_180 = 0.017453293;
    int sign;
    double offsetCorr;
    QString key;
    CWMRange* range;

    ActValues.dspActValues.rmsnf *= m_JustValues.GainCorrCh0;
    ActValues.dspActValues.ampl1nf *= m_JustValues.GainCorrCh0;
    ActValues.dspActValues.phin += m_JustValues.PhaseCorrCh0 * PI_180;
    ActValues.dspActValues.dphif += m_JustValues.PhaseCorrCh0 * PI_180;

    if (m_ConfData.m_bDCmeasurement)
    {
        // wir korrigieren die offsetwerte aus der permanenten offset korrektur
        // im rms können wir aber das vorzeichen nicht mehr sehen .....
        sign = signum(ActValues.dspActValues.ampl1nf);
        ActValues.dspActValues.rmsnf = sign * ActValues.dspActValues.rmsnf + m_JustValues.OffsetCorrCh0;
        ActValues.dspActValues.ampl1nf /= 2.0;
        ActValues.dspActValues.ampl1nf += m_JustValues.OffsetCorrCh0;

        // wir korrigieren die offsetwerte aus der temp. offset korrektur
        range = Range(m_ConfData.m_sRangeN, m_sNRangeList);
        if (measOffsetCorrectionHash.contains(key = range->getOffsKorrKey()))
            offsetCorr = measOffsetCorrectionHash[key];
        else
            offsetCorr = 0.0;

        ActValues.dspActValues.rmsnf = fabs(ActValues.dspActValues.rmsnf + offsetCorr);
        ActValues.dspActValues.ampl1nf += offsetCorr;
    }

    if ( m_ConfData.m_nMeasMode != In_nConvent)
    { // wenn nicht nconvent
        ActValues.dspActValues.rmsxf *= m_JustValues.GainCorrCh1; // x kanal auch korrigieren
        ActValues.dspActValues.ampl1xf *= m_JustValues.GainCorrCh1;
        ActValues.dspActValues.phix += m_JustValues.PhaseCorrCh1 * PI_180;
        ActValues.dspActValues.dphif -= m_JustValues.PhaseCorrCh1 * PI_180;

        if (m_ConfData.m_bDCmeasurement)
        {
            // wir korrigieren die offsetwerte aus der permanenten offset korrektur
            sign = signum(ActValues.dspActValues.ampl1xf);
            ActValues.dspActValues.rmsxf = sign * ActValues.dspActValues.rmsxf + m_JustValues.OffsetCorrCh1;
            ActValues.dspActValues.ampl1xf /= 2.0;
            ActValues.dspActValues.ampl1xf += m_JustValues.OffsetCorrCh1;

            // wir korrigieren die offsetwerte aus der temp. offset korrektur
            switch (m_ConfData.m_nMeasMode)
            {
            case In_IxAbs:
            case In_IxDiff:
                range = Range(m_ConfData.m_sRangeX, m_sXRangeList);
                break;
            case In_ECT:
                range = Range(m_ConfData.m_sRangeET, m_sECTRangeList);
                break;
            }

            if (measOffsetCorrectionHash.contains(key = range->getOffsKorrKey()))
                offsetCorr = measOffsetCorrectionHash[key];
            else
                offsetCorr = 0.0;

            ActValues.dspActValues.rmsxf = fabs(ActValues.dspActValues.rmsxf + offsetCorr);
            ActValues.dspActValues.ampl1xf += offsetCorr;
        }
    }
}


void cWM3000I::CorrRMSValues()
{
    int sign;
    double offsetCorr;
    QString key;
    CWMRange* range;

    ActValues.dspRMSValues.fastRMSN *= m_JustValues.GainCorrCh0;
    ActValues.dspRMSValues.fastRMSN1 *= m_JustValues.GainCorrCh0;

    if (m_ConfData.m_bDCmeasurement)
    {
        // wir korrigieren die offsetwerte aus der permanenten offset korrektur
        sign = signum(ActValues.dspRMSValues.fastRMSN1);
        ActValues.dspRMSValues.fastRMSN = sign * ActValues.dspRMSValues.fastRMSN + m_JustValues.OffsetCorrCh0;
        ActValues.dspRMSValues.fastRMSN1 = sign * ActValues.dspRMSValues.fastRMSN1 + m_JustValues.OffsetCorrCh0;

        // wir korrigieren die offsetwerte aus der temp. offset korrektur
        range = Range(m_ConfData.m_sRangeN, m_sNRangeList);
        if (measOffsetCorrectionHash.contains(key = range->getOffsKorrKey()))
            offsetCorr = measOffsetCorrectionHash[key];
        else
            offsetCorr = 0.0;

        ActValues.dspRMSValues.fastRMSN = fabs(ActValues.dspRMSValues.fastRMSN + offsetCorr);
        ActValues.dspRMSValues.fastRMSN1 = fabs(ActValues.dspRMSValues.fastRMSN1 + offsetCorr);
    }

    if (m_ConfData.m_nMeasMode != In_nConvent) // für nconvent die korrektur nicht berücksichtigen
    {
        ActValues.dspRMSValues.fastRMSX *= m_JustValues.GainCorrCh1;
        ActValues.dspRMSValues.fastRMSX1 *= m_JustValues.GainCorrCh1;

        if (m_ConfData.m_bDCmeasurement)
        {
            // wir korrigieren die offsetwerte aus der permanenten offset korrektur
            sign = signum(ActValues.dspRMSValues.fastRMSX1);
            ActValues.dspRMSValues.fastRMSX = sign * ActValues.dspRMSValues.fastRMSX + m_JustValues.OffsetCorrCh1;
            ActValues.dspRMSValues.fastRMSX1 = sign * ActValues.dspRMSValues.fastRMSX1 + m_JustValues.OffsetCorrCh1;

            // wir korrigieren die offsetwerte aus der temp. offset korrektur
            switch (m_ConfData.m_nMeasMode)
            {
            case In_IxAbs:
            case In_IxDiff:
                range = Range(m_ConfData.m_sRangeX, m_sXRangeList);
                break;
            case In_ECT:
                range = Range(m_ConfData.m_sRangeET, m_sECTRangeList);
                break;
            }

            if (measOffsetCorrectionHash.contains(key = range->getOffsKorrKey()))
                offsetCorr = measOffsetCorrectionHash[key];
            else
                offsetCorr = 0.0;

            ActValues.dspRMSValues.fastRMSX = fabs(ActValues.dspRMSValues.fastRMSX + offsetCorr);
            ActValues.dspRMSValues.fastRMSX1 = fabs(ActValues.dspRMSValues.fastRMSX1 + offsetCorr);
        }
    }
}
