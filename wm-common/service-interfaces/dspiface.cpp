// implementation cDspIFace

#include "tools.h"
#include "dspiface.h"


cDspIFace::cDspIFace(QString bp, QString hn, int hp)
    : cZHServerIFace(hn, hp, 5000) // hostname, hostport, 5000ms timeout
{
    m_bConnected = false;
    QObject::connect(iFaceSock,SIGNAL(DataReceived()),this,SLOT(SocketDoneSlot()));
    QObject::connect(iFaceSock,SIGNAL(connected()),this,SLOT(SocketConnectedSlot()));
    QObject::connect(iFaceSock,SIGNAL(Error()),this,SLOT(SocketErrorSlot()));
    QObject::connect(iFaceSock,SIGNAL(AsyncDataReceived(const QString&)),this,SIGNAL(iFaceAsync(const QString&))); // dieses Signal reichen wir einfach durch
    dspBootPath = bp;
    m_sHost = hn;
    m_nPort = hp;
    //DspMeasDataList.setAutoDelete( true ); // the list owns the objects
    //DspMemoryDataList.setAutoDelete( true ); // the list owns the objects
    m_ActTimer = new cSMTimer();
    QObject::connect(m_ActTimer,SIGNAL(timeout(int)),this,SLOT(ActionHandler(int)));
}


cDspIFace::~cDspIFace()
{
    iFaceSock->close();
    ClearVarLists();
    ClearMemLists();
    delete m_ActTimer;
}


