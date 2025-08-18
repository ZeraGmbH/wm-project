#include "confdialogbase.h"
#include "ui_confdialogbase.h"
#include "wm3000i.h"
#include "maxperiods.h"

extern cWM3000I* g_WMDevice;


ConfDialogBase::ConfDialogBase( QWidget* parent , bool onlyRatio ):
    wmconfdialogbase(parent),
    ui(new Ui::ConfDialogBase),
    m_bOnlyRatio(onlyRatio)
{
    ui->setupUi(this);
    init();
}

ConfDialogBase::~ConfDialogBase()
{
    destroy();
    delete ui;
}

void ConfDialogBase::init()
{
    QRegExp rx( "^[0-9,a-f,A-F]{2,2}$" );
    QValidator* MacValidator = new QRegExpValidator( rx, this );

    ui->MacSLineEdit6->setInputMask("HH");
    ui->MacSLineEdit6->setValidator(MacValidator);
    ui->MacSLineEdit5->setInputMask("HH");
    ui->MacSLineEdit5->setValidator(MacValidator);
    ui->MacSLineEdit4->setInputMask("HH");
    ui->MacSLineEdit4->setValidator(MacValidator);
    ui->MacSLineEdit3->setInputMask("HH");
    ui->MacSLineEdit3->setValidator(MacValidator);
    ui->MacSLineEdit2->setInputMask("HH");
    ui->MacSLineEdit2->setValidator(MacValidator);
    ui->MacSLineEdit1->setInputMask("HH");
    ui->MacSLineEdit1->setValidator(MacValidator);

    ui->MacDLineEdit6->setInputMask("HH");
    ui->MacDLineEdit6->setValidator(MacValidator);
    ui->MacDLineEdit5->setInputMask("HH");
    ui->MacDLineEdit5->setValidator(MacValidator);
    ui->MacDLineEdit4->setInputMask("HH");
    ui->MacDLineEdit4->setValidator(MacValidator);
    ui->MacDLineEdit3->setInputMask("HH");
    ui->MacDLineEdit3->setValidator(MacValidator);
    ui->MacDLineEdit2->setInputMask("HH");
    ui->MacDLineEdit2->setValidator(MacValidator);
    ui->MacDLineEdit1->setInputMask("HH");
    ui->MacDLineEdit1->setValidator(MacValidator);

    QRegExp rx2( "^[0-7]{1,1}$" );
    QValidator* UPriorityValidator = new QRegExpValidator( rx2, this );
    ui->UPrioritylineEdit->setInputMask("N");
    ui->UPrioritylineEdit->setValidator(UPriorityValidator);

    QRegExp rx3( "^[0-1]{1,1}$" );
    QValidator* CFIValidator = new QRegExpValidator( rx3, this );
    ui->CFIlineEdit->setInputMask("N");
    ui->CFIlineEdit->setValidator(CFIValidator);

    QRegExp rx4( "^[0-9,a-f,A-F]{3,3}$" );
    QValidator* VIDValidator = new QRegExpValidator( rx4, this );
    ui->VIDlineEdit->setInputMask("HHH");
    ui->VIDlineEdit->setValidator(VIDValidator);

    QRegExp rx5( "^[0-9,a-f,A-F]{4,4}$" );
    QValidator* APPIDValidator = new QRegExpValidator( rx5, this );
    ui->APPIDlineEdit->setInputMask("HHHH");
    ui->APPIDlineEdit->setValidator(APPIDValidator);

    QRegExp rx6 ("^([0-9]+)(|\\.([0-9]+))(|mA|kA|A)$" );
    QValidator* RatioCurrentValidator = new QRegExpValidator( rx6, this );

    QRegExp rx7	("^([0-9]+)(|\\.([0-9]+))(|mV|kV|V)$" );
    QValidator* RatioVoltageValidator = new QRegExpValidator( rx7, this );

    ui->RatioPrimNLineEdit->setValidator(RatioCurrentValidator);
    ui->RatioSekNLineEdit->setValidator(RatioCurrentValidator);
    ui->RatioPrimXLineEdit->setValidator(RatioCurrentValidator);
    ui->RatioSekXLineEdit->setValidator(RatioCurrentValidator);
    ui->RatioPrimECTLineEdit->setValidator(RatioCurrentValidator);
    ui->RatioSekECTLineEdit->setValidator(RatioVoltageValidator);

    QRegExp rx8( "^[1-8]{1,1}$" );
    QValidator* ASDUSetValidator = new QRegExpValidator( rx8, this );
    ui->FirstASDUlineEdit->setInputMask("N");
    ui->FirstASDUlineEdit->setValidator(ASDUSetValidator);
    ui->LastASDUlineEdit->setInputMask("N");
    ui->LastASDUlineEdit->setValidator(ASDUSetValidator);
    ui->SetlineEdit->setInputMask("N");
    ui->SetlineEdit->setValidator(ASDUSetValidator);

    connect(ui->buttonOk,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->buttonCancel,SIGNAL(clicked()),this,SLOT(abortSlot()));
    connect(ui->Mode0RadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->Mode2RadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->Mode3RadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->DCRadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->ACRadioButton,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->CmpCorrCheckBox,SIGNAL(clicked()),this,SLOT(ApplyDataSlot()));
    connect(ui->S80RadioButton,SIGNAL(clicked()),this,SLOT(SxRadioButtonChecked()));
    connect(ui->S96RadioButton,SIGNAL(clicked()),this,SLOT(SxRadioButtonChecked()));
    connect(ui->S240RadioButton,SIGNAL(clicked()),this,SLOT(SxRadioButtonChecked()));
    connect(ui->S256RadioButton,SIGNAL(clicked()),this,SLOT(SxRadioButtonChecked()));
    connect(ui->S288RadioButton,SIGNAL(clicked()),this,SLOT(SxRadioButtonChecked()));
    connect(ui->F16RadioButton,SIGNAL(clicked()),this,SLOT(FxRadioButtonChecked()));
    connect(ui->F50RadioButton,SIGNAL(clicked()),this,SLOT(FxRadioButtonChecked()));
    connect(ui->F60RadioButton,SIGNAL(clicked()),this,SLOT(FxRadioButtonChecked()));

    m_bRemoteCtrl = false;

    if (!m_bOnlyRatio)
    {
        ui->RatioApplyPushButton->hide();
        if (g_WMDevice->isConventional())
        {
            ui->ConfTabWidget->removeTab(ui->ConfTabWidget->indexOf(ui->nConventTabPage));
            ui->ConfTabWidget->removeTab(ui->ConfTabWidget->indexOf(ui->SyncTabPage));
            ui->ModeButtonGroup->removeChild(ui->Mode3RadioButton);
            ui->ModeButtonGroup->removeChild(ui->Mode2RadioButton);
            ui->RatioTabPage->removeChild(ui->ECTratioGroupBox);
        }

        if (!g_WMDevice->isDC())
        {
            ui->ConfTabWidget->removeChild(ui->DCRadioButton);
            ui->ACRadioButton->setEnabled(false);
            ui->Widget9->removeChild(ui->OffsetCorrectionNCheckbox);
            ui->Widget9->removeChild(ui->OffsetCorrectionXCheckbox);
            ui->Widget9->removeChild(ui->CmpKorrGroupBox_3);
        }

        if (!g_WMDevice->isNewSamplerates())
        {
            ui->SPeriodeGroupBox->removeChild(ui->S96RadioButton);
            ui->SPeriodeGroupBox->removeChild(ui->S240RadioButton);
            ui->SPeriodeGroupBox->removeChild(ui->S288RadioButton);
        }
    }
    else
    {
        connect(ui->RatioApplyPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    }
}

void ConfDialogBase::SetConfInfoSlot(cConfData *cd )
{
    if ( true /*! isVisible()*/ ) {
    m_ConfDataTemp =  m_ConfData = *cd;
    SetModeMenu(); // modus menu einrichten
    SetCmpMenu(); // berechnung menu einrichten
    SetSyncMenu(); // sync menu einrichten
    SetRatioMenu(); // teiler menu einrichten
    SetLogMenu(); // logfile menu einrichten
    SetMessungMenu(); // messung menu einrichten
    SetnConventMenu(); // nconvent menu einrichten
    Actualize();
    }
}


void ConfDialogBase::SetConfListSlot( QStringList & NPItems, QStringList & NSItems)
{
    if (! isVisible() ) {
    ui->RatioNPrimComboBox->clear();
    ui->RatioNPrimComboBox->insertStringList(NPItems); // normalwandler primär stufen
    ui->RatioNPrimComboBox->setCurrentItem(NPItems.findIndex(m_ConfDataTemp.m_NPrimary));
    ui->RatioNSekComboBox->clear();
    ui->RatioNSekComboBox->insertStringList(NSItems); // normalwandler sekundär stufen
    ui->RatioNSekComboBox->setCurrentItem(NSItems.findIndex(m_ConfDataTemp.m_NSecondary));
    }
}


void ConfDialogBase::Actualize()
{
    switch (m_ConfDataTemp.m_nMeasMode) {
     case In_IxAbs:
     case In_IxDiff:
     case In_nConvent:
        ui->XTRatioGroupBox->setEnabled(true);
        ui->ECTratioGroupBox->setEnabled(false);
        if(m_bOnlyRatio)
        {
            ui->XTRatioGroupBox->show();
            ui->ECTratioGroupBox->hide();
        }
        break;
    case In_ECT:
        ui->XTRatioGroupBox->setEnabled(false);
        ui->ECTratioGroupBox->setEnabled(true);
        if(m_bOnlyRatio)
        {
            ui->XTRatioGroupBox->hide();
            ui->ECTratioGroupBox->show();
        }
        break;
/*	case In_nConvent:
        XTRatioGroupBox->setEnabled(false);
        ECTratioGroupBox->setEnabled(false);
    break; */
    }

    if (ui->RatioNPrimComboBox->count()==0  || m_bRemoteCtrl ) // es existiert keine eigenfehlertabelle
    { // bzw. wir sind unter rechner kontrolle
        ui->NTRatioGroupBox->setEnabled(true);
        ui->NTRatioGroupBox2->setEnabled(false);
        if (m_bOnlyRatio)
        {
            ui->NTRatioGroupBox2->hide();
            ui->NTRatioGroupBox->show();
        }
    }
    else
    {
        ui->NTRatioGroupBox2->setEnabled(true);
        ui->NTRatioGroupBox->setEnabled(false);
        if (m_bOnlyRatio)
        {
            ui->NTRatioGroupBox2->show();
            ui->NTRatioGroupBox->hide();
        }
    }
    resizeRatioTab();
}


void ConfDialogBase::accept()
{
    if( acceptRatio() && acceptEot())
    {
        ApplyDataSlot();
        m_ConfData = m_ConfDataTemp;
        emit SendConfDataSignal(&m_ConfData);
        mWmKeyBoard->hide();
        close();
    }
}

bool ConfDialogBase::acceptRatio()
{
    bool ratioInputOK = true;

    ratioInputOK = ratioInputOK &&  ui->RatioPrimNLineEdit->hasAcceptableInput()
                   && ui->RatioSekNLineEdit->hasAcceptableInput()
                   && ui->RatioPrimXLineEdit->hasAcceptableInput()
                   && ui->RatioSekXLineEdit->hasAcceptableInput()
                   && ui->RatioPrimECTLineEdit->hasAcceptableInput()
                   && ui->RatioSekECTLineEdit->hasAcceptableInput();

    if ( !ratioInputOK )
    {
        mWmMsgBox.msgRatioErr();
    }
    return ratioInputOK;
}

void ConfDialogBase::resizeRatioTab()
{
    if (m_bOnlyRatio){
        ui->RatioTabPage->resize({280,280});
        // The widget needs to be resized, as it gets to big after the hide/show settings where changed
    }
}

bool ConfDialogBase::acceptEot()
{
    bool rationOetOK(true);
    if (ui->RatioNPrimComboBox->isEnabled())
    {
        if (ui->RatioNPrimComboBox->currentIndex() == -1)
        {
            rationOetOK = false;
        }
    }
    if (ui->RatioNSekComboBox->isEnabled())
    {
        if (ui->RatioNSekComboBox->currentIndex() == -1)
        {
            rationOetOK = false;
        }
    }
    if ( !rationOetOK )
    {
        mWmMsgBox.msgRatioEotErr();
    }
    return rationOetOK;
}

void ConfDialogBase::abortSlot()
{
    m_ConfDataTemp = m_ConfData;
    SetConfInfoSlot(&m_ConfData);
    mWmKeyBoard->hide();
    close();
}


void ConfDialogBase::SetModeMenu()
{
    switch (m_ConfDataTemp.m_nMeasMode) {
    case In_IxAbs:
        ui->Mode0RadioButton->setChecked(true);
        break;
    case In_ECT:
        ui->Mode2RadioButton->setChecked(true);
        break;
    case In_nConvent:
        ui->Mode3RadioButton->setChecked(true);
        break;
    }

    ui->DCRadioButton->setChecked(m_ConfDataTemp.m_bDCmeasurement);
    ui->ACRadioButton->setChecked(!m_ConfDataTemp.m_bDCmeasurement);
}


void ConfDialogBase::SetCmpMenu()
{
    ui->CmpKorrLineEdit1->setText(QString( "%1" ).arg(m_ConfDataTemp.m_fxPhaseShift,8,'f',4));
    ui->CmpKorrLineEdit2->setText(QString( "%1" ).arg(m_ConfDataTemp.m_fxTimeShift,5,'f',3));
    ui->CmpCorrCheckBox->setChecked(m_ConfDataTemp.m_bOECorrection);
    ui->OffsetCorrectionNCheckbox->setChecked(m_ConfData.m_bOffsetCorrectionN);
    ui->OffsetCorrectionXCheckbox->setChecked(m_ConfData.m_bOffsetCorrectionX);
}


void ConfDialogBase::SetSyncMenu()
{
    switch (m_ConfDataTemp.m_nSyncSource) {
    case Extern:
        ui->SyncRadioButton1->setChecked(true);
        break;
    case Intern:
        ui->SyncRadioButton2->setChecked(true);
        break;
    }

    ui->TSyncSpinBox->setValue(m_ConfDataTemp.m_nTSync);
    ui->SSynccheckBox->setChecked(m_ConfDataTemp.m_bStrongEthSynchronisation);
}

void ConfDialogBase::clearUnitComboBoxes()
{
    ui->RatioPrimNUnitComboBox->clear();
    ui->RatioSekNUnitComboBox->clear();
    ui->RatioPrimXUnitComboBox->clear();
    ui->RatioSekXUnitComboBox->clear();
    ui->RatioPrimECTUnitComboBox->clear();
    ui->RatioSekECTUnitComboBox->clear();

    ui->RatioPrimNDivComboBox->clear();
    ui->RatioSekNDivComboBox->clear();
    ui->RatioPrimXDivComboBox->clear();
    ui->RatioSekXDivComboBox->clear();
    ui->RatioPrimECTDivComboBox->clear();
    ui->RatioSekECTDivComboBox->clear();
}

void ConfDialogBase::SetRatioMenu()
{
    clearUnitComboBoxes();

    ui->RatioPrimNUnitComboBox->insertStringList(mGuiHelper->GetList(ALIST));
    ui->RatioSekNUnitComboBox->insertStringList(mGuiHelper->GetList(ALIST));

    ui->RatioPrimNDivComboBox->insertStringList(mGuiHelper->GetList(DIVLIST));
    ui->RatioSekNDivComboBox->insertStringList(mGuiHelper->GetList(DIVLIST));

    // alle edit felder  und radiobuttons vorbesetzen
    ui->RatioPrimNLineEdit->setText(mGuiHelper->baseUnitTextCurrent(m_ConfDataTemp.m_NPrimary, ui->RatioPrimNUnitComboBox, ui->RatioPrimNDivComboBox));
    ui->RatioSekNLineEdit->setText(mGuiHelper->baseUnitTextCurrent(m_ConfDataTemp.m_NSecondary, ui->RatioSekNUnitComboBox, ui->RatioSekNDivComboBox));

    ui->RatioPrimXUnitComboBox->insertStringList(mGuiHelper->GetList(ALIST));
    ui->RatioSekXUnitComboBox->insertStringList(mGuiHelper->GetList(ALIST));

    ui->RatioPrimXDivComboBox->insertStringList(mGuiHelper->GetList(DIVLIST));
    ui->RatioSekXDivComboBox->insertStringList(mGuiHelper->GetList(DIVLIST));

    ui->RatioPrimXLineEdit->setText(mGuiHelper->baseUnitTextCurrent(m_ConfDataTemp.m_XPrimary, ui->RatioPrimXUnitComboBox, ui->RatioPrimXDivComboBox));
    ui->RatioSekXLineEdit->setText(mGuiHelper->baseUnitTextCurrent(m_ConfDataTemp.m_XSecondary, ui->RatioSekXUnitComboBox, ui->RatioSekXDivComboBox));

    ui->RatioPrimECTUnitComboBox->insertStringList(mGuiHelper->GetList(ALIST));
    ui->RatioSekECTUnitComboBox->insertStringList(mGuiHelper->GetList(VLIST));

    ui->RatioPrimECTDivComboBox->insertStringList(mGuiHelper->GetList(DIVLIST));
    ui->RatioSekECTDivComboBox->insertStringList(mGuiHelper->GetList(DIVLIST));

    ui->RatioPrimECTLineEdit->setText(mGuiHelper->baseUnitTextCurrent(m_ConfDataTemp.m_ETPrimary, ui->RatioPrimECTUnitComboBox, ui->RatioPrimECTDivComboBox));
    ui->RatioSekECTLineEdit->setText(mGuiHelper->baseUnitTextVoltage(m_ConfDataTemp.m_ETSecondary, ui->RatioSekECTUnitComboBox, ui->RatioSekECTDivComboBox));
}


void ConfDialogBase::SetLogMenu()
{
    ui->LogfileSizeSpinbox->setValue(m_ConfDataTemp.m_nLogFileMax>>10);
}


void ConfDialogBase::SuggestASDUs()
{
    m_ConfDataTemp.FirstASDU = 1;
    m_ConfDataTemp.LastASDU = 1;
    switch (m_ConfDataTemp.m_nSFreq)
    {
    case F16: // 16.67 hz sind bei nconventional nicht unterstützt
        break;

    case F50:
        switch (m_ConfDataTemp.m_nSRate)
        {
        case S80:
            break;
        case S96:
            m_ConfDataTemp.LastASDU = 2;
            break;
        case S256:
            m_ConfDataTemp.LastASDU = 8;
            break;
        case S288:
            m_ConfDataTemp.LastASDU = 6;
            break;
        }
        break;

    case F60:
        switch (m_ConfDataTemp.m_nSRate)
        {
        case S80:
        case S96:
            break;
        case S240:
            m_ConfDataTemp.LastASDU = 6;
            break;
        case S256:
            m_ConfDataTemp.LastASDU = 8;
            break;
        }
        break;
    }
}

void ConfDialogBase::ApplyDataSlot() // einstellungen werden intern übernommen, die menus aktualisiert
{
    switch (m_ConfDataTemp.m_nMeasMode) {
    case In_IxDiff:
    case In_IxAbs:
    case In_nConvent:
        m_ConfDataTemp.m_XPrimary = mGuiHelper->genRatioText( ui->RatioPrimXLineEdit->text(), ui->RatioPrimXUnitComboBox, ui->RatioPrimXDivComboBox);
        m_ConfDataTemp.m_XSecondary = mGuiHelper->genRatioText( ui->RatioSekXLineEdit->text(), ui->RatioSekXUnitComboBox, ui->RatioSekXDivComboBox);
        break;
    case In_ECT:
        m_ConfDataTemp.m_ETPrimary = mGuiHelper->genRatioText( ui->RatioPrimECTLineEdit->text(), ui->RatioPrimECTUnitComboBox, ui->RatioPrimECTDivComboBox);
        m_ConfDataTemp.m_ETSecondary = mGuiHelper->genRatioText( ui->RatioSekECTLineEdit->text(), ui->RatioSekECTUnitComboBox, ui->RatioSekECTDivComboBox);
        break;
    }

    if (ui->Mode2RadioButton->isChecked()) m_ConfDataTemp.m_nMeasMode=In_ECT;
    if (ui->Mode3RadioButton->isChecked()) m_ConfDataTemp.m_nMeasMode=In_nConvent;
    if (ui->Mode0RadioButton->isChecked()) m_ConfDataTemp.m_nMeasMode=In_IxAbs;
    if (ui->DCRadioButton->isChecked())
        m_ConfDataTemp.m_bDCmeasurement = true;
    if (ui->ACRadioButton->isChecked())
        m_ConfDataTemp.m_bDCmeasurement = false;
    // ModeMenu gescannt

    m_ConfDataTemp.m_fxPhaseShift=(ui->CmpKorrLineEdit1->text()).toDouble();
    m_ConfDataTemp.m_fxTimeShift=(ui->CmpKorrLineEdit2->text()).toDouble();
    m_ConfDataTemp.m_bOECorrection=ui->CmpCorrCheckBox->isChecked();
    m_ConfDataTemp.m_bOffsetCorrectionN=ui->OffsetCorrectionNCheckbox->isChecked();
    m_ConfDataTemp.m_bOffsetCorrectionX=ui->OffsetCorrectionXCheckbox->isChecked();
    // CmpMenu gescannt

    if (ui->SyncRadioButton1->isChecked()) m_ConfDataTemp.m_nSyncSource=Extern;
    if (ui->SyncRadioButton2->isChecked()) m_ConfDataTemp.m_nSyncSource=Intern;
    m_ConfDataTemp.m_nTSync=ui->TSyncSpinBox->value();
    m_ConfDataTemp.m_bStrongEthSynchronisation=ui->SSynccheckBox->isChecked();
    // SyncMenu gescannt

    m_ConfDataTemp.m_nLogFileMax=(ui->LogfileSizeSpinbox->value())<<10;
    // LogMenu gescannt

     if (ui->RatioNPrimComboBox->count()==0)   // es existiert keine eigenfehlertabelle, bzw. korrektur aus
    {
        m_ConfDataTemp.m_NPrimary = mGuiHelper->genRatioText( ui->RatioPrimNLineEdit->text(), ui->RatioPrimNUnitComboBox, ui->RatioPrimNDivComboBox);
        m_ConfDataTemp.m_NSecondary = mGuiHelper->genRatioText( ui->RatioSekNLineEdit->text(), ui->RatioSekNUnitComboBox, ui->RatioSekNDivComboBox);
    }
    else
    {
        m_ConfDataTemp.m_NPrimary = ui->RatioNPrimComboBox->currentText();
        m_ConfDataTemp.m_NSecondary = ui->RatioNSekComboBox->currentText();
    }

    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[5]=ui->MacSLineEdit6->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[4]=ui->MacSLineEdit5->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[3]=ui->MacSLineEdit4->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[2]=ui->MacSLineEdit3->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[1]=ui->MacSLineEdit2->text().toUShort(0,16);
    m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[0]=ui->MacSLineEdit1->text().toUShort(0,16);

    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[5]=ui->MacDLineEdit6->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[4]=ui->MacDLineEdit5->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[3]=ui->MacDLineEdit4->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[2]=ui->MacDLineEdit3->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[1]=ui->MacDLineEdit2->text().toUShort(0,16);
    m_ConfDataTemp.m_MacDestAdr.MacAdrByte[0]=ui->MacDLineEdit1->text().toUShort(0,16);

    m_ConfDataTemp.FirstASDU = ui->FirstASDUlineEdit->text().toUShort(0,10);
    m_ConfDataTemp.LastASDU = ui->LastASDUlineEdit->text().toUShort(0,10);
    m_ConfDataTemp.DataSet = ui->SetlineEdit->text().toUShort(0,10);

    m_ConfDataTemp.m_nPriorityTagged = (ui->TPIDlineEdit->text().toUShort(0,16) << 16);
    m_ConfDataTemp.m_nPriorityTagged +=(ui->UPrioritylineEdit->text().toUShort(0,10) << 13);
    m_ConfDataTemp.m_nPriorityTagged +=(ui->CFIlineEdit->text().toUShort(0,10) << 12);
    m_ConfDataTemp.m_nPriorityTagged +=(ui->VIDlineEdit->text().toUShort(0,16));

    m_ConfDataTemp.m_nEthTypeHeader = (ui->EthTypelineEdit->text().toUShort(0,16) << 16);
    m_ConfDataTemp.m_nEthTypeHeader += ui->APPIDlineEdit->text().toUShort(0,16);

    // nConventMenu gescannt

    if (ui->F16RadioButton->isChecked()) m_ConfDataTemp.m_nSFreq=F16;
    if (ui->F50RadioButton->isChecked()) m_ConfDataTemp.m_nSFreq=F50;
    if (ui->F60RadioButton->isChecked()) m_ConfDataTemp.m_nSFreq=F60;

    float f(50.0);
    switch (m_ConfDataTemp.m_nSFreq)
    {  // wir setzen den realen frequenzwert
      case F16: f = 50.0/3;break;
      case F50: f = 50.0;break;
      case F60: f = 60.0;
    }

    m_ConfDataTemp.m_fSFreq = f;

    if (ui->S80RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S80;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS80MeasPeriod);
        if (ui->CmpIntervallSpinBox->value() > nmaxS80MeasPeriod)
            ui->CmpIntervallSpinBox->setValue(nmaxS80MeasPeriod);
    }

    if (ui->S96RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S96;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS96MeasPeriod);
        if (ui->CmpIntervallSpinBox->value() > nmaxS96MeasPeriod)
            ui->CmpIntervallSpinBox->setValue(nmaxS96MeasPeriod);
    }

    if (ui->S240RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S240;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS240MeasPeriod);
        if (ui->CmpIntervallSpinBox->value() > nmaxS240MeasPeriod)
            ui->CmpIntervallSpinBox->setValue(nmaxS240MeasPeriod);
    }

    if (ui->S256RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S256;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS256MeasPeriod);
        if (ui->CmpIntervallSpinBox->value() > nmaxS256MeasPeriod)
            ui->CmpIntervallSpinBox->setValue(nmaxS256MeasPeriod);
    }

    if (ui->S288RadioButton->isChecked())
    {
        m_ConfDataTemp.m_nSRate=S288;
        ui->CmpIntervallSpinBox->setMaxValue(nmaxS288MeasPeriod);
        if (ui->CmpIntervallSpinBox->value() > nmaxS288MeasPeriod)
            ui->CmpIntervallSpinBox->setValue(nmaxS288MeasPeriod);
    }

    m_ConfDataTemp.m_nMeasPeriod=ui->CmpIntervallSpinBox->value();


    m_ConfDataTemp.m_nIntegrationTime=ui->TIntegrationSpinBox->value();
    //  MessungMenu gescannt

    Actualize();
}

