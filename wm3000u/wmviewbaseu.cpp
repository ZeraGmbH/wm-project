
#include "wmviewbaseu.h"
#include "wmglobal.h"



WMViewBaseU::WMViewBaseU(WMViewBase *parent) :
    m_parent(parent)
{
    setDeviceName("wm3000u");
    init();
}

void WMViewBaseU::ActualizeStates()
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

