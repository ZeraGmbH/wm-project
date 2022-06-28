#include "wmuoffsetcustomlabels.h"

void WmuOffsetCustomLabels::setUiTexts(QLabel *nLabel, QLabel *xLabel)
{
    nLabel->setText("Un:");
    xLabel->setText("Ux:");
}

void WmuOffsetCustomLabels::updateValues(QLabel *nValue, QLabel *xValue, cConfData* conf, tJustValues* just)
{
    Q_UNUSED(conf)
    nValue->setText(QString("%1 V").arg(just->OffsetCorrDevN,10, 'f', 5));
    xValue->setText(QString("%1 V").arg(just->OffsetCorrDevX,10, 'f', 5));
}

