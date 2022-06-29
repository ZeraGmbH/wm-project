#include "wmrawactualvalbase.h"
#include "ui_wmrawactualvalbase.h"
#include "wmglobal.h"
#include "common-modes.h"
#include <tools.h>
#include <geometrytowidget.h>
#include <QContextMenuEvent>
#include <QCloseEvent>
#include <QFileInfo>

const double PI2 = 6.283185307;

WMRawActualValBase::WMRawActualValBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMRawActualValBase)
{
    ui->setupUi(this);
    ui->XnAmplDisp -> setText( QString("%1 V").arg(0.0,10,'f',5) );
    ui->XnPhaseDisp -> setText( QString("%1 %2").arg(0.0,8,'f',4).arg( trUtf8("°")) );
    ui->XxAmplDisp -> setText( QString("%1 V").arg(0.0,10,'f',5) );
    ui->XxPhaseDisp -> setText( QString("%1 %2").arg(0.0,8,'f',4).arg( trUtf8("°")) );

    AmplDispMode = x1;
    PrimSekDispMode = prim;
    WinkelDispMode = mathpos;
    m_pContextMenu = new WMRawActualConfigBase(this);
    connect(this,SIGNAL(SendVektorDispFormat(bool,int,int,int)),m_pContextMenu,SLOT(ReceiveDisplayConfSlot(bool,int,int,int)));
    connect(m_pContextMenu,SIGNAL(SendVektorDisplayFormat(int,int,int)),this,SLOT(ReceiveVektorDispFormat(int,int,int)));
    connect(&m_geomChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
}

WMRawActualValBase::~WMRawActualValBase()
{
    onSaveConfig();
    delete ui;
}

void WMRawActualValBase::closeEvent(QCloseEvent* ce)
{
    m_geomChangeTimer.handleGeomChange();
    emit sigIsVisible(false);
    ce->accept();
}

void WMRawActualValBase::resizeEvent(QResizeEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void WMRawActualValBase::moveEvent(QMoveEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void WMRawActualValBase::onShowHide(bool shw)
{
    m_geomChangeTimer.handleGeomChange();
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
    resize(ui->gridLayout->minimumSize());
}


bool WMRawActualValBase::onLoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(QDir::homePath()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream( &file );
        stream >> m_geomToFromStream;
        stream >> AmplDispMode;
        stream >> WinkelDispMode,
                stream >> PrimSekDispMode;
        file.close();
        hide();
        resize(m_geomToFromStream.getSize());
        move(m_geomToFromStream.getPoint());
        if (m_geomToFromStream.getVisible())
        {
            show();
            emit sigIsVisible(true);
        }
        // FVWM und Gnome verhalten sich anders
#ifndef FVWM 
        move(m_geomToFromStream.getPoint());
#endif   
        return true;
    }
    return false;
}

void WMRawActualValBase::onSaveSession(QString session)
{
    if(!QDir(QString("%1/.wm3000u/").arg(QDir::homePath())).exists())
    {
        //create temporary object that gets deleted when leaving the control block
        QDir().mkdir(QString("%1/.wm3000u/").arg(QDir::homePath()));
    }
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(QDir::homePath()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
        file.at(0);

        int vi;
        vi = (isVisible()) ? 1 : 0;
        if (vi) {
            m_geomToFromStream.setPoint(pos());
            m_geomToFromStream.setSize(size());
        }
        m_geomToFromStream.setVisible(vi);

        QDataStream stream( &file );
        stream << m_geomToFromStream;
        stream << AmplDispMode;
        stream << WinkelDispMode;
        stream << PrimSekDispMode;
        file.close();
    }
}

void WMRawActualValBase::contextMenuEvent(QContextMenuEvent *)
{
    emit SendVektorDispFormat(m_pConfData->m_bDCmeasurement, AmplDispMode, WinkelDispMode, PrimSekDispMode);
    m_pContextMenu->show();
}

void WMRawActualValBase::ReceiveVektorDispFormat(int m, int m2, int m3)
{
    AmplDispMode = m;
    WinkelDispMode = m2;
    PrimSekDispMode = m3;
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
