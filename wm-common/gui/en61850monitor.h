#ifndef EN61850MONBASE_H
#define EN61850MONBASE_H

#include "en61850.h"
#include "widgetgeometry.h"
#include "sessionstreamer.h"
#include "settingschangetimer.h"
#include <QDialog>

namespace Ui {
    class EN61850monitor;
}

class EN61850monitor : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    explicit EN61850monitor(QWidget* parent, QString machineName);
    ~EN61850monitor();
public slots:
    virtual void onShowHide(bool shw);
    virtual void onETHStatus(cEN61850Info *ethInfo);
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
signals:
    void sigRequestInformation();
    void sigIsVisible(bool);
    void sigResetETHStatus();
protected:
    virtual void closeEvent(QCloseEvent * ce) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent*) override;
    virtual void accept() override;
    virtual void reject() override;
private slots:
    void onPollTimer();
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;
    void prepareNextPoll();
    void actualizeByteCount(cEN61850Info *ethInfo);
    void actualizeSyncLostCount(cEN61850Info *ethInfo);
    Ui::EN61850monitor *ui;
    QTimer m_PollTimer;
    SettingsChangeTimer m_settingsChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
};

#endif // EN61850MONBASE_H
