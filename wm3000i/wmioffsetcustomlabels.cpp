#include "wmioffsetcustomlabels.h"
#include "wmglobal.h"

void WmiOffsetCustomLabels::setUiTexts(QLabel *nLabel, QLabel *xLabel)
{
    nLabel->setText("In:");
    xLabel->setText("Ix:");
}

void WmiOffsetCustomLabels::updateValues(QLabel *nValue, QLabel *xValue, cConfData *conf, tJustValues *just)
{
    nValue->setText(QString("%1 A").arg(just->OffsetCorrDevN,10,'f',5));
    if (conf->m_nMeasMode == In_ECT)
        xValue->setText(QString("%1 V").arg(just->OffsetCorrDevX,10,'f',5));
    else
        xValue->setText(QString("%1 A").arg(just->OffsetCorrDevX,10,'f',5));
}

