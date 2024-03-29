// headerdatei dspiface.h 
// deklaration einer dsp interface klasse

#ifndef DSPIFACE_H
#define DSPIFACE_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3ptrlist.h>
#include <Q3MemArray>

#include "tools.h"
#include "ethadress.h"
#include "dspvar.h"
#include "dspmeasdata.h"

#include "zhserveriface.h"
#include "zhclientsocket.h"


enum dspIFaceActionHandlerState {
    dspIFaceIdle, // 0
    
    dspIFaceConnectYourselfStart,
    dspIFaceConnectionFinished,
    
    BootDspStart,
    BootDspTestRunning,
    BootDspReset,
    BootDspSetPath,
    BootDspExecute,
    BootDspFinished,
    
    SetSamplingSystemStart,
    SetSamplingSystemFinished, // 10
    
    VarLists2DspStart,
    VarLists2DspFinished,
    
    CmdLists2DspStart,
    IntCmdList2Dsp,
    CmdLists2DspFinished,
    
    SetSignalRoutingStart,
    SetSignalRoutingFinished,
    
    ResetMaximumStart,
    ResetMaximumExec,
    ResetMaximumFinished, // 20
    
    SetDsp61850SourceAdrStart,
    SetDsp61850SourceAdrExec,
    SetDsp61850SourceAdrWaitAutoNeg,
    SetDsp61850SourceAdrFinished,
    
    SetDsp61850DestAdrStart,
    SetDsp61850DestAdrExec,
    SetDsp61850DestAdrWaitAutoNeg,
    SetDsp61850DestAdrFinished,
    
    SetDsp61850PriorityTaggedStart,
    SetDsp61850PriorityTaggedExec, // 30
    SetDsp61850PriorityTaggedFinished,
    
    SetDsp61850EthTypeAppIdStart, 
    SetDsp61850EthTypeAppIdExec,
    SetDsp61850EthTypeAppIdFinished,
       
    SetDsp61850EthSyncStart,
    SetDsp61850EthSyncExec,
    SetDsp61850EthSyncFinished,
    
    TriggerIntHKSKStart,
    TriggerIntHKSKExec,
    TriggerIntHKSKFinished, // 40
    
    DataAcquisitionStart, 
    DataAcquisitionFinished,

    DspMemoryReadLongStart,
    DspMemoryReadLongFinished,

    DspMemoryReadStart,
    DspMemoryReadFinished,
    
    DspMemoryWriteStart,
    DspMemoryWriteFinished,
    
    ActivateInterfaceStart,
    ActivateInterfaceFinished,
    
    DeactivateInterfaceStart,
    DeactivateInterfaceFinished,
    
    ReadDeviceVersionStart,
    ReadDeviceVersionFinished,
    
    ReadServerVersionStart,
    ReadServerVersionFinished,
    
    dspIFaceContinue
};


