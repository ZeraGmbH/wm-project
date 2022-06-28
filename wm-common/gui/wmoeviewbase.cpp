#include "wmoeviewbase.h"
#include "ui_wmoeviewbase.h"
#include "geometrytowidget.h"
#include <QCloseEvent>

WMOeViewBase::WMOeViewBase(QWidget* parent, QString machineName):
    QDialog(parent),
    ui(new Ui::WMOeViewBase),
    m_sessionStreamer(machineName, this)
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

void WMOeViewBase::readStream(QDataStream &stream)
{
    stream >> m_geomToFromStream;
    geometryToWidget(m_geomToFromStream, this);
}

void WMOeViewBase::writeStream(QDataStream &stream)
{
    stream << m_geomToFromStream;
}

void WMOeViewBase::setDefaults()
{
}

void WMOeViewBase::ReceiveOEViewDataSlot(cOwnErrorViewData *oe)
{
    ui->WandlerName->setText(oe->m_sTrName);
    ui->PrimDisp->setText(oe->m_sPrim);
    ui->SekDisp->setText(oe->m_sSec);
    ui->LoadpointDisp->setText(oe->m_sLoad);
    if (oe->m_bValid) {
        ui->AmplDisp->setText(oe->m_sAmpl);
        ui->PhaseDisp->setText(oe->m_sPhase);
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

void WMOeViewBase::onShowHide(bool shw)
{
    m_geomChangeTimer.handleGeomChange();
    if (shw)
        show();
    else
        close();
}

void WMOeViewBase::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
}

bool WMOeViewBase::onLoadSession(QString session)
{
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}
