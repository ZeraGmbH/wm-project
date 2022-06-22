#ifndef STREAMNULL_H
#define STREAMNULL_H

#include "streamstrategy.h"

class StreamNull : public IStreamStrategy
{
public:
    virtual void read(QDataStream& stream)  override { Q_UNUSED(stream) }
    virtual void write(QDataStream& stream) override { Q_UNUSED(stream) }
};

typedef QSharedPointer<StreamNull> StreamNullPtr;

#endif // STREAMNULL_H
