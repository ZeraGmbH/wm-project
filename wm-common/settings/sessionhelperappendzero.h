#ifndef SESSIONHELPERAPPENDZERO_H
#define SESSIONHELPERAPPENDZERO_H

#include "sessionhelperappendstrategy.h"

class SessionHelperAppendZero : public SessionHelperAppendStrategy
{
public:
    virtual void writeSessionAppend(QDataStream& stream) override { Q_UNUSED(stream) }
    virtual void readSessionAppend(QDataStream& stream)  override { Q_UNUSED(stream) }
};

#endif // SESSIONHELPERAPPENDZERO_H
