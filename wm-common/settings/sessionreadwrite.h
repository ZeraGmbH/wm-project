#ifndef SESSIONREADWRITE_H
#define SESSIONREADWRITE_H

#include <QString>
#include <QWidget>
#include "sessionstreamstrategy.h"
#include "sessionstreamzero.h"
#include "widgeom.h"

class SessionReadWrite
{
public:
    SessionReadWrite(QString machineName, SessionStreamStrategy* streamStrategy = new SessionStreamZero);
    virtual ~SessionReadWrite();
    void writeSession(QWidget *widget, cWidgetGeometry geometry, QString session);
    cWidgetGeometry readSession(QWidget *widget, QString session);
    QString getSessionFileName(QWidget *widget, QString session);
private:
    QString m_SessionPath;
    SessionStreamStrategy* m_streamStrategy;
};

#endif // SESSIONREADWRITE_H
