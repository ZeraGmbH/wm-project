#ifndef SESSIONFILENAMEGEN_H
#define SESSIONFILENAMEGEN_H

#include <QString>
#include <QDir>

class SessionFileNameGen
{
public:
    SessionFileNameGen(QString machineName, QString homePath = QDir::homePath());
    QString getSessionPath();
    QString getSessionFileName(QString baseName, QString sessionName = "");
private:
    QString m_sessionPath;
};

#endif // SESSIONFILENAMEGEN_H
