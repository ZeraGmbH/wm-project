#include "sessionhelperappendwmmeasvalues.h"
#include "wmmeasvaluesbase.h"

SessionHelperAppendWMMeasValues::SessionHelperAppendWMMeasValues(WMMeasValuesBase *values) :
    m_values(values)
{
}

void SessionHelperAppendWMMeasValues::writeSessionAppend(QDataStream &stream)
{
    for (int i = 0; i < 4; i++)
        stream << m_values->m_Format[i];

    stream << m_values->m_nDisplayMode;
    stream << m_values->m_nLPDisplayMode;
}

void SessionHelperAppendWMMeasValues::readSessionAppend(QDataStream &stream)
{
    for (int i = 0; i< 4; i++)
        stream >> m_values->m_Format[i];

    stream >> m_values->m_nDisplayMode;
    stream >> m_values->m_nLPDisplayMode;
}
