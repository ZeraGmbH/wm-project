#ifndef WMOFFSETVALBASE_H
#define WMOFFSETVALBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QTimer>

#include "wm3000u.h"
#include "widgetgeometry.h"
#include "confdata.h"
#include "tools.h"

namespace Ui {
    class WMOffsetValBase;
}

class WMOffsetValBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMOffsetValBase( QWidget* parent = 0);
    ~WMOffsetValBase();

public slots:
    virtual void onShowHide( bool b );
    virtual void ReceiveJustDataSlot( tJustValues *JustValues );
    bool onLoadSession(QString session);
    void onSaveSession(QString session);

signals:
    void sigIsVisible(bool);

protected:
    virtual void closeEvent( QCloseEvent * ce );
    virtual void resizeEvent ( QResizeEvent *);
    virtual void moveEvent( QMoveEvent *);

private:
    Ui::WMOffsetValBase *ui;
    tJustValues m_JustValues;
    WidgetGeometry m_widGeometry;

    QTimer m_Timer;

    void init();
    void destroy();

private slots:
    void onSaveConfig();

};


#endif // WMOFFSETVALBASE_H
