#ifndef WMRANGEDIALOGBASE_H
#define WMRANGEDIALOGBASE_H

#include "formatinfo.h"
#include "qdialog.h"
#include "widgetgeometry.h"
#include <sessionstreamer.h>


class wmRangeDialogBase : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    wmRangeDialogBase(QWidget* parent, QString machineName);
public slots:
    bool onLoadSession(QString session);
    void onSaveSession(QString session);

private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;

    cFormatInfo m_Format[4];
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;

};

#endif // WMRANGEDIALOGBASE_H
