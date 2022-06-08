#ifndef WMMEASVALUESBASE_H
#define WMMEASVALUESBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlabel.h>
#include <QTimer>

#include "bigletter.h"
#include "confdata.h"
#include "widgeom.h"
#include "wmmeasconfigbase.h"
#include "formatinfo.h"
#include "wmactvalues.h"
#include "sessionreadwrite.h"
#include "sessionappendcustom.h"

namespace Ui {
    class WMMeasValuesBase;
}

class WMMeasValuesBase : public QDialog, public ISessionAppendCustomHandler
{
    Q_OBJECT

public:
    explicit WMMeasValuesBase(QWidget* parent, QString machineName);
    ~WMMeasValuesBase();

public slots:
    virtual void ShowHideMVSlot( bool b );
    virtual void SetActualValuesSlot( cwmActValues * av );
    virtual void ActualizeLPSlot( cwmActValues * av );
    virtual void SetConfInfoSlot( cConfData * cd );
    virtual void ActualizeLoadPoint();
    bool LoadSession( QString session );
    void SaveSession( QString session );
    virtual void ReceiveFormatInfoSlot(int m, int m2, int n, cFormatInfo * fi );

signals:
    void isVisibleSignal(bool);
    void SendFormatInfoSignal(bool, int, int, int, cFormatInfo*);

protected:
    virtual void closeEvent( QCloseEvent * ce ) override;
    virtual void resizeEvent( QResizeEvent * e ) override;
    virtual void moveEvent( QMoveEvent *) override;
    virtual void contextMenuEvent( QContextMenuEvent * ) override;

private:
    virtual void transferSessionCustom(QDataStream& stream, bool write) override;

    Ui::WMMeasValuesBase *ui;
    cWidgetGeometry m_widGeometry;
    cwmActValues m_ActValues;
    cConfData m_ConfData;
    WMMeasConfigBase* m_pContextMenu;
    cFormatInfo m_Format[4];
    int m_nDisplayMode;
    int m_nLPDisplayMode;

    QTimer m_Timer;
    SessionReadWrite m_sessionReadWrite;

    void init();
    void destroy();
    void ActualizeDisplay();
    void adjustBoxWidths();

private slots:
    void saveConfiguration();
};

#endif // WMMEASVALUESBASE_H
