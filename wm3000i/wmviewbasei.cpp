
#include "wmviewbasei.h"
#include "wmglobal.h"

WMViewBaseI::WMViewBaseI(WMViewBase *parent) :
    m_parent(parent)
{
    setDeviceName("wm3000i");
    init();
}


void WMViewBaseI::updateXRangeLabel(QLabel *xRangeLabel)
{
    switch (m_ConfData.m_nMeasMode) { // statuszeile bereich x,diff,ect eintragen
    case In_IxDiff:
    case In_IxAbs:
        xRangeLabel->setText(QString("ChX=%1").arg(m_ConfData.m_sRangeX));
        xRangeLabel->show();
        break;
    case In_ECT:
        xRangeLabel->setText(QString("ChECT=%1").arg(m_ConfData.m_sRangeET));
        xRangeLabel->show();
        break;
    case In_nConvent:
        xRangeLabel->hide();
        break;
    }
}

