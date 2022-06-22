#ifndef STREAMSTRATEGY_H
#define STREAMSTRATEGY_H

#include <QDataStream>
#include <QSharedPointer>

class IStreamStrategy
{
public:
    virtual ~IStreamStrategy() {}
    virtual void read(QDataStream& stream) = 0;
    virtual void write(QDataStream& stream) = 0;
};

typedef QSharedPointer<IStreamStrategy> IStreamStrategyPtr;

#endif // STREAMSTRATEGY_H