class cDspIFace: public cZHServerIFace
{
    Q_OBJECT
    
public:
    cDspIFace(QString, QString, int);
    ~cDspIFace();
    void connectYourself(); // verbinde dich mit deinem server
    bool connected();
    void BootDsp(); // aussführen des bootvorgangs  
    void SetSamplingSystem(int,int,int); // anzahl kanäle, samples/signalperiode, samples/messperiode
    void VarList2Dsp(); // variablenlisten zum server senden
    void CmdLists2Dsp(); // sendet cyc- und intliste ad den dsp server
    void ClearCmdList(); // löscht alle cmdlisten
    void ClearVarLists(); // löscht die varliste
    void ClearMemLists(); 
    void SetSignalRouting(ulong*); // zeiger auf ein 8*32bit routing feld 
    void SetDsp61850SourceAdr(cETHAdress&);
    void SetDsp61850DestAdr(cETHAdress&);
    void SetDsp61850PriorityTagged(ulong); // setzt die einträge für den eth frame decoder
    void SetDsp61850EthTypeAppId(ulong); // dito
    void SetDsp61850EthSynchronisation(ulong); // setzt ob harte sync. und auf welches dataset
    void ResetMaximum(); // löscht den maximum speicher
    void TriggerIntHKSK(int); // triggert die abarbeitung hksk in intliste
    void addCycListItem(QString&); // ein neues Kommando ans ende der cyc. liste
    void addIntListItem(QString&); // dito für intliste
    cDspMeasData<float>* GetMVHandle(QString); // legt eine neue messwerte gruppe an
    cDspMeasData<quint32> *GetMemHandle(QString); // legt eine neue memory gruppe an
    void ActivateInterface(); // aktiviert die var- und cmd-listen im dsp
    void DeactivateInterface(); // nur anders rum
    void DataAcquisition(cDspMeasData<float>*); // liest alle daten dieser messwertegruppe vom type vapplication
    void DspMemoryRead(cDspMeasData<float>*); // liest alle daten dieser memorygruppe
    void DspMemoryRead(cDspMeasData<quint32> *);
    void DspMemoryWrite(cDspMeasData<float>*); // schreibt alle daten dieser memorygruppe
    void DspMemoryWrite(cDspMeasData<quint32>*);
    float* data(cDspMeasData<float>*); // gibt einen zeiger zurück auf die var daten vom typ vapplication
    quint32 *data(cDspMeasData<quint32> *);
    void SetGainCorrection(int,float); // setzt für kanal (int 0..) die verstärkungskorrektur
    void SetPhaseCorrection(int,float); // setzt für kanal (int 0..) die phasenkorrektur    
    void SetOffsetCorrection(int,float); // setzt für kanal (int 0..) die offsetkorrektur
    bool IFaceError(); // abfrage ob bei der letzten aktion fehler aufgetreten ist
    void ReadDeviceVersion();
    void ReadServerVersion();
    
    
protected slots:
    void ActionHandler(int);
    virtual void SocketConnectedSlot();
    virtual void SocketDoneSlot();    
    virtual void SocketErrorSlot();
    
private:
    QString dspBootPath; // vollständiger programmname
    QString m_sHost; // host name
    int m_nPort; // host port
    QStringList CycCmdList, IntCmdList;
    QList<cDspMeasData<float>*> DspMeasDataList; // eine liste mit zeigern auf "programmdaten"
    QList<cDspMeasData<quint32>*> DspMemoryDataList; // eine liste mit zeigern auf  dsp speicher allgemein
    cSMTimer* m_ActTimer;
    void GetInterfaceData();
    void GetInterfaceLongData();
    void TestDspRunning(); // schreibt test kommando an dsp
    void SetDspBootPath(); // schreibt kommando für bootpath zu setzen
    void DspReset(); 
    void SendBootCommand(); // sendet das bootkommando an den server
    void SendSamplingSystemCommand(); // na was wohl ?
    void SendVarListCommand(); // und hier ????
    void SendCmdListCommand();
    void SendCmdIntListCommand();
    void SendSignalRoutingCommand();
    void SendResetMaximumCommand();
    void SendSetDsp61850SourceAdrCommand();
    void SendSetDsp61850DestAdrCommand();
    void SendSetDsp61850PriorityTaggedCommand();
    void SendSetDsp61850EthTypeAppIdCommand();
    void SendSetDsp61850EthSyncCommand();
    void SendTriggerIntHKSKCommand();
    void SendDataAcquisitionCommand();
    void SendDspMemoryReadLongCommand();
    void SendDspMemoryReadCommand();
    void SendDspMemoryWriteCommand();
    void SendActivateInterfaceCommand();
    void SendDeactivateInterfaceCommand();
    void SendReadDeviceVersionCommand();
    void SendReadServerVersionCommand();
    
    bool m_biFaceError;
    int m_nP1, m_nP2, m_nP3; // parameter
    ulong m_nuP1;
    ulong m_lP1[16]; // parameter
    cETHAdress m_ethadr;
    cDspMeasData<float> *m_pMeasData;
    cDspMeasData<quint32> *m_pMeasDataUlong;
    int m_nBusyCount;
    bool m_bConnected;
};
    
#endif
    
