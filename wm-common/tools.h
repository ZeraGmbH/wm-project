// headerdatei tools.h

#ifndef TOOLS_H
#define TOOLS_H

#include <qobject.h>
#include <qtimer.h>
#include <q3valuelist.h>

#include "complex.h"

double normWinkelrad02PI(double); // normiert den winkel auf 0..2PI
double normWinkelrad_PIPI(double w); // dito von -PI .. +PI
double UserAtan(double im, double re);
double UserAtan(complex);
double sign(double);
char* sAlloc(QString&); // allokiert speicher für inhalt von qstring

enum WdtStates
{
    wmwdt_idle = 0,
    wmwdt_start,
    wmwdt_meas,
    wmwdt_compu
};

class WmWatchDogTimer : public QObject
{
    Q_OBJECT
public:
    WmWatchDogTimer();
    void setState(int state);
signals:
    void timeout();
private:
    int m_wdtState;
    int m_count;
};

struct SMTimerData
{
    int t;
    int Action;
};


class cSMTimer: public QTimer // statemachine timer
{
    Q_OBJECT
    
public:
    cSMTimer();
    ~cSMTimer();
    void start(int, int); // zeit, action
    
signals:
    void timeout(int); // wir senden bei timeout die action
     
private:
    Q3ValueList<SMTimerData> m_TimerDataList;
    Q3ValueList<SMTimerData> m_TimerEventList;
//    QTimer* m_lTimer;
    
private slots:
    void TimeExpiredSlot();
};

#endif
