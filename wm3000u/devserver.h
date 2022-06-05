// headerdatei devserver.h 
// deklaration einer device server klasse

#ifndef DEVSERVER_H
#define DEVSERVER_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3serversocket.h>
#include <q3socket.h>
#include <q3socketdevice.h> 
#include <qsocketnotifier.h>
#include <q3ptrlist.h>

#include "wmglobal.h"
#include "wmactvalues.h"
#include "versserial.h"
#include "cbiface.h"
#include "confdata.h"
#include "cmdinterpret.h"
#include "scpiface.h"
#include "en61850.h"
#include "clientiodevice.h"
#include "deviceserver.h"

class cwm3000DeviceServer:  public cDeviceServer  // der wm3000 device server
{
    Q_OBJECT
    
public:
    cwm3000DeviceServer( quint16 port, int backlog = 1, QObject * parent = 0, const char * name = 0 );
    virtual void newConnection(int); // instanziiert ein client socket device und ein wm3000 scpi interfaceund trägt das interface in die liste ein 
    virtual void newConnection(cClientIODevice*); // nimmt das bereits instanziierte xxx interface, generiert noch ein wm3000 scpi interface und trägt das interface in die liste ein
    
public slots:
    void ReceiveDeviceConfiguration(cConfData*); // empfangen der conf. vom device
    void ReceiveIFaceConfiguration(cConfData*); // empfangen der conf. vom interface
    void ReceiveETHStatus( cEN61850Info*); // empfangen der en61850 status infos
    void ReceiveENInfoReguest(); // wenn eins der interfaces en61850 info angefragt hat
    void ReceiveSelftestResult(int);
    void ReceiveSelftestRequest(); // wenn eins der interfaces *tst ausführen soll
    void ReceiveOffsetResult(double offs);
    void ReceiveChannelNOffsetMeasureRequest();
    void ReceiveChannelXOffsetMeasureRequest();
    void ReceiveResetETHStatusCommand();
    void ReceiveIFaceRangeCommand(cConfData*);
    void SetRangeListSlot( cWMRangeList&,  cWMRangeList&);
    void ReceiveIFaceDefaultConfig(); // das device soll default konfig. setzen
    void ReceiveRangeAutomaticDone(); // rückmeldung wenn bereich automatik fertig ist
    void ReceiveActualValues(cwmActValues*); // empfängt messwerte vom device
    void ReceiveLPValue(cwmActValues*); // empfängt lastpunkt  vom device
    void ReceiveVersionInfo(tVersSerial*); // empfängt info von device
    void ReceiveAffectStatus(uchar act, ushort stat);
    
signals:
    void SendDeviceConfiguration(cConfData*); // senden der conf.  an das device
    void SendIFaceConfiguration(cConfData*); // senden der conf.  an das interface
    void SendIFaceETHStatus(cEN61850Info*); // wir senden den eth status an das interface
    void SendSelftestResult(int); // wir senden das ergebnis des selbsttest an das interface
    void SendOffsetResult(double); // dito
    void RequestEN61850Info(); //  der server fordert die info an und verteilt sie
    void RequestSelftest(); // der server lässt den selbsttest durchführen
    void RequestChannelNOffsetMeasure(); // der server lässt die n kanal offsetmessung durchführen
    void RequestChannelXOffsetMeasure(); // der server lässt die x kanal offsetmessung durchführen
    void ResetETHStatus(); // der server leitet das reset kommando weiter
    void SendRangeCommand(cConfData*); // ebenso den bereich setzen befehl
    void SendDefaultConfigCommand(); // der server leitet kommando zum setzen default konfig. weiter
    void ConfirmRangeAutoRdy();
    void SendActualValues(cwmActValues*); // sendet messwerte an die clients
    void SendLPValue(cwmActValues*); // sendet lastpunkt an die clients
    void SendVersionInfo(tVersSerial*); // sendet info an die clients
    void AffectStatus(uchar, ushort);
    
private:
    cConfData m_ConfData; // wir speichern die konfiguration zwischen
    tVersSerial* m_pV;
    cWMRangeList m_sNXRangeList;
    cWMRangeList m_sEVTRangeList;
};

#endif