void ConfDialogBase::SetnConventMenu()
{
    ui->MacSLineEdit6->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit5->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit4->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[3],2,16).replace(' ','0').upper());
    ui->MacSLineEdit3->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit2->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    ui->MacSLineEdit1->setText( QString("%1").arg(m_ConfDataTemp.m_MacSourceAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );

    ui->MacDLineEdit6->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[5],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit5->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[4],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit4->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[3],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit3->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[2],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit2->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[1],2,16).replace(' ','0').upper() );
    ui->MacDLineEdit1->setText( QString("%1").arg(m_ConfDataTemp.m_MacDestAdr.MacAdrByte[0],2,16).replace(' ','0').upper() );

    ui->FirstASDUlineEdit->setText(QString("%1").arg(m_ConfDataTemp.FirstASDU) );
    ui->LastASDUlineEdit->setText(QString("%1").arg(m_ConfDataTemp.LastASDU) );
    ui->SetlineEdit->setText(QString("%1").arg(m_ConfDataTemp.DataSet) );

    ui->TPIDlineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nPriorityTagged >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    ui->UPrioritylineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nPriorityTagged >> 13) & 7,1,10));
    ui->CFIlineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nPriorityTagged >> 12) & 1,1,10));
    ui->VIDlineEdit->setText(QString("%1").arg(m_ConfDataTemp.m_nPriorityTagged & 0xFFF,3,16).replace(' ','0').upper() );

    ui->EthTypelineEdit->setText(QString("%1").arg((m_ConfDataTemp.m_nEthTypeHeader >> 16) & 0xFFFF,4,16).replace(' ','0').upper() );
    ui->APPIDlineEdit->setText(QString("%1").arg(m_ConfDataTemp.m_nEthTypeHeader & 0xFFFF,4,16).replace(' ','0').upper() );
}


