#ifndef WMMEASCONFIGBASE_H
#define WMMEASCONFIGBASE_H

#include "formatinfo.h"
#include <QDialog>
#include <QList>

namespace Ui {
    class WMMeasConfigBase;
}

class WMMeasConfigBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMMeasConfigBase(QWidget* parent = 0);
    ~WMMeasConfigBase();

public slots:
    virtual void ReceiveFormatInfoSlot(bool dc, int m, int m2, int n, cFormatInfo * fi );
    virtual void TotalRMScheckedSlot();
    virtual void H1RMScheckedSlot();

signals:
    void SendFormatInfoSignal(int, int, int, cFormatInfo*);

private:
    Ui::WMMeasConfigBase *ui;
    QList<eUnit*> m_lpUnitList;
    eUnit* m_ErrUnit[2];
    eUnit* m_AngleUnit[6];
    cFormatInfo m_Format[4];
    int m_nDisplayMode;
    int m_nLPDisplayMode;
    bool m_dcMode;

    void init();
    virtual void Actualize();

private slots:
    virtual void LPComboActivatedSlot( int index );
    virtual void ErrComboActivatedSlot( int index );
    virtual void WComboActivatedSlot( int index );
    virtual void IECCheckedSlot();
    virtual void ANSICheckedSlot();
    virtual void SendData2AppSlot();

};

#endif // WMMEASCONFIGBASE_H
