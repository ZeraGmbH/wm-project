#ifndef WMGLOBALCOMMON_H
#define WMGLOBALCOMMON_H

#include "range.h"

#include <memory>
#include <vector>

#include <QString>

enum Languages {de,gb,pl};
const float TDBase = 100.0e6; // 100 mhz auflösung für td messung (pps->1. sample)

enum SyncSources {Intern,Extern,MaxSSource}; // sync sources
enum SignalFreqs {F16,F50,F60,MaxFreq}; // -> feste abtastfrequenzen
enum SampleRates {S80,S96,S240,S256,S288,MaxSRate}; // abtastraten
enum SenseMode {sensNsensX, adcNadcX, sensNadcX, sensXadcN, sensNsensX0V, anzSenseMode}; // sense modes innerhalb der hardware
enum SignalModes {AC, DC, maxSMode}; // signal modi
enum UserDecisions {AbortProgram,Stop,Retry,SimulationMode}; // benutzer entscheidungen
enum JustMode {sensNadcXPhase, sensXadcNPhase, sensExTadcNPhase, sensNsensXOffset, sensNOffset, sensXOffset, sensExTOffset, adcNPhase, adcXPhase}; // justage modes

class cJustMeasInfoBase
{
public:
//    cJustMeasInfoBase(const QString rngN, const QString rngX, const QString rngStore, SenseMode sm, MeasMode mm, JustMode jm, int nS, int nIgn, int nMeas )
//        :m_srngN(rngN), m_srngX(rngX), m_srngStore(rngStore), m_nSMode(sm), m_nMMode(mm), m_nJMode(jm), m_nnS(nS), m_nIgnore(nIgn), m_nnMeas(nMeas){}
    cJustMeasInfoBase(const QString rngN, const QString rngX, const QString rngStore, SenseMode sm)
        :m_srngN(rngN), m_srngX(rngX), m_srngStore(rngStore), m_nSMode(sm){}

    QString m_srngN; // bereich kanal n der angewählt wird
    QString m_srngX; // bereich kanal x ......
    QString m_srngStore; // der bereich auf dem die justage daten gespeichert werden
    SenseMode m_nSMode; // sense mode (was zu testen bzw. justieren ist)
};

class cJustMeasInfoBase2nd
{
public:
    cJustMeasInfoBase2nd(JustMode jm, int nS, int nIgn, int nMeas)
        :m_nJMode(jm), m_nnS(nS), m_nIgnore(nIgn), m_nnMeas(nMeas){}
    JustMode m_nJMode; // welcher justage modues
    int m_nnS; // samples pro periode
    int m_nIgnore; // anzahl messungen zum einschwingen
    int m_nnMeas; // anzahl messungen zur messwertbestimmung
};

#endif // WMGLOBALCOMMON_H
