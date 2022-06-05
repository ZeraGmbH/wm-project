#ifndef CSCPISTATSYST_H
#define CSCPISTATSYST_H

#include <qglobal.h>

class cSCPIFace; // forward to avoid circular includes

class cSCPIStatSyst {
public:
    cSCPIStatSyst(cSCPIFace* , uchar); // callback , bit fürs statusbyte
    void SetCondition(ushort); // überschreibt alle bits ...also ev. vorher lesen und
    ushort GetCondition();
    void SetNTransition(ushort);
    ushort GetNTransition();
    void SetPTransition(ushort);
    ushort GetPTransition();
    void SetEnable(ushort);
    ushort GetEnable();
    ushort GetEvent();
    void SetConditionBit(ushort);
    void ResetConditionBit(ushort);

private:
    cSCPIFace* m_myIFace;
    uchar m_nStatBit; // bit position im status byte
    ushort m_nCondition; // wenn ein Conditionbit gesetzt bzw. rückgesetzt
    ushort m_nPTransition; // und pos. flanke enabled
    ushort m_nNTransition; // bzw. neg. flanke enabled
    ushort m_nEvent; // dann wird dies zu einem event
    ushort m_nEnable; // wenn enabled führt event zum setzen von

};

#endif // CSCPISTATSYST_H
