#include "statuslabelcontainer.h"
#include <QFileInfo>

StatusLabelContainer::StatusLabelContainer(QStatusBar *statusBar, IStatusLabelXRangeParent *xRangeLabelParent) :
    m_statusBar(statusBar),
    m_xRangeLabelParent(xRangeLabelParent),
    m_pOwnErrTableLabel(new QLabel(statusBar)),
    m_pResultFileLabel(new QLabel(statusBar)),
    m_pRangeNLabel(new QLabel(statusBar)),
    m_pRangeXLabel(new QLabel(statusBar)),
    m_pRunningLabel(new QLabel(statusBar))
{
    statusBar->addPermanentWidget(m_pOwnErrTableLabel, 0);
    statusBar->addPermanentWidget(m_pResultFileLabel, 0);
    statusBar->addPermanentWidget(m_pRangeNLabel, 0);
    statusBar->addPermanentWidget(m_pRangeXLabel, 0);
    statusBar->addPermanentWidget(m_pRunningLabel, 0);
}

void StatusLabelContainer::updateLabels(cConfData *confData)
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
}
