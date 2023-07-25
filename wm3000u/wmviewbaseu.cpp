
#include "wmviewbaseu.h"
#include "wmglobal.h"



WMViewBaseU::WMViewBaseU(WMViewBase *parent) :
    m_parent(parent)
{
    setDeviceName("wm3000u");
    init();
}


void WMViewBaseU::updateXRangeLabel(QLabel *xRangeLabel)
{
    switch (m_ConfData.m_nMeasMode) { // statuszeile bereich x,diff,ect eintragen
    case Un_UxAbs:
        xRangeLabel->setText(QString("ChX=%1").arg(m_ConfData.m_sRangeX));
        xRangeLabel->show();
        break;
    case Un_EVT:
        xRangeLabel->setText(QString("ChEVT=%1").arg(m_ConfData.m_sRangeET));
        xRangeLabel->show();
        break;
    case Un_nConvent:
        xRangeLabel->hide();
        break;
    }
}

