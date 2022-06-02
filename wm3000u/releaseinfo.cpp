#include <QProcess>
#include <QString>

#include "releaseinfo.h"
#include "wmglobal.h"

#define ReleaseInfoFilePath "/opt/zera/conf/CHANGELOG"

cReleaseInfo::cReleaseInfo(QObject *parent)
{
    m_pProcess = new QProcess(parent);
}


cReleaseInfo::~cReleaseInfo()
{
    delete m_pProcess;
}

void cReleaseInfo::show()
{
    m_pProcess->start(QString("gedit %1").arg(ReleaseInfoFilePath));
}