void ConfDialogBase::SetMessungMenu()
{
    bool buttonOK;
    QList<QRadioButton*> buttonList;
    QList<int> measperiodList;

    buttonList << ui->S80RadioButton << ui->S96RadioButton
               << ui->S240RadioButton << ui->S256RadioButton
               << ui->S288RadioButton;
    measperiodList << nmaxS80MeasPeriod << nmaxS96MeasPeriod
                   << nmaxS240MeasPeriod << nmaxS256MeasPeriod
                   << nmaxS288MeasPeriod;


    // in abhängigkeit der gewählten frequenz enablen bzw. disablen wir die sample rate radiobuttons
    switch (m_ConfDataTemp.m_nSFreq) {
    case F16:
        ui->F16RadioButton->setChecked(true);

        ui->S80RadioButton->setEnabled(true);
        ui->S96RadioButton->setEnabled(false);
        ui->S240RadioButton->setEnabled(false);
        ui->S256RadioButton->setEnabled(true);
        ui->S288RadioButton->setEnabled(false);
        break;
    case F50:
        ui->F50RadioButton->setChecked(true);

        ui->S80RadioButton->setEnabled(true);
        ui->S96RadioButton->setEnabled(true);
        ui->S240RadioButton->setEnabled(false);
        ui->S256RadioButton->setEnabled(true);
        ui->S288RadioButton->setEnabled(true);
        break;
    case F60:
        ui->F60RadioButton->setChecked(true);

        ui->S80RadioButton->setEnabled(true);
        ui->S96RadioButton->setEnabled(true);
        ui->S240RadioButton->setEnabled(true);
        ui->S256RadioButton->setEnabled(true);
        ui->S288RadioButton->setEnabled(false);
        break;
    }

    buttonOK = false;
    // wir schauen nach ob die derzeit gesetzte samplerate bei der aktuell gewählten frequenz erlaubt ist
    for (int Sx = S80; Sx < MaxSRate; Sx++)
    {
        if (m_ConfDataTemp.m_nSRate == Sx)
        {
            if (buttonList.at(Sx)->isEnabled())
            {
                buttonList.at(Sx)->setChecked(true);
                buttonOK = true;
                break;
            }
        }
    }

    // wenn die aktuell gesetzte samplerate nicht erlaubt ist, setzten wir die 1. aus der liste die erlaubt ist
    if (!buttonOK)
    {
        for (int Sx = S80; Sx < MaxSRate; Sx++)
            if (buttonList.at(Sx)->isEnabled())
            {
                buttonList.at(Sx)->setChecked(true);
                break;
            }
    }

    // bei 16.67 hz max. 16 perioden .... 1sec
    if (m_ConfDataTemp.m_nSFreq == F16)
    {
        ui->CmpIntervallSpinBox->setMaxValue(16);
        ui->CmpIntervallSpinBox->setValue(m_ConfDataTemp.m_nMeasPeriod > 16 ? 16 : m_ConfDataTemp.m_nMeasPeriod);
    }
    else
    // ansonsten -> in abhängigkeit der samplerate setzen wir das maximum und ev. den wert messperioden anzahl
    for (int Sx = S80; Sx < MaxSRate; Sx++)
    {
        if (buttonList.at(Sx)->isChecked())
        {
            ui->CmpIntervallSpinBox->setMaxValue(measperiodList.at(Sx));
            ui->CmpIntervallSpinBox->setValue(m_ConfDataTemp.m_nMeasPeriod > measperiodList.at(Sx) ? measperiodList.at(Sx) : m_ConfDataTemp.m_nMeasPeriod);
            break;
        }
    }

    ui->TIntegrationSpinBox->setValue(m_ConfDataTemp.m_nIntegrationTime);
}


