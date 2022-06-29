#ifndef WMRAWACTUALVALBASE_H
#define WMRAWACTUALVALBASE_H

#include "ui_wmrawactualvalbase.h"
#include "wmactvalues.h"
#include "widgetgeometry.h"
#include "confdata.h"
#include "wmrawactualconfigbase.h"
#include <geometrychangetimer.h>
#include <QDialog>

class WMRawActualValBase : public QDialog
{
    Q_OBJECT
public:
    explicit WMRawActualValBase( QWidget* parent = 0);
    ~WMRawActualValBase();
public slots:
    void onShowHide(bool shw);
    void ReceiveAVDataSlot(cwmActValues *ActValues);
    void SetConfInfoSlot(cConfData *cd);
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
    void ReceiveVektorDispFormat(int m, int m2, int m3);
signals:
    void sigIsVisible(bool);
    void SendVektorDispFormat(bool,int,int,int);
protected:
    virtual void closeEvent(QCloseEvent * ce) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void moveEvent(QMoveEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override;
private slots:
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    Ui::WMRawActualValBase *ui;
    cwmActValues m_ActValues;
    cConfData *m_pConfData;
    int AmplDispMode;
    int PrimSekDispMode;
    int WinkelDispMode;
    WMRawActualConfigBase* m_pContextMenu;
    GeometryChangeTimer m_geomChangeTimer;
    WidgetGeometry m_geomToFromStream;
};

#endif // WMRAWACTUALVALBASE_H
