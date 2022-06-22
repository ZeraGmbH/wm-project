#ifndef SESSIONSTREAMHANDLER_H
#define SESSIONSTREAMHANDLER_H

#include "streamstrategy.h"

class ISessionStream
{
private:
    virtual void readSession(QDataStream& stream) = 0;
    virtual void writeSession(QDataStream& stream) = 0;
    friend class SessionStreamHandler;
};

class SessionStreamHandler : public IStreamStrategy
{
public:
    SessionStreamHandler(ISessionStream *sessionStream) : m_sessionStream(sessionStream) {}
    virtual void read(QDataStream& stream)  override { m_sessionStream->readSession(stream); }
    virtual void write(QDataStream& stream) override { m_sessionStream->writeSession(stream); }
protected:
    ISessionStream *m_sessionStream;
};

#endif // SESSIONSTREAMHANDLER_H