void ConfDialogBase::RemoteCtrlInfoSlot(bool remote)
{
    m_bRemoteCtrl = remote;
    ui->buttonOk->setEnabled(!remote);
    if (remote)
        ui->RatioTabPage->hide();
    Actualize();
}


void ConfDialogBase::takeScreenshoots()
{
    mScrShooter->showed(false);
    {
        showRatio();
        this->move(0,0);
        mScrShooter->showed(true);
    }
    mScrShooter->useTimer(ui->RatioTabPage, 42);
}

void ConfDialogBase::takeScreenshootFinished()
{
    if(mScrShooter->showed())
    {
        ui->RatioTabPage->hide();
    }
}

void ConfDialogBase::screenshooterTriggered()
{
    mScrShooter->storeScreen("2_Alles");
    this->show();
    mScrShooter->setTabWidgetPoi(this, ui->ConfTabWidget);
}

void ConfDialogBase::keyboardScreenshorTriggerd()
{
    mScrShooter->setKeyBoardWidgetPoi(mWmKeyBoard);
}

void ConfDialogBase::showRatio()
{
    ui->RatioTabPage->setParent(0);
    ui->RatioTabPage->setWindowTitle("Transformer Ratio");
    ui->RatioTabPage->show();
}

void ConfDialogBase::FxRadioButtonChecked()
{
    ApplyDataSlot(); // wir übernehmen die neue frequenz
    SetMessungMenu(); // und wir passen das messung menü an für den fall dass sich die frequenz geändert hat
    SuggestASDUs();
    SetnConventMenu();
}


