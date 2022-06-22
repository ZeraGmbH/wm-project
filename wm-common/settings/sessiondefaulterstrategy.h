#ifndef SESSIONDEFAULTERSTRATEGY_H
#define SESSIONDEFAULTERSTRATEGY_H

#include <QSharedPointer>

class ISessionDefaulterStrategy
{
public:
    virtual void setDefaults() = 0;
};

typedef QSharedPointer<ISessionDefaulterStrategy> ISessionDefaulterStrategyPtr;

#endif // SESSIONDEFAULTERSTRATEGY_H
