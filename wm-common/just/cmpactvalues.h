#ifndef CMPACTVALUES_H
#define CMPACTVALUES_H

#include "confdata.h"
#include "wmactvalues.h"
#include "ownerror.h"

class cmpActValues
{
public:
    cmpActValues(cConfData *confData, cwmActValues *actValues);
    void berechnung(cOwnError *ownErr);
    void phikBerechnung();
    void korrUndPrimarBerechnung(cOwnError *ownErr);
    void setFactors(const double kn, const double kx);
    void fehlerBerechnung();
    void phikArteche();

private:
    cConfData *m_confData;
    cwmActValues *m_actValues;
    double m_kx, m_kn;
    complex m_phiTemp, m_phiTempDiv;

};

#endif // CMPACTVALUES_H
