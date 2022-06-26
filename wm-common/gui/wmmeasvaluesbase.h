#ifndef WMMEASVALUESBASE_H
#define WMMEASVALUESBASE_H

#include "bigletter.h"
#include "confdata.h"
#include "widgetgeometry.h"
#include "wmmeasconfigbase.h"
#include "formatinfo.h"
#include "wmactvalues.h"
#include "sessionstreamer.h"
#include "geometrychangehandler.h"
#include "geometrytowidget.h"
#include <QDialog>
#include <QLabel>

namespace Ui {
    class WMMeasValuesBase;
}

class WMMeasValuesBase : public QDialog, public ISessionStreamImplementor
{
    Q_OBJECT
public:
    explicit WMMeasValuesBase(QWidget* parent, QString machineName, QList<eUnit *> lpUnitList);
    ~WMMeasValuesBase();

public slots:
    virtual void onShowHide(bool shw);
    virtual void SetActualValuesSlot(cwmActValues * av);
    virtual void ActualizeLPSlot(cwmActValues * av);
    virtual void SetConfInfoSlot(cConfData * cd);
    virtual void ActualizeLoadPoint();
    bool onLoadSession(QString session);
    void onSaveSession(QString session);
    virtual void ReceiveFormatInfoSlot(int m, int m2, int n, cFormatInfo * fi );

signals:
    void isVisibleSignal(bool);
    void SendFormatInfoSignal(bool, int, int, int, cFormatInfo*);

protected:
    virtual void closeEvent(QCloseEvent *ce ) override;
    virtual void resizeEvent(QResizeEvent*) override;
    virtual void moveEvent(QMoveEvent*) override;
    virtual void contextMenuEvent( QContextMenuEvent * ) override;

private slots:
    void saveConfiguration();
    void onWriteStreamForGeomChange();
private:
    virtual void readStream(QDataStream& stream) override;
    virtual void writeStream(QDataStream& stream) override;
    virtual void setDefaults() override;
    void setInitialDefaults();
    void actualizeDisplay();
    void adjustBoxWidths();

    Ui::WMMeasValuesBase *ui;
    cwmActValues m_ActValues;
    cConfData m_ConfData;
    WMMeasConfigBase* m_pContextMenu;
    cFormatInfo m_Format[4];
    int m_nDisplayMode;
    int m_nLPDisplayMode;
    GeometryChangeHandler m_geomHandler;
    WidgetGeometry m_geomToFromStream;
    SessionStreamer m_sessionStreamer;
};

#endif // WMMEASVALUESBASE_H
