// headerdatei logfileview.h

#ifndef LOGFILEVIEW_H
#define LOGFILEVIEW_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtimer.h>
#include <q3textedit.h>
#include <qdialog.h>
#include <qevent.h>
#include <QResizeEvent>
#include <QCloseEvent>
#include "widgeom.h"
#include "sessionhelper.h"

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
    void show(); // mittels timer aktiviert
    
protected:
    virtual void closeEvent ( QCloseEvent * );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);

signals:
    void isVisibleSignal(bool);    
    
private:
    cWidgetGeometry m_widGeometry;
    QTimer showT;
    QStringList m_loglist;
    QTimer m_Timer;
    SessionHelper m_sessionHelper;
    
private slots:
    void showList();
    void saveConfiguration();
};

#endif