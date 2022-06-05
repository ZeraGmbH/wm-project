/****************************************************************************
** Form interface generated from reading ui file 'wmoeviewbase.ui'
**
** Created: Mi Sep 19 14:11:07 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMOEVIEWBASE_H
#define WMOEVIEWBASE_H


#include <qvariant.h>
#include <qdialog.h>
#include <QTimer>

#include "ownerrorviewdata.h"
#include "widgeom.h"
#include "sessionhelper.h"


namespace Ui {
    class WMOeViewBase;
}

class WMOeViewBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMOeViewBase(QWidget* parent, QString machineName);
    ~WMOeViewBase();

public slots:
    virtual void ReceiveOEViewDataSlot( cOwnErrorViewData * oe );
    virtual void ShowHideOESlot( bool b );
    void SaveSession( QString session );
    bool LoadSession( QString session );

signals:
    void isVisibleSignal(bool);

protected:
    virtual void closeEvent( QCloseEvent * ce );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);

private:
    Ui::WMOeViewBase *ui;
    cWidgetGeometry m_widGeometry;
    cOwnErrorViewData m_OwnErrorView;
    QTimer m_Timer;
    SessionHelper m_sessionHelper;
    void init();
    void destroy();

private slots:
    void saveConfiguration();

};

#endif // WMOEVIEWBASE_H
