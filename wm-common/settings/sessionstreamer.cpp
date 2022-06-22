#include "sessionstreamer.h"

SessionStreamer::SessionStreamer(QString machineName,
                                 ISessionStreamImplementor *sessionStreamImplementor,
                                 QString homePath) :
    m_sessionStreamImplementor(sessionStreamImplementor),
    m_sessionFileNameGen(machineName, homePath)
{
}

void SessionStreamer::readSession(QString baseName, QString sessionName)
{
    QFile file(m_sessionFileNameGen.getSessionFileName(baseName, sessionName));
    if(file.open(QFile::ReadOnly)) {
    }
    else
        m_sessionStreamImplementor->setDefaults();
}

bool SessionStreamer::writeSession(QString baseName, QString sessionName)
{
    createDir(m_sessionFileNameGen.getSessionPath());
    QFile file(m_sessionFileNameGen.getSessionFileName(baseName, sessionName));
    bool validFile = file.open(QFile::WriteOnly);
    if(validFile) {
    }
    else
        m_sessionStreamImplementor->setDefaults();
    return validFile;
}

void SessionStreamer::createDir(QString dir)
{
    if(!QDir(dir).exists()) {
        QDir().mkpath(dir);
    }
}