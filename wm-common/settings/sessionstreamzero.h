#ifndef SESSIONHELPERSTREAMZERO_H
#define SESSIONHELPERSTREAMZERO_H

#include "streamstrategy.h"

class SessionStreamZero : public IStreamStrategy
{
public:
    virtual void read(QDataStream& stream)  override { Q_UNUSED(stream) }
    virtual void write(QDataStream& stream) override { Q_UNUSED(stream) }
};

typedef QSharedPointer<SessionStreamZero> SessionStreamZeroPtr;

#endif // SESSIONHELPERSTREAMZERO_H
