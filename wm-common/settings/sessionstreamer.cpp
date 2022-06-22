#include "sessionstreamer.h"

SessionStreamer::SessionStreamer(QString machineName,
                                 ISessionDefaulterStrategyPtr sessionDefaulter, QString homePath,
                                 IStreamStrategyPtr streamStrategy) :
    m_streamStrategy(streamStrategy),
    m_sessionDefaulter(sessionDefaulter),
    m_sessionFileNameGen(machineName, homePath)
{
}

void SessionStreamer::readSession(QString sessionName)
{
}

bool SessionStreamer::writeSession(QString sessionName)
{
    return true;
}
