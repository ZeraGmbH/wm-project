#ifndef WMOEVIEWBASE_H
#define WMOEVIEWBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QTimer>

#include "ownerror.h"
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
