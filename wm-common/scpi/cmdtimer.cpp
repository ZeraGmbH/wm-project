#include "cmdtimer.h"

cCmdTimer::cCmdTimer(QObject * parent, const char * name)
    :QTimer(parent, name)
{
    connect(this, SIGNAL(timeout()), this, SLOT(TimeExpired()));
}


int cCmdTimer::start(int t, QString& s)
{
    m_sCmd = s;
    return QTimer::start(t, true); // immer single shot
}


void cCmdTimer::TimeExpired()
{
    emit Command2Execute(m_sCmd);
}
