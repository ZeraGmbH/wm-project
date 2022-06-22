#ifndef SESSIONSTREAMSTRATEGY_H
#define SESSIONSTREAMSTRATEGY_H

#include <QDataStream>

class SessionStreamStrategy
{
public:
    virtual ~SessionStreamStrategy() {}
    virtual void readSession(QDataStream& stream) = 0;
    virtual void writeSession(QDataStream& stream) = 0;
};

#endif // SESSIONSTREAMSTRATEGY_H
