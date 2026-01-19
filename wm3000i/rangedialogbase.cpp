//Added by qt3to4:
#include <QShowEvent>
#include "rangedialogbase.h"
#include "ui_rangedialogbase.h"
#include "wmglobal.h"

RangeDialogBase::RangeDialogBase(QWidget* parent , QString machineName):
    wmRangeDialogBase   (parent, machineName),
    ui(new Ui::RangeDialogBase)
{
    ui->setupUi(this);
}

RangeDialogBase::~RangeDialogBase()
{
    delete ui;
}

void RangeDialogBase::ActualizeDialog() // aktualisiert dialoganzeige
{
    if ( m_ConfData.m_sRangeNVorgabe == "Auto" )
    ui->RangeNComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeNVorgabe));
    else
    ui->RangeNComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeN));
    if ( m_ConfData.m_sRangeXVorgabe == "Auto" )
    ui->RangeXComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeXVorgabe));
    else
    ui->RangeXComboBox->setCurrentItem(m_sNXItemList.findIndex(m_ConfData.m_sRangeX));
    if ( m_ConfData.m_sRangeETVorgabe == "Auto")
    ui->RangeECTComboBox->setCurrentItem(m_sECTItemList.findIndex(m_ConfData.m_sRangeETVorgabe));
    else
    ui->RangeECTComboBox->setCurrentItem(m_sECTItemList.findIndex(m_ConfData.m_sRangeET));
    switch (m_ConfData.m_nMeasMode) {
    case In_IxDiff:
    case In_IxAbs:
        ui->RangeXComboBox->setEnabled(true);
        ui->RangeECTComboBox->setEnabled(false);
        break;
    case In_ECT:
        ui->RangeXComboBox->setEnabled(false);
        ui->RangeECTComboBox->setEnabled(true);
        break;
    case In_nConvent:
        ui->RangeXComboBox->setEnabled(false);
        ui->RangeECTComboBox->setEnabled(false);
        break;
    }
}


void RangeDialogBase::SetRangeListSlot( cWMRangeList& nx,  cWMRangeList& ect)
{
    CWMRange *Range;
    Q3PtrListIterator<CWMRange> it(nx);

   while (!m_sNXItemList.isEmpty()) m_sNXItemList.pop_back(); // liste erst mal leeren
   for(Range=it.toFirst();Range;Range=++it) m_sNXItemList.append(Range->Name());
   ui->RangeNComboBox->clear();
   ui->RangeNComboBox->insertStringList(m_sNXItemList);
   ui->RangeXComboBox->clear();
   ui->RangeXComboBox->insertStringList(m_sNXItemList);

   it=ect;
   while (!m_sECTItemList.isEmpty()) m_sECTItemList.pop_back(); // liste erst mal leeren
   for(Range=it.toFirst();Range;Range=++it) m_sECTItemList.append(Range->Name());
   ui->RangeECTComboBox->clear();
   ui->RangeECTComboBox->insertStringList(m_sECTItemList);
   ActualizeDialog();
}


void RangeDialogBase::SetConfInfoSlot(cConfData *cd)
{
    m_ConfData=*cd;
    ActualizeDialog();
}


void RangeDialogBase::SetNRangeSlot( const QString & nr )
{
    m_ConfData.m_sRangeNVorgabe=nr;
    if ( nr != QString("Auto") ) m_ConfData.m_sRangeNSoll=nr; // damit wird sofort geschaltet
    emit SendRange(&m_ConfData);
}


void RangeDialogBase::SetXRangeSlot( const QString & xr)
{
    m_ConfData.m_sRangeXVorgabe=xr;
    if ( xr != QString("Auto") ) m_ConfData.m_sRangeXSoll=xr;
    emit SendRange(&m_ConfData);
}



void RangeDialogBase::SetECTRangeSlot( const QString & er)
{
    m_ConfData.m_sRangeETVorgabe=er;
    if ( er != QString("Auto") ) m_ConfData.m_sRangeETSoll=er;
    emit SendRange(&m_ConfData);
}


void RangeDialogBase::showEvent( QShowEvent * )
{
    wmRangeDialogBase::onLoadSession(".ses");
    emit SendRangeInfoReqSignal(); // aufforderung an device bereichliste und configuration zu senden
}

void RangeDialogBase::closeEvent(QCloseEvent *)
{
    wmRangeDialogBase::onSaveSession(".ses");
    emit sigIsVisible(false);
}

void RangeDialogBase::RemoteCtrlInfoSlot(bool remote)
{
    setEnabled(!remote);
}

void RangeDialogBase::onShowHide(bool shw)
{
    if (shw)
    {
        show();
    }
    else
    {
        hide();
    }
}



QDialog *RangeDialogBase::getChildThis()
{
    return this;
}

