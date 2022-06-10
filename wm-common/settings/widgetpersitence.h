#ifndef WIDGETPERSITENCE_H
#define WIDGETPERSITENCE_H

#include "sessionappendstrategy.h"
#include "sessionappendzero.h"
#include "iwidgetadapter.h"
#include "widgeom.h"

class WidgetPersitence
{
public:
    WidgetPersitence(IWidgetAdapter* wid, QString dirNameSessionFiles, SessionAppendStrategy* appendStrategy = new SessionAppendZero);
    void handleResizeMove(IWidgetAdapter* wid);
    void handleClose(IWidgetAdapter* wid);
    bool load(IWidgetAdapter* wid, QString sessionName = ".ses");
    void save(IWidgetAdapter* wid, QString sessionName = ".ses");
private:
    IWidgetAdapter* m_myWidget;
    SessionAppendStrategy* m_appendStrategy;
    QString m_dirNameSessionFiles;
};

#endif // WIDGETPERSITENCE_H
