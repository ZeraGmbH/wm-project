// headerdatei logfileview.h

#ifndef LOGFILEVIEW_H
#define LOGFILEVIEW_H

#include "sessionstreamer.h"
#include "geometrychangetimer.h"
#include "widgetgeometry.h"
#include <QTimer>
#include <QDialog>
#include <Q3TextEdit>
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
    virtual ~CLogFileView();
    Q3TextEdit *m_pText;
    
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
    GeometryChangeTimer m_geomChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
};

#endif
