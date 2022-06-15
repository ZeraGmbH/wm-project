#include "statuslabelcontainer.h"
#include <QFileInfo>

StatusLabelContainer::StatusLabelContainer(QStatusBar *statusBar) :
    m_statusBar(statusBar),
    m_pOwnErrTableLabel(new QLabel(statusBar))
{
    statusBar->addPermanentWidget(m_pOwnErrTableLabel, 0);
}

void StatusLabelContainer::updateLabels(cConfData *confData)
{
    QString ownErrTableFile = confData->m_sOETFile;
    QFileInfo fi (ownErrTableFile);
    m_pOwnErrTableLabel->setText("EFT="+((ownErrTableFile=="") ? QObject::tr("Keine") : fi.baseName()));

}
