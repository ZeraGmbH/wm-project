#include "releaseinfo.h"

void cReleaseInfo::show()
{
    m_process.start(QString("gedit %1").arg("/opt/zera/conf/CHANGELOG"));
}
