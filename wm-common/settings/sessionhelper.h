#ifndef SESSIONHELPER_H
#define SESSIONHELPER_H

#include <QString>
#include <QWidget>
#include "sessionhelperappendstrategy.h"
#include "sessionhelperappendzero.h"
#include "widgeom.h"

class SessionHelper
{
public:
    SessionHelper(QString machineName, SessionHelperAppendStrategy* appendStrategy = new SessionHelperAppendZero);
    virtual ~SessionHelper();
    void writeSession(QWidget *widget, cWidgetGeometry geometry, QString session);
    cWidgetGeometry readSession(QWidget *widget, QString session);
private:
    QString getSessionFileName(QWidget *widget, QString session);
    QString m_SessionPath;
    SessionHelperAppendStrategy* m_appendStrategy;
};

#endif // SESSIONHELPER_H
