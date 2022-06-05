#ifndef SESSIONHELPER_H
#define SESSIONHELPER_H

#include <QString>
#include <QWidget>
#include "widgeom.h"

class SessionHelper
{
public:
    SessionHelper(QString machineName);
    void writeSession(QWidget *widget, cWidgetGeometry geometry, QString session);
    cWidgetGeometry readSession(QWidget *widget, QString session);
private:
    QString getSessionFileName(QWidget *widget, QString session);
    QString m_SessionPath;
};

#endif // SESSIONHELPER_H
