#ifndef WMRAWACTUALVALBASE_H
#define WMRAWACTUALVALBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <QTimer>

#include "wmactvalues.h"
#include "widgetgeometry.h"
#include "confdata.h"
#include "wmrawactualconfigbase.h"
#include "tools.h"

namespace Ui {
    class WMRawActualValBase;
}

class WMRawActualValBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMRawActualValBase( QWidget* parent = 0);
    ~WMRawActualValBase();

public slots:
    virtual void onShowHide(bool shw);
    virtual void ReceiveAVDataSlot( cwmActValues * ActValues );
    virtual void SetConfInfoSlot( cConfData * cd );
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
    virtual void ReceiveVektorDispFormat( int m, int m2, int m3 );

signals:
    void sigIsVisible(bool);
    void SendVektorDispFormat(bool,int,int,int);

protected:
    virtual void closeEvent(QCloseEvent * ce) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override;

private:
    Ui::WMRawActualValBase *ui;
    int AmplPrimSekMode;
    cwmActValues m_ActValues;
    WidgetGeometry m_widGeometry;
    cConfData *m_pConfData;
    int AmplDispMode;
    int WinkelDispMode;
    WMRawActualConfigBase* m_pContextMenu;
    QTimer m_Timer;

    void init();
    void destroy();

private slots:
    void onSaveConfig();

};

#endif // WMRAWACTUALVALBASE_H
