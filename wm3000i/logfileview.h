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
//Added by qt3to4:
#include <QResizeEvent>
#include <QCloseEvent>
#include "widgeom.h"

class CLogFileView:public QDialog
{
    Q_OBJECT
    
public:
    CLogFileView(const QString,const long, QWidget *parent, const char * wname = 0);
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
    QTimer showT, m_Timer;
    QStringList m_loglist;
    
private slots:
    void showList();
    void saveConfiguration();
};

#endif
