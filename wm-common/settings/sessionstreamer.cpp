#include "sessionstreamer.h"

SessionStreamer::SessionStreamer(QString machineName,
                                 ISessionStreamImplementor *sessionStreamImplementor,
                                 QString homePath) :
    m_sessionStreamImplementor(sessionStreamImplementor),
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
