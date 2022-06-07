#ifndef SESSIONHELPERappendStrategyEGY_H
#define SESSIONHELPERappendStrategyEGY_H

#include <QDataStream>

class SessionHelperAppendStrategy
{
public:
    virtual ~SessionHelperAppendStrategy() {}
    virtual void writeSessionAppend(QDataStream& stream) = 0;
    virtual void readSessionAppend(QDataStream& stream) = 0;
};

#endif // SESSIONHELPERappendStrategyEGY_H
