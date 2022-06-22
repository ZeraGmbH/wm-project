#ifndef SESSIONSTREAMER_H
#define SESSIONSTREAMER_H

#include "sessionstreamzero.h"
#include "sessionfilenamegen.h"
#include "sessiondefaulterstrategy.h"
#include <QString>

class SessionStreamer
{
public:
    SessionStreamer(QString machineName,
                    ISessionDefaulterStrategyPtr sessionDefaulter,
                    QString homePath = QDir::homePath(),
                    IStreamStrategyPtr streamStrategy = SessionStreamZeroPtr::create().staticCast<IStreamStrategy>());
    void readSession(QString sessionName = "");
    bool writeSession(QString sessionName = "");
private:
    IStreamStrategyPtr m_streamStrategy;
    ISessionDefaulterStrategyPtr m_sessionDefaulter;
    SessionFileNameGen m_sessionFileNameGen;
};

#endif // SESSIONSTREAMER_H
