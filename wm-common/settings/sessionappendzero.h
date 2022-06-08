#ifndef SESSIONHELPERAPPENDZERO_H
#define SESSIONHELPERAPPENDZERO_H

#include "sessionappendstrategy.h"

class SessionAppendZero : public SessionAppendStrategy
{
public:
    virtual void writeSessionAppend(QDataStream& stream) override { Q_UNUSED(stream) }
    virtual void readSessionAppend(QDataStream& stream)  override { Q_UNUSED(stream) }
};

#endif // SESSIONHELPERAPPENDZERO_H
