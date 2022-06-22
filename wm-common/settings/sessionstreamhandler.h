#ifndef SESSIONSTREAMHANDLER_H
#define SESSIONSTREAMHANDLER_H

#include "streamstrategy.h"

class ISessionStreamHandlerHandler
{
private:
    virtual void readSession(QDataStream& stream) = 0;
    virtual void writeSession(QDataStream& stream) = 0;
    friend class SessionStreamHandler;
};

class SessionStreamHandler : public IStreamStrategy
{
public:
    SessionStreamHandler(ISessionStreamHandlerHandler *sessionCustomHandler) : m_sessionCustomHandler(sessionCustomHandler) {}
    virtual void read(QDataStream& stream)  override { m_sessionCustomHandler->readSession(stream); }
    virtual void write(QDataStream& stream) override { m_sessionCustomHandler->writeSession(stream); }
protected:
    ISessionStreamHandlerHandler *m_sessionCustomHandler;
};

#endif // SESSIONSTREAMHANDLER_H
