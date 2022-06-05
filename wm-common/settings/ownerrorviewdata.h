#ifndef OWNERRORVIEWDATA_H
#define OWNERRORVIEWDATA_H

#include <QString>

struct cOwnErrorViewData
{
    bool m_bValid;
    QString m_sTrName; // der wandlername
    QString m_sPrim;
    QString m_sSec;
    QString m_sLoad;
    QString m_sAmpl;
    QString m_sPhase;
};

#endif // OWNERRORVIEWDATA_H
