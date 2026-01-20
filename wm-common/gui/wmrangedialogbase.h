#ifndef WMRANGEDIALOGBASE_H
#define WMRANGEDIALOGBASE_H

#include "formatinfo.h"
#include "qdialog.h"
#include "screenshooter.h"
#include "settingschangetimer.h"
#include "widgetgeometry.h"
#include <sessionstreamer.h>


class wmRangeDialogBase : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    wmRangeDialogBase(QWidget* parent, QString machineName);
    ~wmRangeDialogBase();
    void setScreenShooter(screenshooter* poi);

public slots:
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
    void takeScreenshoots();
    void takeScreenshootFinished();
protected:
    virtual void closeEvent(QCloseEvent *ce) override;
    virtual void resizeEvent (QResizeEvent*) override;
    virtual void moveEvent(QMoveEvent*) override;
    virtual QWidget* getChildThis() = 0;
private slots:
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;

    cFormatInfo m_Format[4];
    SettingsChangeTimer m_settingsChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
    screenshooter* mScrShooter;
};

#endif // WMRANGEDIALOGBASE_H
