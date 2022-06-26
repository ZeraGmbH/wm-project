#include "sessionstreamer.h"

SessionStreamer::SessionStreamer(QString machineName, QString defaultBaseName,
                                 ISessionStreamImplementor *sessionStreamImplementor,
                                 QString homePath) :
    m_sessionStreamImplementor(sessionStreamImplementor),
    m_sessionFileNameGen(machineName, homePath),
    m_strDefaultBaseName(defaultBaseName)
{
}

void SessionStreamer::readSession(QString sessionName)
{
    QFile file(m_sessionFileNameGen.getSessionFileName(m_strDefaultBaseName, sessionName));
    bool error = false;
    if(file.open(QFile::ReadOnly)) {
        QDataStream stream(&file);
        m_sessionStreamImplementor->readStream(stream);
        if(!stream.eof() || stream.status() != QDataStream::Ok)
            error = true;
    }
    else
        error = true;
    if(error)
        m_sessionStreamImplementor->setDefaults();
}

bool SessionStreamer::writeSession(QString sessionName)
{
    createDir(m_sessionFileNameGen.getSessionPath());
    QFile file(m_sessionFileNameGen.getSessionFileName(m_strDefaultBaseName, sessionName));
    bool validFile = file.open(QFile::WriteOnly);
    if(validFile) {
        QDataStream stream(&file);
        m_sessionStreamImplementor->writeStream(stream);
    }
    return validFile;
}

void SessionStreamer::createDir(QString dir)
{
    if(!QDir(dir).exists()) {
        QDir().mkpath(dir);
    }
}
