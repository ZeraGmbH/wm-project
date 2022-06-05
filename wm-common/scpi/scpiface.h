#ifndef CSCPIFACE_H
#define CSCPIFACE_H

#include "cbiface.h"
#include "clientiodevice.h"
#include "scpiopcstates.h"
#include "cmdtimer.h"
#include "cmdinterpret.h"
#include "tools.h"
#include <QObject>

class cSCPIStatSyst; // forward to avoid circular includes

class cSCPIFace : public QObject, public cbIFace  // virtuelle basisklasse eines scpi interfaces
{
    Q_OBJECT

public:
    cSCPIFace(cClientIODevice*, short); //  zeiger auf das zugehörige io device, länge für die eventqueue
    ~cSCPIFace();
    virtual void SCPICmd( int,  char*);
    virtual char* SCPIQuery( int, char*);
    cClientIODevice* GetClientIODevice();
    void AddEventError(int); // fehler bzw. event eintrag in queue
    void ClearEventError(); // alle einträge in queue löschen und mav bit = 0
    cSCPIStatSyst* m_pOperationStat; // public -> ein gerät das ein scpi interface hat kann die stati setzen
    cSCPIStatSyst* m_pQuestionableStat; // diese 2 stati sind immer vorhanden

    virtual void ResetDevice(); // wird von common cmd aufgerufen, virt. falls abgel. klasse auch was
    virtual void DeviceClearStatus(); // rückzusetzen oder zu löschen hat
    virtual void OPCCommand(); // virt. falls ein device nicht nur sequentielle kommandos bearbeiten soll
    virtual char* OPCQuery(); // das gilt auch für die abfrage
    void SetSTB(uchar); // status register setzen  ; hier alles einzeln weil setzen bzw. rücksetzen
    void ResetSTB(uchar); // bzw. rücksetzen ;       kann weitere massnahmen zur folge haben
    void SetSRE(uchar); // service request register setzen
    void ResetSRE(uchar); // bzw. rücksetzen
    void SetESR(uchar); // standard event status register setzen
    void ResetESR(uchar); // bzw. rücksetzen
    void SetESE(uchar); // standard event enable register setzen
    void ResetESE(uchar); // bzw. rücksetzen

    // die routinen für die common commands
    void DeviceReset(char*);
    void SetDeviceOPC(char*);
    void DeviceClearStatus(char*);
    void SetDeviceESE(char*);
    void SetDeviceSRE(char*);
    virtual char* GetDeviceIdentification();
    virtual char* DeviceSelfTest();
    char* GetDeviceOPC();
    char* GetDeviceESE();
    char* GetDeviceESR();
    char* GetDeviceSRE();
    char* GetDeviceSTB();
    void setOPCQState(opcStates state);


public slots:
    void ReceiveCommand(QString&); // das kommando
    void ReceivePriority(int); // 0 ist die höchste priorität und darf alles
    void AffectSCPIStatus(uchar, ushort);

protected slots:
    virtual void ExecuteCommand(int)=0; // ausführen eines common kommandos statemachine
    virtual void CmdExecution(QString&) = 0;

protected:
    char* answ;
    cCmdTimer* m_pCmdTimer;
    cSMTimer* m_pSMachineTimer;
    bool isAuthorized();
    cNode* m_pCommands;
    cParse* m_pParser;
    cCmdInterpreter* m_pCmdInterpreter;
    QString m_sCommand;
    QString m_sAnswer; // wir speichern antworten zwischen
    void SetIEEE488Register(char*, uchar&);
    void SetnoOperFlag(bool);
    bool m_bCmdError; // kommando fehler speicher
    Q3ValueList<int> m_ErrEventQueue;


signals:
    void SendAnswer(QString&);

private:
    int m_nPriority;
    cClientIODevice*  m_pCIOD; // pointer io device
    char* RegConversion(uchar);
    ushort m_nQueueLen; // max. anzahl einträge in obiger queue

    uchar m_nSTB; // status byte !!!!!! für diese 4 register immer (ausnahme konstruktor) funktion verwenden !!!!!!!
    uchar m_nSRE; // service reguest enable
    uchar m_nESR; // standard event status register (immer vorhanden)
    uchar m_nESE; // standard event status enable (immer vorhanden)
    opcStates m_nOPCState;
    opcStates m_nOPCQState;
    bool m_bnoOperationPendingFlag; // immer funktion zum setzen verwenden !!!!!!!

    cNode* InitCommonCmdTree(cNode*);
};

#endif // CSCPIFACE_H
