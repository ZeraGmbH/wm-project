#include "widgetpersitence.h"

WidgetPersitence::WidgetPersitence(IWidgetAdapter *wid, QString dirNameSessionFiles, SessionAppendStrategy *appendStrategy) :
    m_myWidget(wid),
    m_appendStrategy(appendStrategy),
    m_dirNameSessionFiles(dirNameSessionFiles)
{
}