void cDspIFace::ActionHandler(int entryAHS)
{
    static int AHS = dspIFaceIdle;
    
    if ( entryAHS != dspIFaceContinue ) { // bei continue machen wir beim internen state weiter
        if (AHS != dspIFaceIdle) { // wir sollen was neues starten
            qDebug ("DspIFace reentered with new AHS=%d while AHS=%d , check your program !",entryAHS, AHS); // das darf eigentlich nicht passieren
            return; // und sind fertig
        }
        else
        {
            m_biFaceError = false; // wir haben noch keinen fehler
            AHS = entryAHS; // wir starten es
        }
    }
    
    switch (AHS)
    {
    case dspIFaceConnectYourselfStart:
        AHS++;
        if(iFaceSock->state() == QAbstractSocket::UnconnectedState) {
        iFaceSock->connectToHost(m_sHost,m_nPort); // verbindung zum dspserver herstellen
        }
        else {
            SocketDoneSlot();
        }
        break; // dspIFaceConnectYourselfStart

    case ReadDeviceVersionFinished:
    case ReadServerVersionFinished:
    case SetSignalRoutingFinished:
    case CmdLists2DspFinished:
    case VarLists2DspFinished:
    case BootDspFinished:
    case dspIFaceConnectionFinished:
        if (m_biFaceError)
            emit iFaceError();
        else
            emit iFaceDone();
        AHS = dspIFaceIdle; // wir sind durch
        break; // dspIFaceConnectionFinished

    case BootDspStart:
        TestDspRunning();
        AHS++;
        break; // BootDspStart

    case BootDspTestRunning:
    {
        QString s;
        if (m_biFaceError) {
            emit iFaceError();
            AHS = dspIFaceIdle;
            break; // BootDspTestRunning
        }

        s = iFaceSock->GetAnswer();
        if (s == "RUNNING") {
            emit iFaceDone(); // wir booten nicht, neu wenn der dsp schon läuft
            AHS = dspIFaceIdle;
        }

        else
        {
            AHS++;
            m_ActTimer->start(0,dspIFaceContinue); // starten müssen wir selbst
        }
        break; // BootDspTestRunning
    }

    case BootDspReset:
        DspReset();
        AHS++;
        break; // BootDspReset

    case BootDspSetPath:
        SetDspBootPath();
        AHS++;
        break; // BootDspSetPath

    case BootDspExecute:
        if (m_biFaceError) {
            emit iFaceError();
            AHS = dspIFaceIdle;
        }
        else
        {
            SendBootCommand();
            AHS++;
        }
        break; // BootDspExecute

    case SetSamplingSystemStart:
        SendSamplingSystemCommand();
        AHS++;
        break; // SetSamplingSystemStart

    case VarLists2DspStart:
        SendVarListCommand();
        AHS++;
        break; // VarLists2DspStart

    case CmdLists2DspStart:
        SendCmdListCommand();
        AHS++;
        break; // CmdLists2DspStart

    case IntCmdList2Dsp:
        if (m_biFaceError) {
            emit iFaceError();
            AHS =dspIFaceIdle;
        }
        else
        {
            SendCmdIntListCommand();
            AHS++;
        }
        break; // IntCmdList2Dsp

    case SetSignalRoutingStart:
        SendSignalRoutingCommand();
        AHS++;
        break; // SetSignalRoutingStart

    case ResetMaximumStart:
        m_nBusyCount = 0;
        AHS++;

    case ResetMaximumExec:
        SendResetMaximumCommand();
        AHS++;
        break; // ResetMaximumStart

    case TriggerIntHKSKFinished:
    case SetDsp61850EthSyncFinished:
    case SetDsp61850EthTypeAppIdFinished:
    case SetDsp61850PriorityTaggedFinished:
    case ResetMaximumFinished:
    case SetSamplingSystemFinished:
    {
        QString s;
        if (m_biFaceError) { // wenn fehler
            emit iFaceError(); // melden
            AHS = dspIFaceIdle; // und fertig
        }
        else
        {
            s = iFaceSock->GetAnswer(); // kein fehler
            if (s == "BUSY")
            {
                m_nBusyCount++;
                if (m_nBusyCount > 100) { // 100x hintereinander busy wird als fehler
                    iFaceSock->SetError(myErrDeviceBusy);
                    emit iFaceError(); // gemeldet
                    AHS = dspIFaceIdle; // und fertig
                }
                else
                {
                    AHS--; // aber bei busy wiederholen wir
                    m_ActTimer->start(10,dspIFaceContinue); // starten müssen wir selbst
                } // wir starten aber mit 10ms verzögerung
            }
            else
            {
                emit iFaceDone();
                AHS = dspIFaceIdle;
            }
        }

        break;
    } // ResetMaximumFinished

    case SetDsp61850SourceAdrStart:
        m_nBusyCount = 0;
        AHS++;

    case SetDsp61850SourceAdrExec:
        SendSetDsp61850SourceAdrCommand();
        AHS++;
        break; // SetDsp61850SourceAdrExec

    case SetDsp61850DestAdrWaitAutoNeg:
    case SetDsp61850SourceAdrWaitAutoNeg:
        AHS++;
        m_ActTimer->start(1000,dspIFaceContinue); // starten müssen wir selbst
        break;

    case SetDsp61850DestAdrFinished:
    case SetDsp61850SourceAdrFinished:
    {
        QString s;
        if (m_biFaceError) { // wenn fehler
            emit iFaceError(); // melden
            AHS = dspIFaceIdle; // und fertig
        }
        else
        {
            s = iFaceSock->GetAnswer(); // kein fehler
            if (s == "BUSY")

            {
                m_nBusyCount++;
                if (m_nBusyCount > 100)
                {
                    iFaceSock->SetError(myErrDeviceBusy);
                    emit iFaceError(); // gemeldet
                    AHS = dspIFaceIdle; // und fertig
                }
                else
                {
                    AHS--; // aber bei busy wiederholen wir
                    AHS--;
                    m_ActTimer->start(10,dspIFaceContinue); // starten müssen wir selbst
                }
            }
            else
            {
                emit iFaceDone();
                AHS = dspIFaceIdle;
            }
        }

        break; // SetDsp61850SourceAdrFinished
    }

    case SetDsp61850DestAdrStart:
        m_nBusyCount = 0;
        AHS++;

    case SetDsp61850DestAdrExec:
        SendSetDsp61850DestAdrCommand();
        AHS++;
        break; // SetDsp61850DestAdrExec

    case SetDsp61850PriorityTaggedStart:
        m_nBusyCount = 0;
        AHS++;

    case SetDsp61850PriorityTaggedExec:
        SendSetDsp61850PriorityTaggedCommand();
        AHS++;
        break; // SetDsp61850PriorityTaggedExec

    case SetDsp61850EthTypeAppIdStart:
        m_nBusyCount = 0;
        AHS++;

    case SetDsp61850EthTypeAppIdExec:
        SendSetDsp61850EthTypeAppIdCommand();
        AHS++;
        break; // SetDsp61850EthTypeAppIdExec

    case SetDsp61850EthSyncStart:
        m_nBusyCount = 0;
        AHS++;

    case SetDsp61850EthSyncExec:
        SendSetDsp61850EthSyncCommand();
        AHS++;
        break; // SetDsp61850EthSyncExec

    case TriggerIntHKSKStart:
        m_nBusyCount = 0;
        AHS++;

    case TriggerIntHKSKExec:
        SendTriggerIntHKSKCommand();
        AHS++;
        break; // TriggerIntHKSKExec

    case DataAcquisitionStart:
        SendDataAcquisitionCommand();
        AHS++;
        break;

    case DataAcquisitionFinished:
    {
        QString list;
        QStringList DataEntryList, DataList; // werte zuorden
        QString s;

        if (m_biFaceError)
            emit iFaceError();
        else
        {
            GetInterfaceData();
            emit iFaceDone();
        }

        AHS = dspIFaceIdle; // wir sind durch
        break; // DataAcquisitionFinished
    }

    case DspMemoryReadLongStart:
        SendDspMemoryReadLongCommand();
        AHS++;
        break;

    case DspMemoryReadLongFinished:
        if (m_biFaceError)
            emit iFaceError();
        else
        {
            GetInterfaceLongData();
            emit iFaceDone();
        }
        AHS = dspIFaceIdle; // wir sind durch
        break; // DspMemoryReadFinished

    case DspMemoryReadStart:
        SendDspMemoryReadCommand();
        AHS++;
        break; // DspMemoryReadStart

    case DspMemoryReadFinished:
        if (m_biFaceError)
            emit iFaceError();
        else
        {
            GetInterfaceData();
            emit iFaceDone();
        }
        AHS = dspIFaceIdle; // wir sind durch
        break; // DspMemoryReadFinished

    case DspMemoryWriteStart:
        SendDspMemoryWriteCommand();
        AHS++;
        break; // DspMemoryWriteStart

    case DeactivateInterfaceFinished:
    case ActivateInterfaceFinished:
    case DspMemoryWriteFinished:
        if (m_biFaceError)
            emit iFaceError();
        else
            emit iFaceDone();
        AHS = dspIFaceIdle; // wir sind durch
        break; // DspMemoryWriteFinished

    case ActivateInterfaceStart:
        SendActivateInterfaceCommand();
        AHS++;
        break; // ActivateInterfaceStart

    case DeactivateInterfaceStart:
        SendDeactivateInterfaceCommand();
        AHS++;
        break; // DeactivateInterfaceStart

    case ReadDeviceVersionStart:
        SendReadDeviceVersionCommand();
        AHS++;
        break;

    case ReadServerVersionStart:
        SendReadServerVersionCommand();
        AHS++;
        break;

    }
}


