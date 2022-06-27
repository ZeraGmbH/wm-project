/****************************************************************************
** Form interface generated from reading ui file 'en61850monitor.ui'
**
** Created: Mi Sep 19 12:39:27 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef EN61850MONBASE_H
#define EN61850MONBASE_H

#include "en61850.h"
#include "widgetgeometry.h"
#include "sessionstreamer.h"
#include "geometrychangetimer.h"
#include <QDialog>
#include <QTimer>

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
    GeometryChangeTimer m_geomChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
};

#endif // EN61850MONBASE_H
