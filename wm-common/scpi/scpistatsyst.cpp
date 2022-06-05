#include "scpistatsyst.h"
#include "scpiface.h"

cSCPIStatSyst::cSCPIStatSyst(cSCPIFace* ip, uchar bit)  // callback und bit fürs statusbyte
{
    m_myIFace = ip;
    m_nStatBit = bit;
    m_nCondition = 0;
    m_nPTransition = 0;
    m_nNTransition = 0;
    m_nEnable = 0;
    m_nEvent = 0;
}


void cSCPIStatSyst::SetCondition(ushort w)  // überschreibt alle bits ...also ev. vorher lesen und
{
    ushort oldcond, Edge;
    oldcond = m_nCondition;
    m_nCondition = w;
    Edge = (oldcond ^ m_nCondition) & m_nCondition; // pos. flanken
    m_nEvent |= Edge & m_nPTransition;
    if (Edge & m_nEnable)
    m_myIFace->SetSTB(m_nStatBit);
    Edge = (oldcond ^ m_nCondition) & ~m_nCondition; // neg. flanken
    m_nEvent |= Edge & m_nPTransition;
    if (Edge & m_nEnable)
    m_myIFace->SetSTB(m_nStatBit);
}


ushort cSCPIStatSyst::GetCondition() {
    return m_nCondition;
}


void cSCPIStatSyst::SetNTransition(ushort w) {
    m_nNTransition = w;
}


ushort cSCPIStatSyst::GetNTransition() {
    return m_nNTransition;
}


void cSCPIStatSyst::SetPTransition(ushort w) {
    m_nPTransition = w;
}


ushort cSCPIStatSyst::GetPTransition() {
    return m_nPTransition;
}


void cSCPIStatSyst::SetEnable(ushort w) {
    m_nEnable = w;
    SetCondition( GetCondition() ); // 3x darfs´ te raten
}


ushort cSCPIStatSyst::GetEnable() {
    return m_nEnable;
}


ushort cSCPIStatSyst::GetEvent() {
    ushort oldEvent = m_nEvent;
    m_nEvent = 0; // events werden nach dem lesen gelöscht
    return oldEvent;
}


void cSCPIStatSyst::SetConditionBit(ushort bp) {
    SetCondition( GetCondition() | bp );
}


void cSCPIStatSyst::ResetConditionBit(ushort bp) {
    SetCondition( GetCondition() & ~bp );
}
