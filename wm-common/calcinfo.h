#ifndef CALCINFO_H
#define CALCINFO_H

#include <QString>
#include <Q3PtrList>

class cCalcInfo
{
public:
    cCalcInfo(const QString chn, const QString rng) : m_sChannel(chn), m_sRange(rng) {}
    QString m_sChannel;
    QString m_sRange;
};

typedef QList<cCalcInfo*> cCalcInfoList;

#endif // CALCINFO_H
