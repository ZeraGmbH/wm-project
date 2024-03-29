// headerdatei logfileview.h

#ifndef LOGFILEVIEW_H
#define LOGFILEVIEW_H

#include "sessionstreamer.h"
#include "settingschangetimer.h"
#include "widgetgeometry.h"
#include <QTimer>
#include <QDialog>
#include <QPlainTextEdit>
#include <QResizeEvent>
#include <QCloseEvent>

class CLogFileView : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
    
public:
    CLogFileView(const QString cap,
                 const long max,
                 QWidget *parent,
                 const char *wname,
                 QString machineName);
    QPlainTextEdit *m_pText;
    
public slots:    
    void onSaveSession(QString session);
    bool onLoadSession(QString session);
    void onShowHide(bool shw);
    void onAddLogText(const QString&);

protected:
    virtual void closeEvent(QCloseEvent *ce ) override;
    virtual void resizeEvent(QResizeEvent*) override;
    virtual void moveEvent(QMoveEvent*) override;

signals:
    void sigIsVisible(bool);
    
private slots:
    void showList();
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;

    QTimer m_timerDelayShow;
    QStringList m_loglist;
    SettingsChangeTimer m_settingsChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
};

#endif
