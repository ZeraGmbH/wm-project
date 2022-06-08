#ifndef SESSIONAPPENDCUSTOM_H
#define SESSIONAPPENDCUSTOM_H

#include "sessionappendstrategy.h"

class ISessionAppendCustomHandler
{
private:
    virtual void transferSessionCustom(QDataStream& stream, bool write) = 0;
    friend class SessionAppendCustom;
};

class SessionAppendCustom : public SessionAppendStrategy
{
public:
    SessionAppendCustom(ISessionAppendCustomHandler *sessionCustomHandler) : m_sessionCustomHandler(sessionCustomHandler) {}
    virtual void readSessionAppend(QDataStream& stream)  override { m_sessionCustomHandler->transferSessionCustom(stream, false); }
    virtual void writeSessionAppend(QDataStream& stream) override { m_sessionCustomHandler->transferSessionCustom(stream, true); }
protected:
    ISessionAppendCustomHandler *m_sessionCustomHandler;
};

#endif // SESSIONAPPENDCUSTOM_H
