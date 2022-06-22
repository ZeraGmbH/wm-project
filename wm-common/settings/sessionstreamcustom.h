#ifndef SESSIONSTREAMCUSTOM_H
#define SESSIONSTREAMCUSTOM_H

#include "streamstrategy.h"

class ISessionStreamCustomHandler
{
private:
    virtual void readSession(QDataStream& stream) = 0;
    virtual void writeSession(QDataStream& stream) = 0;
    friend class SessionStreamCustom;
};

class SessionStreamCustom : public IStreamStrategy
{
public:
    SessionStreamCustom(ISessionStreamCustomHandler *sessionCustomHandler) : m_sessionCustomHandler(sessionCustomHandler) {}
    virtual void read(QDataStream& stream)  override { m_sessionCustomHandler->readSession(stream); }
    virtual void write(QDataStream& stream) override { m_sessionCustomHandler->writeSession(stream); }
protected:
    ISessionStreamCustomHandler *m_sessionCustomHandler;
};

#endif // SESSIONSTREAMCUSTOM_H
