#ifndef SESSIONREADWRITE_H
#define SESSIONREADWRITE_H

#include <QString>
#include <QWidget>
#include "streamstrategy.h"
#include "streamnull.h"
#include "widgetgeometry.h"

class SessionReadWrite
{
public:
    SessionReadWrite(QString machineName, IStreamStrategy* streamStrategy = new StreamNull);
    virtual ~SessionReadWrite();
    void writeSession(QWidget *widget, WidgetGeometry geometry, QString session);
    WidgetGeometry readSession(QWidget *widget, QString session);
    QString getSessionFileName(QWidget *widget, QString session);
private:
    QString m_SessionPath;
    IStreamStrategy* m_streamStrategy;
};

#endif // SESSIONREADWRITE_H
