#ifndef WMRAWACTUALVALBASE_H
#define WMRAWACTUALVALBASE_H

#include "ui_wmrawactualvalbase.h"
#include "wmactvalues.h"
#include "widgetgeometry.h"
#include "confdata.h"
#include "wmrawactualconfigbase.h"
#include "settingschangetimer.h"
#include "sessionstreamer.h"
#include <QDialog>

class WMRawActualValBase : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    explicit WMRawActualValBase(QWidget* parent, QString machineName);
    ~WMRawActualValBase();
    void activateContextMenu();
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
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override;
private slots:
    void onSaveConfig();
    void onWriteStreamForGeomChange();
private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;
    void setInitialDefaults();
    void setSettingslabel();
    Ui::WMRawActualValBase *ui;
    cwmActValues m_ActValues;
    cConfData *m_pConfData;
    int AmplDispMode;
    int PrimSekDispMode;
    int WinkelDispMode;
    WMRawActualConfigBase* m_pContextMenu;
    SettingsChangeTimer m_settingsChangeTimer;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
    QTimer m_Timer;
};

#endif // WMRAWACTUALVALBASE_H
