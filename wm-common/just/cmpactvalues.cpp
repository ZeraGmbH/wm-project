#include "cmpactvalues.h"
#include "tools.h"

#include "math.h"

const double PI_180 = 1.74532925e-2;

cmpActValues::cmpActValues(cConfData *confData, cwmActValues *actValues) :
    m_confData(confData),
    m_actValues(actValues)
{

}

void cmpActValues::berechnung(cOwnError *ownErr)
{
    //phikBerechnung();
    phikArteche();
    korrUndPrimarBerechnung(ownErr);
    fehlerBerechnung();
}

void cmpActValues::fehlerBerechnung()
{
    m_actValues->AngleError = UserAtan(m_actValues->VekX) - UserAtan(m_actValues->VekN);
    m_actValues->AngleError = normWinkelrad_PIPI(m_actValues->AngleError);

    double err;

    if (m_confData->m_bDCmeasurement)
        err = (m_actValues->VekX.re() - m_actValues->VekN.re()) / m_actValues->VekN.re();
    else
    {
        double absN;
        absN = fabs(m_actValues->VekN);
        err = (fabs(m_actValues->VekX) -absN) / absN;
    }

    m_actValues->AmplErrorIEC = 100.0 * err;
    m_actValues->RCF = 1.0 / (1.0 + err);   // ratio corretion factor
}

void cmpActValues::phikBerechnung()
{
    double phik;
    //  korrektur des kanal X vektors mit der bekannten abtastverzögerung und dem bekannten phasenfehler des prüflings
    phik = ( ( -360.0  * m_actValues->Frequenz * m_confData->m_fxTimeShift * 1.0e-3 ) - m_confData->m_fxPhaseShift) * PI_180;
    m_actValues->VekXSek *= complex( cos(phik),sin(phik) );
}

void cmpActValues::korrUndPrimarBerechnung(cOwnError *ownErr)
{
    // eigenfehler korrektur des normwandlers

    m_actValues->UInCorr = ownErr->GetOECorrVector(); // achtung complex !!!!!
    m_actValues->VekNSek *= m_actValues->UInCorr;

    // umrechnen auf primärgrößen

    m_actValues->VekN = m_actValues->VekNSek * m_kn;
    m_actValues->VekX = m_actValues->VekXSek * m_kx;
    m_actValues->VekDX = m_actValues->VekX - m_actValues->VekN;
}

void cmpActValues::setFactors(const double kn, const double kx)
{
    m_kn = kn;
    m_kx = kx;
}

void cmpActValues::phikArteche()
{
    double phik;
    //  korrektur des kanal X vektors mit der bekannten abtastverzögerung und dem bekannten phasenfehler des prüflings
    phik = ( m_confData->m_fxPhaseShift + ( -360.0  * m_actValues->Frequenz * m_confData->m_fxTimeShift * 1.0e-3 )) * PI_180;
    m_actValues->VekXSek /= complex( cos(phik),sin(phik) );
}

