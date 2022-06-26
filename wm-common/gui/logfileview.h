// headerdatei logfileview.h

#ifndef LOGFILEVIEW_H
#define LOGFILEVIEW_H

#include "sessionreadwrite.h"
#include "geometrychangehandler.h"
#include <QTimer>
#include <QDialog>
#include <Q3TextEdit>
#include <QResizeEvent>
#include <QCloseEvent>

class CLogFileView:public QDialog
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
    void SaveSession(QString);
    bool LoadSession(QString);
    void ShowHideLogFileSlot(bool shw);
    void AddLogTextSlot(const QString&);

protected:
    virtual void closeEvent(QCloseEvent *ce ) override;
    virtual void resizeEvent(QResizeEvent*) override;
    virtual void moveEvent(QMoveEvent*) override;

signals:
    void isVisibleSignal(bool);    
    
private:
    QTimer m_timerDelayShow;
    QStringList m_loglist;
    SessionReadWrite m_sessionReadWrite;
    GeometryChangeHandler m_geomHandler;

private slots:
    void showList();
    void saveConfiguration();
};

#endif
