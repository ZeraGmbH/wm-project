#ifndef SESSIONHELPERappendStrategyEGY_H
#define SESSIONHELPERappendStrategyEGY_H

#include <QDataStream>

class SessionAppendStrategy
{
public:
    virtual ~SessionAppendStrategy() {}
    virtual void readSessionAppend(QDataStream& stream) = 0;
    virtual void writeSessionAppend(QDataStream& stream) = 0;
};

#endif // SESSIONHELPERappendStrategyEGY_H