void cDspIFace::SocketConnectedSlot()
{
    m_bConnected = true;
    m_ActTimer->start(0,dspIFaceContinue); // statemachine wird gestartet
}



void cDspIFace::SocketDoneSlot()
{
    m_ActTimer->start(0,dspIFaceContinue); // statemachine wird gestartet
}


void cDspIFace::SocketErrorSlot()
{
    m_biFaceError = true; // fehler merken
    m_ActTimer->start(0,dspIFaceContinue); // statemachine wird gestartet
}


bool cDspIFace::connected()
{
    return m_bConnected;
}


void cDspIFace::connectYourself()
{
    m_ActTimer->start(0,dspIFaceConnectYourselfStart); // statemachine wird gestartet
}


void cDspIFace::BootDsp()
{
    m_ActTimer->start(0,BootDspStart); // statemachine wird gestartet
}


void cDspIFace::SetSamplingSystem(int n, int nss, int nsm)
{
    m_nP1 = n;
    m_nP2 = nss;
    m_nP3 = nsm;
    m_ActTimer->start(0,SetSamplingSystemStart); // statemachine wird gestartet
}


void cDspIFace::VarList2Dsp() // sendet varliste an den dsp
{
    m_ActTimer->start(0,VarLists2DspStart); // statemachine wird gestartet
}


