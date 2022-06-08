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

#include <qvariant.h>
#include <qdialog.h>
#include <QTimer>

#include "en61850.h"
#include "widgeom.h"
#include "sessionreadwrite.h"

namespace Ui {
    class EN61850monitor;
}

class EN61850monitor : public QDialog
{
    Q_OBJECT

public:
    explicit EN61850monitor(QWidget* parent, QString machineName);
    ~EN61850monitor();

public slots:
    virtual void ShowHideSlot( bool b );
    virtual void SetETHStatusSlot( cEN61850Info * ethInfo );
    bool LoadSession( QString session );
    void SaveSession( QString session );

signals:
    void InformationRequest();
    void isVisibleSignal(bool);
    void ResetETHStatus();

protected:
    int newVariable;
    virtual void closeEvent( QCloseEvent * ce );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);

protected slots:
    virtual void accept();
    virtual void reject();


private:
    Ui::EN61850monitor *ui;
    QTimer *m_pTimer, m_Timer;
    cWidgetGeometry m_widGeometry;
    cEN61850Info ETHStatus;
    SessionReadWrite m_sessionReadWrite;

    void init();
    void destroy();

private slots:
    void TimerSlot();
    void saveConfiguration();

};

#endif // EN61850MONBASE_H
