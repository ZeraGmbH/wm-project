#ifndef SESSIONREADWRITE_H
#define SESSIONREADWRITE_H

#include <QString>
#include <QWidget>
#include "sessionappendstrategy.h"
#include "sessionappendzero.h"
#include "widgeom.h"

class SessionReadWrite
{
public:
    SessionReadWrite(QString machineName, SessionAppendStrategy* appendStrategy = new SessionAppendZero);
    virtual ~SessionReadWrite();
    void writeSession(QWidget *widget, cWidgetGeometry geometry, QString session);
    cWidgetGeometry readSession(QWidget *widget, QString session);
    QString getSessionFileName(QWidget *widget, QString session);
private:
    QString m_SessionPath;
    SessionAppendStrategy* m_appendStrategy;
};

#endif // SESSIONREADWRITE_H
