
#include "wmviewbasei.h"
#include "wmglobal.h"

WMViewBaseI::WMViewBaseI(WMViewBase *parent) :
    m_parent(parent)
{
    setDeviceName("wm3000i");
    init();
}

void WMViewBaseI::ActualizeStates()
{
    if (m_ConfData.m_bRunning)
    {
        ui->messungStartAction->setMenuText(tr("S&top"));
        ui->messungStartAction->setStatusTip(tr("Messung anhalten"));
    }
    else
    {
        ui->messungStartAction->setMenuText(tr("S&tart"));
        ui->messungStartAction->setStatusTip(tr("Messung starten"));
    }

    ui->messungSimulationAction->setOn(m_ConfData.m_bSimulation);
    ui->hilfeSelbsttestAction->setDisabled(m_ConfData.m_bSimulation);

    m_statusLabelContainer.updateLabels(&m_ConfData, m_bJustified, m_bFreqQuestionable, m_bPPSQuestionable);

    UpdateRecentFileList(recentOETFiles, m_ConfData.m_sOETFile);
    UpdateRecentFileList(recentResultFiles, m_ConfData.m_sResultFile);
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