void ConfDialogBase::SxRadioButtonChecked()
{
    ApplyDataSlot(); // wir übernehmen die neue sample raten
    SuggestASDUs();
    SetnConventMenu();
}

void ConfDialogBase::setKeyboard(wmKeyboardForm *keyboard)
{
    mWmKeyBoard = keyboard;
    ui->APPIDlineEdit->setKeyboard(keyboard);
 //   ui->TPIDlineEdit->setKeyboard(keyboard);
    ui->VIDlineEdit->setKeyboard(keyboard);
 //   ui->EthTypelineEdit->setKeyboard(keyboard);

    ui->MacSLineEdit1->setKeyboard(keyboard);
    ui->MacSLineEdit2->setKeyboard(keyboard);
    ui->MacSLineEdit3->setKeyboard(keyboard);
    ui->MacSLineEdit4->setKeyboard(keyboard);
    ui->MacSLineEdit5->setKeyboard(keyboard);
    ui->MacSLineEdit6->setKeyboard(keyboard);

    ui->MacDLineEdit1->setKeyboard(keyboard);
    ui->MacDLineEdit2->setKeyboard(keyboard);
    ui->MacDLineEdit3->setKeyboard(keyboard);
    ui->MacDLineEdit4->setKeyboard(keyboard);
    ui->MacDLineEdit5->setKeyboard(keyboard);
    ui->MacDLineEdit6->setKeyboard(keyboard);

    ui->UPrioritylineEdit->setKeyboard(keyboard);
    ui->UPrioritylineEdit->setInputMode(NUMINPUT);
    ui->CFIlineEdit->setKeyboard(keyboard);
    ui->CFIlineEdit->setInputMode(NUMINPUT);
    ui->FirstASDUlineEdit->setKeyboard(keyboard);
    ui->FirstASDUlineEdit->setInputMode(NUMINPUT);
    ui->LastASDUlineEdit->setKeyboard(keyboard);
    ui->LastASDUlineEdit->setInputMode(NUMINPUT);
    ui->SetlineEdit->setKeyboard(keyboard);
    ui->SetlineEdit->setInputMode(NUMINPUT);

    ui->CmpKorrLineEdit1->setKeyboard(keyboard);
    ui->CmpKorrLineEdit1->setInputMode(FLOATINPUT);
    ui->CmpKorrLineEdit2->setKeyboard(keyboard);
    ui->CmpKorrLineEdit2->setInputMode(FLOATINPUT);

    ui->RatioPrimNLineEdit->setKeyboard(keyboard);
    ui->RatioPrimNLineEdit->setInputMode(FLOATINPUT);
    ui->RatioSekNLineEdit->setKeyboard(keyboard);
    ui->RatioSekNLineEdit->setInputMode(FLOATINPUT);
    ui->RatioPrimXLineEdit->setKeyboard(keyboard);
    ui->RatioPrimXLineEdit->setInputMode(FLOATINPUT);
    ui->RatioSekXLineEdit->setKeyboard(keyboard);
    ui->RatioSekXLineEdit->setInputMode(FLOATINPUT);
    ui->RatioSekECTLineEdit->setKeyboard(keyboard);
    ui->RatioSekECTLineEdit->setInputMode(FLOATINPUT);
    ui->RatioPrimECTLineEdit->setKeyboard(keyboard);
    ui->RatioPrimECTLineEdit->setInputMode(FLOATINPUT);
}

void ConfDialogBase::on_ConfTabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    mWmKeyBoard->hide();
}