void cDspIFace::CmdLists2Dsp() // sendet cyc- und intliste an den dsp server
{
    m_ActTimer->start(0,CmdLists2DspStart); // statemachine wird gestartet
}


void cDspIFace::ClearCmdList() // löscht alle cmdlisten
{
    CycCmdList.clear();
    IntCmdList.clear();
}


void cDspIFace::ClearVarLists() // löscht alle varlisten
{
    qDeleteAll(DspMeasDataList);
    DspMeasDataList.clear();
}


void cDspIFace::ClearMemLists() // löscht alle memorylisten
{
    qDeleteAll(DspMemoryDataList);
    DspMemoryDataList.clear();
}


void cDspIFace::SetSignalRouting(ulong* proute)
{
    for (int i = 0; i < 16; i++, proute++)
        m_lP1[i] = *proute;
    m_ActTimer->start(0,SetSignalRoutingStart); // statemachine wird gestartet
}


void cDspIFace::SetDsp61850SourceAdr(cETHAdress& ea)
{
    m_ethadr = ea;
    m_ActTimer->start(0,SetDsp61850SourceAdrStart); // statemachine wird gestartet
}


void cDspIFace::SetDsp61850DestAdr(cETHAdress& ea)
{
    m_ethadr = ea;
    m_ActTimer->start(0,SetDsp61850DestAdrStart); // statemachine wird gestartet
}


void cDspIFace::SetDsp61850PriorityTagged(ulong pt) // setzt die einträge für den eth frame decoder
{
    m_nuP1 = pt;
    m_ActTimer->start(0,SetDsp61850PriorityTaggedStart); // statemachine wird gestartet
}


void cDspIFace:: SetDsp61850EthTypeAppId(ulong eta) // dito
{
    m_nuP1 = eta;
    m_ActTimer->start(0,SetDsp61850EthTypeAppIdStart); // statemachine wird gestartet
}


void cDspIFace::SetDsp61850EthSynchronisation(ulong sy)
{
    m_nuP1 = sy;
    m_ActTimer->start(0,SetDsp61850EthSyncStart); // statemachine wird gestartet
}


void cDspIFace::ResetMaximum()	     
{
    m_ActTimer->start(0,ResetMaximumStart);
}


void cDspIFace::TriggerIntHKSK(int hksk) // triggert die abarbeitung hksk in intliste
{
    m_nP1 = hksk;
    m_ActTimer->start(0,TriggerIntHKSKStart);
}


void cDspIFace::addCycListItem(QString &s) // ein neues Kommando ans ende der cyc. liste
{
    CycCmdList.push_back(s);
}


void cDspIFace::addIntListItem(QString &s) // dito für intliste
{
    IntCmdList.push_back(s);
}


cDspMeasData<float>* cDspIFace::GetMVHandle(QString s) // legt eine neue messwerte gruppe für befehlketten an
{
    cDspMeasData<float>* pdmd = new cDspMeasData<float>(s); // neues object anlegen
    DspMeasDataList.append(pdmd); // an ptr liste hängen
    return pdmd; // handle rückgabe
}


cDspMeasData<quint32> *cDspIFace::GetMemHandle(QString s)
{
    cDspMeasData<quint32>* pdmd = new cDspMeasData<quint32>(s); // neues object anlegen
    DspMemoryDataList.append(pdmd); // an ptr liste hängen
    return pdmd; // handle rückgabe
}

void cDspIFace::ActivateInterface() // aktiviert die var- und cmd-listen im dsp
{
    m_ActTimer->start(0,ActivateInterfaceStart);
}


