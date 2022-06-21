#include "sessionfilenamegen.h"

SessionFileNameGen::SessionFileNameGen(QString machineName, QString homePath) :
    m_sessionPath(QString("%1/.%2").arg(homePath, machineName).replace("//", "/"))
{
}

QString SessionFileNameGen::getSessionPath()
{
    return m_sessionPath;
}

QString SessionFileNameGen::getSessionFileName(QString baseName, QString sessionName)
{
    QString filename = sessionName.isEmpty() ? baseName + ".ses" : baseName + sessionName;
    return m_sessionPath + "/" + filename;
}
