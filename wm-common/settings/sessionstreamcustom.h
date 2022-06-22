#ifndef SESSIONSTREAMCUSTOM_H
#define SESSIONSTREAMCUSTOM_H

#include "sessionstreamstrategy.h"

class ISessionStreamCustomHandler
{
private:
    virtual void transferSessionCustom(QDataStream& stream, bool write) = 0;
    friend class SessionStreamCustom;
};

class SessionStreamCustom : public SessionStreamStrategy
{
public:
    SessionStreamCustom(ISessionStreamCustomHandler *sessionCustomHandler) : m_sessionCustomHandler(sessionCustomHandler) {}
    virtual void readSession(QDataStream& stream)  override { m_sessionCustomHandler->transferSessionCustom(stream, false); }
    virtual void writeSession(QDataStream& stream) override { m_sessionCustomHandler->transferSessionCustom(stream, true); }
protected:
    ISessionStreamCustomHandler *m_sessionCustomHandler;
};

#endif // SESSIONSTREAMCUSTOM_H
