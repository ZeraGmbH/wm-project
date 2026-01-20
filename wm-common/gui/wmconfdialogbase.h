#ifndef WMCONFDIALOGBASE_H
#define WMCONFDIALOGBASE_H

#include "confdata.h"
#include "confguihelper.h"
#include "formatinfo.h"
#include "qdialog.h"
#include "screenshooter.h"
#include "sessionstreamer.h"
#include "settingschangetimer.h"
#include "widgetgeometry.h"
#include "wmmessagebox.h"
#include <QObject>

class wmconfdialogbase : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    wmconfdialogbase(QWidget* parent, QString machineName);
    ~wmconfdialogbase();
    bool is_3( const QString & s );
    bool is_w3( const QString & s );
    void setScreenShooter(screenshooter *poi);
public slots:
    bool onLoadSession(QString session);
    void onSaveSession(QString session);

    void screenshooterTriggered();
    void keyboardScreenshorTriggerd();
    void takeScreenshoots();
    void takeScreenshootFinished();

protected:
    screenshooter* mScrShooter;
    cConfData m_ConfData, m_ConfDataTemp;
    bool m_bRemoteCtrl;
    QString m_sText;
    wmKeyboardForm* mWmKeyBoard;
    confGuiHelper *mGuiHelper;
    wmMessageBox mWmMsgBox;

    void setRatioPointer(QWidget *poi);
    void showRatio();
    void cancelRatio();
    void SuggestASDUs();
private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;
    SettingsChangeTimer m_settingsChangeTimer;
private slots:
    void onWriteStreamForGeomChange();
protected:
    virtual QWidget* getRatioWidgetThis() = 0;
    virtual QTabWidget* getConfWidgetThis() = 0;

    cFormatInfo m_Format[4];
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
    QWidget* mpTransRatio;

};

#endif // WMCONFDIALOGBASE_H
