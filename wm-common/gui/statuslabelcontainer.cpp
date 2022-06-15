#include "statuslabelcontainer.h"
#include <QFileInfo>

StatusLabelContainer::StatusLabelContainer(QStatusBar *statusBar) :
    m_statusBar(statusBar),
    m_pOwnErrTableLabel(new QLabel(statusBar)),
    m_pResultFileLabel(new QLabel(statusBar))

{
    statusBar->addPermanentWidget(m_pOwnErrTableLabel, 0);
    statusBar->addPermanentWidget(m_pResultFileLabel, 0);
}

void StatusLabelContainer::updateLabels(cConfData *confData)
{
    QString fileNameErrTable = confData->m_sOETFile;
    QFileInfo fiOETF(fileNameErrTable);
    m_pOwnErrTableLabel->setText("EFT=" + (fileNameErrTable.isEmpty() ? QObject::tr("Keine") : fiOETF.baseName()));

    QString fileNameResult = confData->m_sResultFile;
    QFileInfo fiRF(confData->m_sResultFile);
    m_pResultFileLabel->setText("MED=" + (fileNameResult.isEmpty() ? QObject::tr("Keine") : fiRF.baseName()));

}
