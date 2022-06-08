// headerdatei logfileview.h

#ifndef LOGFILEVIEW_H
#define LOGFILEVIEW_H

#include <QTimer>
#include <QDialog>
#include "sessionreadwrite.h"
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
    void ShowHideLogFileSlot(bool); // sichtbar oder nicht
    void AddLogTextSlot(const QString&); // wenn neue daten einlaufen

protected:
    virtual void closeEvent ( QCloseEvent * );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);

signals:
    void isVisibleSignal(bool);    
    
private:
    cWidgetGeometry m_widGeometry;
    QTimer m_timerDelayShow;
    QStringList m_loglist;
    QTimer m_Timer;
    SessionReadWrite m_sessionReadWrite;
    
private slots:
    void showList();
    void saveConfiguration();
};

#endif
