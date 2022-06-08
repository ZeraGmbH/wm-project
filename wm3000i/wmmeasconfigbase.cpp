#include "wmmeasconfigbase.h"
#include "ui_wmmeasconfigbase.h"
#include "errorunits.h"
#include "currentunits.h"
#include "loadpointunits.h"
#include "common-modes.h"
#include "angleunits.h"

WMMeasConfigBase::WMMeasConfigBase(QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMMeasConfigBase)
{
    ui->setupUi(this);
    init();
}

WMMeasConfigBase::~WMMeasConfigBase()
{
    destroy();
    delete ui;
}

void WMMeasConfigBase::init()
{
    m_lpUnitList.append(LoadpointUnit + LPProzent);
    m_lpUnitList.append(CurrentUnit + CurrA);
    m_lpUnitList.append(CurrentUnit + CurrkA);
    for(auto unit : m_lpUnitList) {
        ui->LPcomboBox->insertItem(unit->Name);
    }

    m_errUnitList.append(ErrorUnit + ErrProzent);
    m_errUnitList.append(ErrorUnit + Errppm);
    for(auto unit : m_errUnitList) {
        ui->ErrcomboBox->insertItem(unit->Name);
    }

    m_angleUnit.append(AngleUnit + Anglegrad);
    m_angleUnit.append(AngleUnit + Anglemin);
    m_angleUnit.append(AngleUnit + Anglecrad);
    m_angleUnit.append(AngleUnit + Anglemrad);
    m_angleUnit.append(AngleUnit + Angleurad);
    m_angleUnit.append(AngleUnit + Anglerad);
    for(auto unit : m_errUnitList) {
        ui->WcomboBox->insertItem(unit->Name);
    }

    m_nDisplayMode = IEC; // wmglobal
    m_nLPDisplayMode = totalRms;
}


void WMMeasConfigBase::ReceiveFormatInfoSlot(bool dc, int m, int m2, int n, cFormatInfo* fi )
{
    int i;
    for(i = 0; i < n; i++, fi++)
        m_Format[i] = *fi;

    m_nDisplayMode = m;
    m_nLPDisplayMode = m2;
    m_dcMode = dc;

    ui->LPspinBoxFW->setValue(m_Format[0].FieldWidth);
    ui->LPspinBoxRS->setValue(m_Format[0].Resolution);
    ui->LPcomboBox->setCurrentText(m_Format[0].UnitInfo.Name);

    ui->ErrspinBoxFW->setValue(m_Format[1].FieldWidth);
    ui->ErrspinBoxRS->setValue(m_Format[1].Resolution);
    ui->ErrcomboBox->setCurrentText(m_Format[1].UnitInfo.Name);

    ui->WspinBoxFW->setValue(m_Format[2].FieldWidth);
    ui->WspinBoxRS->setValue(m_Format[2].Resolution);
    ui->WcomboBox->setCurrentText(m_Format[2].UnitInfo.Name);

    ui->RCFspinBoxFW->setValue(m_Format[3].FieldWidth);
    ui->RCFspinBoxRS->setValue(m_Format[3].Resolution);

    Actualize();
}



void WMMeasConfigBase::LPComboActivatedSlot(int index)
{
    m_Format[0].UnitInfo = *m_lpUnitList[index];
}


void WMMeasConfigBase::ErrComboActivatedSlot(int index)
{
    m_Format[1].UnitInfo = *m_errUnitList[index];
}


void WMMeasConfigBase::WComboActivatedSlot(int index)
{
    m_Format[2].UnitInfo = *m_angleUnit[index];
}


void WMMeasConfigBase::Actualize()
{
    bool isIEC = (m_nDisplayMode == IEC);

    ui->WgroupBox->setEnabled(isIEC); // winkel combox nur wenn iec anzeige

    ui->IECradioButton->setChecked(isIEC);
    ui->ANSIradioButton->setChecked(!isIEC);

    bool isTotalRMS = (m_nLPDisplayMode == totalRms);

    ui->TotalRMSradioButton->setChecked(isTotalRMS);
    ui->H1radioButton->setChecked(!isTotalRMS);

    if (m_dcMode)
    {
        ui->ANSIradioButton->setEnabled(false);
        ui->H1radioButton->setEnabled(false);
        ui->WgroupBox->setEnabled(false);
    }
    else
    {
        ui->ANSIradioButton->setEnabled(true);
        ui->H1radioButton->setEnabled(true);
    }
}


void WMMeasConfigBase::IECCheckedSlot()
{
    m_nDisplayMode = IEC;
    Actualize();
}


void WMMeasConfigBase::ANSICheckedSlot()
{
    m_nDisplayMode = ANSI;
    Actualize();
}


void WMMeasConfigBase::TotalRMScheckedSlot()
{
    m_nLPDisplayMode = totalRms;
    Actualize();
}


void WMMeasConfigBase::H1RMScheckedSlot()
{
    m_nLPDisplayMode = H1Rms;
    Actualize();
}


void WMMeasConfigBase::SendData2AppSlot()
{
    m_Format[0].FieldWidth = ui->LPspinBoxFW->value();
    m_Format[0].Resolution = ui->LPspinBoxRS->value();

    m_Format[1].FieldWidth = ui->ErrspinBoxFW->value();
    m_Format[1].Resolution = ui->ErrspinBoxRS->value();

    m_Format[2].FieldWidth = ui->WspinBoxFW->value();
    m_Format[2].Resolution = ui->WspinBoxRS->value();

    m_Format[3].FieldWidth = ui->RCFspinBoxFW->value();
    m_Format[3].Resolution = ui->RCFspinBoxRS->value();

    emit SendFormatInfoSignal(m_nDisplayMode, m_nLPDisplayMode, 4, m_Format);
}

