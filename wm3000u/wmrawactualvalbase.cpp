#include "wmrawactualvalbase.h"
#include "qdebug.h"
#include "ui_wmrawactualvalbase.h"
#include "wmglobal.h"
#include "common-modes.h"
#include <tools.h>
#include <geometrytowidget.h>
#include <QCloseEvent>
#include <QFileInfo>

const double PI2 = 6.283185307;

WMRawActualValBase::WMRawActualValBase(QWidget* parent, QString machineName):
    QDialog(parent),
    ui(new Ui::WMRawActualValBase),
    m_sessionStreamer(machineName, this)
{
    ui->setupUi(this);
    ui->XnAmplDisp -> setText( QString("%1 V").arg(0.0,10,'f',5) );
    ui->XnPhaseDisp -> setText( QString("%1 %2").arg(0.0,8,'f',4).arg( trUtf8("°")) );
    ui->XxAmplDisp -> setText( QString("%1 V").arg(0.0,10,'f',5) );
    ui->XxPhaseDisp -> setText( QString("%1 %2").arg(0.0,8,'f',4).arg( trUtf8("°")) );

    setInitialDefaults();
    m_pContextMenu = new WMRawActualConfigBase(this);
    connect(this,SIGNAL(SendVektorDispFormat(bool,int,int,int)),m_pContextMenu,SLOT(ReceiveDisplayConfSlot(bool,int,int,int)));
    connect(m_pContextMenu,SIGNAL(SendVektorDisplayFormat(int,int,int)),this,SLOT(ReceiveVektorDispFormat(int,int,int)));
    connect(&m_settingsChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(1000);
}

WMRawActualValBase::~WMRawActualValBase()
{
    delete ui;
}

void WMRawActualValBase::setScreenShooter(screenshooter *poi)
{
    mScrShooter = poi;
}

void WMRawActualValBase::closeEvent(QCloseEvent* ce)
{
    m_settingsChangeTimer.startDelayed();
    ce->accept();
}

void WMRawActualValBase::resizeEvent(QResizeEvent *)
{
    actFontSize();
    m_settingsChangeTimer.startDelayed();
}

void WMRawActualValBase::moveEvent(QMoveEvent *)
{
    m_settingsChangeTimer.startDelayed();
}

void WMRawActualValBase::onShowHide(bool shw)
{
    m_settingsChangeTimer.startDelayed();
    if (shw)
        show();
    else
        close();
}

void WMRawActualValBase::ReceiveAVDataSlot( cwmActValues *ActValues )
{
    m_ActValues = *ActValues;
    // hier wird später die Anzeige bedient
    if (isVisible()) {
        double radgrad = 57.295779; // 360/(2*PI) winkel sind im bogenmass
        double ampl;
        if (PrimSekDispMode == prim) {
            if (m_pConfData->m_bDCmeasurement)
                ampl = m_ActValues.VekN.re();
            else
                ampl = fabs(m_ActValues.VekN);
        }
        else {
            if (m_pConfData->m_bDCmeasurement)
                ampl = m_ActValues.VekNSek.re();
            else
                ampl = fabs(m_ActValues.VekNSek);
        }
        if (AmplDispMode == x1_SQRT2 && !m_pConfData->m_bDCmeasurement)
            ampl/=1.414213562;
        ui->XnAmplDisp -> setText( QString("%1 V").arg(ampl,10,'f',5) );

        double phi = m_ActValues.PHIN; // winkel sind zwischen 0 .. 2PI
        if (WinkelDispMode == techpos)
            phi = PI2 - phi;
        phi = normWinkelrad_PIPI(phi);
        phi *= radgrad;
        ui->XnPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );

        // amplitude der grundschwingung
        if (PrimSekDispMode == prim) {
            if (m_pConfData->m_bDCmeasurement)
                ampl = m_ActValues.VekX.re();
            else
                ampl = fabs(m_ActValues.VekX);
        }
        else {
            if (m_pConfData->m_bDCmeasurement)
                ampl = m_ActValues.VekXSek.re();
            else
                ampl = fabs(m_ActValues.VekXSek);
        }

        if (AmplDispMode == x1_SQRT2 && !m_pConfData->m_bDCmeasurement)
            ampl/=1.414213562;

        ui->XxAmplDisp -> setText( QString("%1 V").arg(ampl,10,'f',5) );

        phi = m_ActValues.PHIX;
        if (WinkelDispMode == techpos)
            phi = PI2 - phi;
        phi = normWinkelrad_PIPI(phi);
        phi *= radgrad;

        ui->XxPhaseDisp -> setText( QString("%1 %2").arg(phi,8,'f',4).arg( trUtf8("°")) );
        ui->FreqDisp -> setText( QString("%1 Hz").arg(ActValues->Frequenz,9,'f',5) );
        setSettingslabel();
    }
}

