#include "statuslabelcontainer.h"
#include "tcpconfig.h"
#include <QFileInfo>

StatusLabelContainer::StatusLabelContainer(IStatusLabelXRangeParent *xRangeLabelParent) :
    m_xRangeLabelParent(xRangeLabelParent)
{
}

void StatusLabelContainer::updateLabels(cConfData *confData, bool adjusted, bool bFreqQuestionable)
{
    QString fileNameErrTable = confData->m_sOETFile;
    QFileInfo fiOETF(fileNameErrTable);
    m_pOwnErrTableLabel->setText("EFT=" + (fileNameErrTable.isEmpty() ? QObject::tr("Keine") : fiOETF.baseName()));
    QString fileNameResult = confData->m_sResultFile;
    QFileInfo fiRF(confData->m_sResultFile);

    m_pResultFileLabel->setText("MED=" + (fileNameResult.isEmpty() ? QObject::tr("Keine") : fiRF.baseName()));
    m_pRangeNLabel->setText(QString("ChN=%1").arg(confData->m_sRangeN));
    m_xRangeLabelParent->updateXRangeLabel(m_pRangeXLabel);
    m_pRunningLabel->setText(confData->m_bRunning ? QObject::tr("Gestartet") : QObject::tr("Gestoppt"));
    m_pSimulationLabel->setText(confData->m_bSimulation ? QObject::tr("Simulation") : QObject::tr("Reale Messung"));
    m_pSimulationLabel->setStyleSheet(confData->m_bSimulation ? "QLabel {color:red;}" : "QLabel {color:text;}");
    if(confData->m_bSimulation) {
        m_pStatusLabel->setText("");
        m_pStatusLabel->setStyleSheet("QLabel {color:text;}");
    }
    else {
        m_pStatusLabel->setText(adjusted ? QObject::tr("Justiert") : QObject::tr("Nicht justiert"));
        m_pStatusLabel->setStyleSheet(adjusted ? "QLabel {color:text;}" : "QLabel {color:red;}");
    }
    m_pFreqLabel->setText( bFreqQuestionable ? QObject::tr("!!SignalFrequenz!!") : "");
}

void StatusLabelContainer::CreateStatusBar(QStatusBar *statusBar, QWidget *labelParent)
{
    m_pOwnErrTableLabel = new QLabel(labelParent);
    m_pResultFileLabel = new QLabel(labelParent);
    m_pRangeNLabel = new QLabel(labelParent);
    m_pRangeXLabel = new QLabel(labelParent);
    m_pRunningLabel = new QLabel(labelParent);
    m_pSimulationLabel = new QLabel(labelParent);
    m_pStatusLabel = new QLabel(labelParent);
    m_pFreqLabel = new QLabel(labelParent);
    m_pIPLabel = new QLabel(labelParent);
    m_pDummyLabel = new QLabel(labelParent);

    statusBar->addPermanentWidget(m_pOwnErrTableLabel, 0);
    statusBar->addPermanentWidget(m_pResultFileLabel, 0);
    statusBar->addPermanentWidget(m_pRangeNLabel, 0);
    statusBar->addPermanentWidget(m_pRangeXLabel, 0);
    statusBar->addPermanentWidget(m_pRunningLabel, 0);
    statusBar->addPermanentWidget(m_pSimulationLabel, 0);
    statusBar->addPermanentWidget(m_pStatusLabel, 0);
    statusBar->addPermanentWidget(m_pFreqLabel, 0);
    statusBar->addPermanentWidget(m_pIPLabel, 0);
    statusBar->addPermanentWidget(m_pDummyLabel,1);

    connect(&m_iPPollTimer,SIGNAL(timeout()), this, SLOT(onIpPollTimer()));
    m_iPPollTimer.setSingleShot(false);
    m_iPPollTimer.start(3000);
}

void StatusLabelContainer::onIpPollTimer()
{
    m_pIPLabel->setText(QString("IP=%1").arg(getIPv4AddressList().join("/")));
}
