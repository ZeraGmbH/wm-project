#ifndef SESSIONSTREAMER_H
#define SESSIONSTREAMER_H

#include "sessionfilenamegen.h"
#include <QString>
#include <QFile>

class ISessionStreamImplementor
{
private:
    virtual void readStream(QDataStream& stream) = 0;
    virtual void writeStream(QDataStream& stream) = 0;
    virtual void setDefaults() = 0;
    friend class SessionStreamer;
};

class SessionStreamer
{
public:
    SessionStreamer(QString machineName,
                    QString defaultBaseName,
                    ISessionStreamImplementor *sessionStreamImplementor,
                    QString homePath = QDir::homePath());
    void readSession(QString sessionName = "");
    bool writeSession(QString sessionName = "");
private:
    void createDir(QString dir);

    ISessionStreamImplementor* m_sessionStreamImplementor;
    SessionFileNameGen m_sessionFileNameGen;
    QString m_strDefaultBaseName;
};

#endif // SESSIONSTREAMER_H
