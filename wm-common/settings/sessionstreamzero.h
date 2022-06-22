#ifndef SESSIONHELPERSTREAMZERO_H
#define SESSIONHELPERSTREAMZERO_H

#include "sessionstreamstrategy.h"

class SessionStreamZero : public SessionStreamStrategy
{
public:
    virtual void readSession(QDataStream& stream)  override { Q_UNUSED(stream) }
    virtual void writeSession(QDataStream& stream) override { Q_UNUSED(stream) }
};

#endif // SESSIONHELPERSTREAMZERO_H
