#include "wmrawactualconfigbase.h"
#include "ui_wmrawactualconfigbase.h"
#include "common-modes.h"

WMRawActualConfigBase::WMRawActualConfigBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMRawActualConfigBase)
{
    ui->setupUi(this);
    init();
}

WMRawActualConfigBase::~WMRawActualConfigBase()
{
    destroy();
    delete ui;
}

void WMRawActualConfigBase::init()
{
    AmplDisplayMode = x1;
    AmplPrimSekMode=prim;
    WinkelDisplayMode = mathpos; 
}


void WMRawActualConfigBase::ReceiveDisplayConfSlot(bool dc, int m, int m2, int m3)
{
    m_dcMode = dc;
    AmplDisplayMode = m;
    WinkelDisplayMode = m2;
    AmplPrimSekMode = m3;
    actualizeDisplay();
}


void WMRawActualConfigBase::actualizeDisplay()
{
    bool isx1 = (AmplDisplayMode == x1);

    ui->Amplx1radioButton->setChecked(isx1);
    ui->Amplx1_sqrt2radioButton->setChecked(!isx1);

    bool ismathpos = (WinkelDisplayMode == mathpos);

    ui->MathradioButton->setChecked(ismathpos);
    ui->TechradioButton->setChecked(!ismathpos);

    if (m_dcMode)
    {
        ui->Amplx1_sqrt2radioButton->setEnabled(false);
        ui->MathradioButton->setEnabled(false);
        ui->TechradioButton->setEnabled(false);
    }
    else
    {
        ui->Amplx1_sqrt2radioButton->setEnabled(true);
        ui->MathradioButton->setEnabled(true);
        ui->TechradioButton->setEnabled(true);
    }

    bool isPrim = (AmplPrimSekMode == prim);

    ui->PrimradioButton->setChecked(isPrim);
    ui->SekradioButton->setChecked(!isPrim);
}



void WMRawActualConfigBase::amplx1checkedSlot()
{
    AmplDisplayMode = x1;
    actualizeDisplay();
}


void WMRawActualConfigBase::amplx1_sq2checkedSlot()
{
    AmplDisplayMode = x1_SQRT2;
    actualizeDisplay();
}


void WMRawActualConfigBase::winkelmathcheckedSlot()
{
    WinkelDisplayMode = mathpos;
    actualizeDisplay();
}


void WMRawActualConfigBase::winkeltechcheckedSlot()
{
    WinkelDisplayMode = techpos;
    actualizeDisplay();
}


void WMRawActualConfigBase::SendData2AppSlot()
{
    emit SendVektorDisplayFormat(AmplDisplayMode, WinkelDisplayMode, AmplPrimSekMode);
}


void WMRawActualConfigBase::primCheckedSlot()
{
    AmplPrimSekMode = prim;
    actualizeDisplay();
}


void WMRawActualConfigBase::sekCheckedSlot()
{
    AmplPrimSekMode = sek;
    actualizeDisplay();
}
