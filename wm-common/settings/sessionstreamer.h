#ifndef SESSIONSTREAMER_H
#define SESSIONSTREAMER_H

#include "sessionfilenamegen.h"
#include <QString>

class ISessionStreamImplementor
{
public:
    virtual void readStream(QDataStream& stream) = 0;
    virtual void writeStream(QDataStream& stream) = 0;
    virtual void setDefaults() = 0;
};


class SessionStreamer
{
public:
    SessionStreamer(QString machineName,
                    ISessionStreamImplementor *sessionStreamImplementor,
                    QString homePath = QDir::homePath());
    void readSession(QString sessionName = "");
    bool writeSession(QString sessionName = "");
private:
    ISessionStreamImplementor* m_sessionStreamImplementor;
    SessionFileNameGen m_sessionFileNameGen;
};

#endif // SESSIONSTREAMER_H
