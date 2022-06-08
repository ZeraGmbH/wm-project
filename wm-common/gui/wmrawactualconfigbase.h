/****************************************************************************
** Form interface generated from reading ui file 'wmrawactualconfigbase.ui'
**
** Created: Mi Sep 19 12:39:28 2012
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WMRAWACTUALCONFIGBASE_H
#define WMRAWACTUALCONFIGBASE_H

#include <qvariant.h>
#include <qdialog.h>


namespace Ui {
    class WMRawActualConfigBase;
}

class WMRawActualConfigBase : public QDialog
{
    Q_OBJECT

public:
    explicit WMRawActualConfigBase( QWidget* parent = 0);
    ~WMRawActualConfigBase();

public slots:
    virtual void ReceiveDisplayConfSlot(bool dc, int m, int m2, int m3 );

signals:
    void SendVektorDisplayFormat(int,int,int);

private:
    Ui::WMRawActualConfigBase *ui;
    bool m_dcMode;
    int AmplDisplayMode;
    int WinkelDisplayMode;
    int AmplPrimSekMode;

    virtual void init();
    virtual void ActualizeDisplay();

private slots:
    virtual void amplx1checkedSlot();
    virtual void amplx1_sq2checkedSlot();
    virtual void winkelmathcheckedSlot();
    virtual void winkeltechcheckedSlot();
    virtual void SendData2AppSlot();
    virtual void primCheckedSlot();
    virtual void sekCheckedSlot();

};

#endif // WMRAWACTUALCONFIGBASE_H