void cDspIFace::DeactivateInterface() // nur anders rum
{
    m_ActTimer->start(0,DeactivateInterfaceStart);
}


void cDspIFace::DataAcquisition(cDspMeasData<float> *pMData) // liest alle daten vom type vapplication
{
    m_pMeasData = pMData;
    m_ActTimer->start(0,DataAcquisitionStart);
}


void cDspIFace::DspMemoryRead(cDspMeasData<float> *pMData) // liest alle daten dieser memorygruppe
{
    m_pMeasData = pMData;
    m_ActTimer->start(0,DspMemoryReadStart);
}

void cDspIFace::DspMemoryRead(cDspMeasData<quint32> *pMData)
{
    m_pMeasDataUlong = pMData;
    m_ActTimer->start(0,DspMemoryReadLongStart);
}


void cDspIFace::ReadDeviceVersion()
{
    m_ActTimer->start(0,ReadDeviceVersionStart);
}


void cDspIFace::ReadServerVersion()
{
    m_ActTimer->start(0,ReadServerVersionStart);
}


void cDspIFace::DspMemoryWrite(cDspMeasData<float> *pMData)  // schreibt alle daten dieser memorygruppe die daten sind schon im data feld
{	
    m_pMeasData = pMData;
    m_pMeasDataUlong = nullptr;
    m_ActTimer->start(0,DspMemoryWriteStart);
}


void cDspIFace::DspMemoryWrite(cDspMeasData<quint32> *pMData)  // schreibt alle daten dieser memorygruppe die daten sind schon im data feld
{
    m_pMeasDataUlong = pMData;
    m_pMeasData = nullptr;
    m_ActTimer->start(0,DspMemoryWriteStart);
}


float* cDspIFace::data(cDspMeasData<float> *pMData) // gibt einen zeiger zurück auf die var daten vom typ vapplication
{
    return pMData->data();
}


quint32* cDspIFace::data(cDspMeasData<quint32> *pMData) // gibt einen zeiger zurück auf die var daten vom typ vapplication
{
    return pMData->data();
}


void cDspIFace::SetGainCorrection(int chn, float val) // setzt für kanal (int 0..) die verstärkungskorrektur
{
    QString s=QString("mem:writ GAINCORRECTION+%1,%2;\n").arg(chn).arg(val);
    iFaceSock->SendCommand(s);
}


void cDspIFace::SetPhaseCorrection(int chn, float val) // setzt für kanal (int 0..) die phasenkorrektur    
{
    QString s=QString("mem:writ PHASECORRECTION+%1,%2;\n").arg(chn).arg(val);
    iFaceSock->SendCommand(s);
}


void cDspIFace::SetOffsetCorrection(int chn, float val) // setzt für kanal (int 0..) die offsetkorrektur
{
    QString s=QString("mem:writ OFFSETCORRECTION+%1,%2;\n").arg(chn).arg(val);
    iFaceSock->SendCommand(s);
}


void cDspIFace::GetInterfaceData()
{
    QString list;
    QStringList DataEntryList, DataList; // werte zuorden 
    QString s;
    float *fval;


    list = iFaceSock->GetAnswer();
    DataEntryList = QStringList::split(";",list); // wir haben jetzt eine stringliste mit allen werten
    fval = m_pMeasData->data();
    for ( QStringList::Iterator it = DataEntryList.begin(); it != DataEntryList.end(); ++it ) {
        s = *it;
        s = s.section(":",1,1);
        DataList = QStringList::split(",",s);
        for ( QStringList::Iterator it2 = DataList.begin(); it2 != DataList.end(); ++it2) {
            s = *it2;
            s.remove(';');
            *fval = s.toFloat();
            fval++;
        }
    }
}

