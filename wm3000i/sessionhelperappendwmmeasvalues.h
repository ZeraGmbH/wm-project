#ifndef SESSIONHELPERAPPENDWMMEASVALUES_H
#define SESSIONHELPERAPPENDWMMEASVALUES_H

#include <sessionhelperappendstrategy.h>

class WMMeasValuesBase;

class SessionHelperAppendWMMeasValues : public SessionHelperAppendStrategy
{
public:
    SessionHelperAppendWMMeasValues(WMMeasValuesBase *values);
    virtual void writeSessionAppend(QDataStream& stream) override;
    virtual void readSessionAppend(QDataStream& stream)  override;
private:
    WMMeasValuesBase *m_values;
};

#endif // SESSIONHELPERAPPENDWMMEASVALUES_H