void WMRawActualValBase::SetConfInfoSlot(cConfData * cd)
{
    m_pConfData = cd;
    if (m_pConfData->m_bDCmeasurement) {
        ui->XnPhaseDisp->setVisible(false);
        ui->XxPhaseDisp->setVisible(false);
        ui->FreqDisp->setVisible(false);
        ui->FreqLabel->setVisible(false);
        AmplDispMode = x1;  // im fall von dc messung lassen wir nur x1 zu !!!
    }
    else {
        ui->XnPhaseDisp->setVisible(true);
        ui->XxPhaseDisp->setVisible(true);
        ui->FreqDisp->setVisible(true);
        ui->FreqLabel->setVisible(true);
    }
    m_settingsChangeTimer.startDelayed();
    resize(ui->gridLayout->minimumSize());
}


bool WMRawActualValBase::onLoadSession(QString session)
{
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}

void WMRawActualValBase::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
}

void WMRawActualValBase::activateContextMenu()
{
    emit SendVektorDispFormat(m_pConfData->m_bDCmeasurement, AmplDispMode, WinkelDispMode, PrimSekDispMode);
    m_pContextMenu->show();

}

void WMRawActualValBase::actFontSize()
{
    QSize ls = this->size();
    QFont fo(ui->FreqDisp->font());
    int divider, divider2, minSize;
#ifdef FVWM // height = 104 -> 15 (was 10) on PC and on device (104/5 = 21 (was 14))
    divider = 5;
    divider2 = 7;
    minSize = 14;
#else
    divider = 10;
    divider2 = 12;
    minSize = 10;
#endif
    int pointSize = ls.height()/divider;
    fo.setPointSize(pointSize);
    ui->FreqDisp->setFont(fo);
    ui->FreqLabel->setFont(fo);

    pointSize = ls.height()/divider2;
    if (pointSize < minSize) pointSize = minSize;
    fo.setPointSize(pointSize);
    ui->XnAmplDisp->setFont(fo);
    ui->XnPhaseDisp->setFont(fo);
    ui->XnLabel->setFont(fo);
    ui->XxAmplDisp->setFont(fo);
    ui->XxPhaseDisp->setFont(fo);
    ui->XxLabel->setFont(fo);
}

void WMRawActualValBase::mousePressEvent(QMouseEvent *event)
{
    if(m_Timer.isActive()){
        activateContextMenu();
    }
    else {
    m_Timer.start();
    }
    event->accept();
}

void WMRawActualValBase::contextMenuEvent(QContextMenuEvent *)
{
    activateContextMenu();
}

void WMRawActualValBase::ReceiveVektorDispFormat(int m, int m2, int m3)
{
    AmplDispMode = m;
    WinkelDispMode = m2;
    PrimSekDispMode = m3;
    setSettingslabel();
    onSaveConfig();
}

void WMRawActualValBase::takeScreenshoots()
{
    mScrShooter->showed(false);
    if (!this->isShown())
    {
        this->show();
        mScrShooter->showed(true);
    }
    mScrShooter->useTimer(this, 33);
}

void WMRawActualValBase::takeScreenshootSetting()
{
    activateContextMenu();
    mScrShooter->useTimer(m_pContextMenu, 34);
}

void WMRawActualValBase::takeScreenshootSettingFinished()
{
    m_pContextMenu->hide();
    if(mScrShooter->showed())
    {
        this->hide();
    }
}

void WMRawActualValBase::onSaveConfig()
{
    onSaveSession(".ses");
}

void WMRawActualValBase::onWriteStreamForGeomChange()
{
    m_geomToFromStream = geometryFromWidget(this);
    onSaveConfig();
}

void WMRawActualValBase::setSettingslabel()
{
    QString str;
    str.append("Ampl.: ");
    switch (AmplDispMode){
    case x1:
    str.append("x1");
    break;
    case x1_SQRT2:
    str.append("RMS");
    break;
    }
    str.append("\t");
    switch (PrimSekDispMode){
    case prim:
    str.append("prim");
    break;
    case sek:
    str.append("sec");
    break;
    }
    str.append("\t");
    switch (WinkelDispMode ) {
    case mathpos:
    str.append("math pos");
    break;
    case techpos:
    str.append("tec pos");
    break;
    }
    ui->Settingslabel->setStyleSheet("color: gray");
    ui->Settingslabel->setText(str);
}

void WMRawActualValBase::readStream(QDataStream &stream)
{
    stream >> m_geomToFromStream;
    stream >> AmplDispMode;
    stream >> WinkelDispMode;
    stream >> PrimSekDispMode;
    geometryToWidget(m_geomToFromStream, this);
}

void WMRawActualValBase::writeStream(QDataStream &stream)
{
    stream << m_geomToFromStream;
    stream << AmplDispMode;
    stream << WinkelDispMode;
    stream << PrimSekDispMode;
}

void WMRawActualValBase::setInitialDefaults()
{
    AmplDispMode = x1;
    PrimSekDispMode = prim;
    WinkelDispMode = mathpos;
}

void WMRawActualValBase::setDefaults()
{
    setInitialDefaults();
}