void cDspIFace::GetInterfaceLongData()
{
    QString list;
    QStringList DataEntryList, DataList; // werte zuorden
    QString s;
    quint32 *lval;


    list = iFaceSock->GetAnswer();
    DataEntryList = QStringList::split(";",list); // wir haben jetzt eine stringliste mit allen werten
    lval = m_pMeasDataUlong->data();
    for ( QStringList::Iterator it = DataEntryList.begin(); it != DataEntryList.end(); ++it ) {
        s = *it;
        s = s.section(":",1,1);
        DataList = QStringList::split(",",s);
        for ( QStringList::Iterator it2 = DataList.begin(); it2 != DataList.end(); ++it2) {
            s = *it2;
            s.remove(';');
            *lval = s.toULong();
            lval++;
        }
    }
}


void cDspIFace::TestDspRunning()
{
    QString s = "status:dsp ?\n";
    iFaceSock->SendQuery(s);
}    


void cDspIFace::SetDspBootPath()
{
    QString cmds = QString("system:dsp:boot:path %1\n").arg(dspBootPath);
    iFaceSock->SendCommand(cmds);
}


void cDspIFace::DspReset()
{
    QString cmds = "system:dsp:reset\n";
    iFaceSock->SendCommand(cmds);
}

void cDspIFace::SendBootCommand()
{
    QString cmds = "system:dsp:boot:execute\n";
    iFaceSock->SendCommand(cmds);
}


void cDspIFace::SendSamplingSystemCommand()
{
    QString cmds = QString("syst:dsp:samp %1,%2,%3\n").arg(m_nP1).arg(m_nP2).arg(m_nP3);
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmds, sl);
}


void cDspIFace::SendVarListCommand() // die komplette varliste bestehend aus n teillisten
{
    QString plist;
    QTextStream ts( &plist, QIODevice::WriteOnly );

    foreach ( cDspMeasData<float>* pDspMeasData, DspMeasDataList )
        ts << pDspMeasData->VarList();
    QString cmd = QString ("measure:list:ravlist %1\n").arg(plist);
    iFaceSock->SendCommand(cmd);
}


void cDspIFace::SendCmdListCommand() // sendet cycliste an den dsp server
{
    QString plist;
    QTextStream ts( &plist, QIODevice::WriteOnly );
    for ( QStringList::Iterator it = CycCmdList.begin(); it != CycCmdList.end(); ++it )
        ts << *it << ";" ;
    QString cmd = QString ("measure:list:cyclist %1\n").arg(plist);
    iFaceSock->SendCommand(cmd);
}


void cDspIFace::SendCmdIntListCommand() // sendet intliste an den dsp server 
{
    QString plist;
    QTextStream ts( &plist, QIODevice::WriteOnly );
    for ( QStringList::Iterator it = IntCmdList.begin(); it != IntCmdList.end(); ++it )
        ts << *it << ";" ;
    QString cmd = QString ("measure:list:intlist %1\n").arg(plist);
    iFaceSock->SendCommand(cmd);
}


void cDspIFace::SendSignalRoutingCommand()
{
    QString cmds;
    QTextStream ts(&cmds,QIODevice::WriteOnly);
    ts << "mem:write ETHROUTINGTAB";
    for (int i = 0; i < 16; i++)
        ts << "," << m_lP1[i];
    ts << ";\n";
    iFaceSock->SendCommand(cmds);
}


void cDspIFace::SendResetMaximumCommand()
{
    QString cmds = QString("syst:dsp:max:res\n");
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmds,sl); // maxima im dsp gelöscht
}	 


void cDspIFace::SendSetDsp61850SourceAdrCommand()
{
    QString cmds;
    QTextStream ts(&cmds,QIODevice::WriteOnly);
    
    ts << "syst:dsp:en61:mac:sadr ";
    int i;
    for (i = 0;i < 5 ;i++)
        ts <<  m_ethadr.MacAdrByte[i] << ",";
    ts <<  m_ethadr.MacAdrByte[i] << "\n";
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmds,sl);
}


void cDspIFace::SendSetDsp61850DestAdrCommand()
{
    QString cmds;
    QTextStream ts(&cmds,QIODevice::WriteOnly);
    
    ts << "syst:dsp:en61:mac:dadr ";
    int i;
    for (i = 0;i < 5 ;i++)
        ts <<  m_ethadr.MacAdrByte[i] << ",";
    ts <<  m_ethadr.MacAdrByte[i] << "\n";
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmds,sl);
}


