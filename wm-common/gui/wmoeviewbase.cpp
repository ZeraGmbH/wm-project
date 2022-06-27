#include "wmoeviewbase.h"
#include "ui_wmoeviewbase.h"
#include "geometrytowidget.h"
#include <QCloseEvent>

WMOeViewBase::WMOeViewBase(QWidget* parent, QString machineName):
    QDialog(parent),
    ui(new Ui::WMOeViewBase),
    m_sessionReadWrite(machineName)
{
    ui->setupUi(this);
    connect(&m_geomChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
}

WMOeViewBase::~WMOeViewBase()
{
    onSaveConfig();
    delete ui;
}

void WMOeViewBase::onSaveConfig()
{
    onSaveSession(".ses");
}

void WMOeViewBase::onWriteStreamForGeomChange()
{
    m_geomToFromStream = geometryFromWidget(this);
    onSaveConfig();
}

void WMOeViewBase::ReceiveOEViewDataSlot(cOwnErrorViewData *oe)
{
    m_OwnErrorView=*oe;
    ui->WandlerName->setText(m_OwnErrorView.m_sTrName);
    ui->PrimDisp->setText(m_OwnErrorView.m_sPrim);
    ui->SekDisp->setText(m_OwnErrorView.m_sSec);
    ui->LoadpointDisp->setText(m_OwnErrorView.m_sLoad);

    if (m_OwnErrorView.m_bValid) {
        ui->AmplDisp->setText(m_OwnErrorView.m_sAmpl);
        ui->PhaseDisp->setText(m_OwnErrorView.m_sPhase);
    }
    else {
        ui->AmplDisp->setText("--------");
        ui->PhaseDisp->setText("--------");
    }
}

void WMOeViewBase::closeEvent( QCloseEvent* ce)
{
    m_geomChangeTimer.handleGeomChange();
    emit sigIsVisible(false);
    ce->accept();
}

void WMOeViewBase::resizeEvent(QResizeEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void WMOeViewBase::moveEvent(QMoveEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void WMOeViewBase::onShowHide(bool shw )
{
    m_geomChangeTimer.handleGeomChange();
    if (shw)
        show();
    else
        close();
}

void WMOeViewBase::onSaveSession(QString session)
{
    m_sessionReadWrite.writeSession(this, m_geomToFromStream, session);
}

bool WMOeViewBase::onLoadSession(QString session)
{
    WidgetGeometry tmpGeometry = m_sessionReadWrite.readSession(this, session);
    if(tmpGeometry.getSize().isValid()) {
        m_geomToFromStream=tmpGeometry;
        return true;
    }
    else
    {
        return false;
    }
}