void cDspIFace::SendSetDsp61850PriorityTaggedCommand()
{
    QString cmd = QString ("syst:dsp:en61:pri %1\n").arg(m_nuP1);
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmd,sl);
}


void cDspIFace::SendSetDsp61850EthTypeAppIdCommand()
{
    QString cmd = QString ("syst:dsp:en61:etht %1\n").arg(m_nuP1);
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmd,sl);
}


void cDspIFace::SendSetDsp61850EthSyncCommand()
{
    QString cmd = QString ("syst:dsp:en61:eths %1\n").arg(m_nuP1);
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmd,sl);
}


void cDspIFace::SendTriggerIntHKSKCommand()
{
    QString cmds = QString("syst:dsp:trig:intl:hksk %1\n").arg(m_nP1);
    QStringList sl;
    sl.append("ACK");
    sl.append("BUSY");
    iFaceSock->SendCommand(cmds,sl);
}


void cDspIFace::SendDataAcquisitionCommand()
{
    QString Cmd = QString("meas %1\n").arg(m_pMeasData->MeasVarList()); // kommando zusammenbauen
    iFaceSock->SendQuery(Cmd); // abfrage durchführen
}

void cDspIFace::SendDspMemoryReadLongCommand()
{
    QString Cmd = QString("mem:read %1\n").arg(m_pMeasDataUlong->VarList()); // kommando zusammenbauen
    iFaceSock->SendQuery(Cmd); // abfrage durchführen
}

void cDspIFace::SendDspMemoryReadCommand()
{
    QString Cmd = QString("mem:read %1\n").arg(m_pMeasData->VarList()); // kommando zusammenbauen
    iFaceSock->SendQuery(Cmd); // abfrage durchführen
}


void cDspIFace::SendDspMemoryWriteCommand()
{
    QString list;
    QStringList DataEntryList; 
    QString s, Cmd;
    QTextStream ts( &Cmd, QIODevice::WriteOnly );
    
    ts << "mem:writ ";
    
    float* fval;
    quint32* lval;
    bool bnotFloat;

    if (m_pMeasData == nullptr){
        list = m_pMeasDataUlong->VarList(); // liste mit allen variablen und deren länge
        lval = m_pMeasDataUlong->data();
        bnotFloat = true;
    }
    else
    {
        list = m_pMeasData->VarList(); // liste mit allen variablen und deren länge
        fval = m_pMeasData->data();
        bnotFloat = false;
    }

    DataEntryList = QStringList::split(";",list); // wir haben jetzt eine stringliste mit je variable, länge
    for ( QStringList::Iterator it = DataEntryList.begin(); it != DataEntryList.end(); ++it ) {
        s = *it; // einen eintrag variable, länge
        ts << s.section(",",0,0); // den namen,
        int n = s.section(",",1,1).toInt(); // anzahl der werte für diese var.
        for (int i = 0;i < n; i++) {
            ts << ",";
            if (bnotFloat)  {// wir haben integer daten
                ts << *lval;
                lval++;
            }
            else {
                ts << *fval;
                fval++;
            }
        }
        ts << ";";
    }
    ts << "\n";
    iFaceSock->SendCommand(Cmd); //  kommando senden
}


void cDspIFace::SendActivateInterfaceCommand()
{
    QString cmd = "measure:list:set\n";
    iFaceSock->SendCommand(cmd);
}

void cDspIFace::SendDeactivateInterfaceCommand()
{
    QString cmd = "measure:list:clear\n";
    iFaceSock->SendCommand(cmd);
}

bool cDspIFace::IFaceError()
{
    return m_biFaceError;
}


void cDspIFace::SendReadDeviceVersionCommand()
{
    QString cmds = QString("syst:vers:dev?\n");
    iFaceSock->SendQuery(cmds);
}


void cDspIFace::SendReadServerVersionCommand()
{
    QString cmds = QString("syst:vers:serv?\n");
    iFaceSock->SendQuery(cmds);
}

