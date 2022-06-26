#include <QCloseEvent>
#include <QFileInfo>
#include "wmoeviewbase.h"
#include "ui_wmoeviewbase.h"

WMOeViewBase::WMOeViewBase(QWidget* parent, QString machineName):
    QDialog(parent),
    ui(new Ui::WMOeViewBase),
    m_sessionReadWrite(machineName)
{
    ui->setupUi(this);
    init();
}

WMOeViewBase::~WMOeViewBase()
{
    destroy();
    delete ui;
}


void WMOeViewBase::init()
{
    m_Timer.setSingleShot(true);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
    onLoadSession(".ses");
}


void WMOeViewBase::destroy()
{
    saveConfiguration();
}


void WMOeViewBase::saveConfiguration()
{
    onSaveSession(".ses");
}


void WMOeViewBase::ReceiveOEViewDataSlot(cOwnErrorViewData *oe)
{
    m_OwnErrorView=*oe;
    ui->WandlerName->setText(m_OwnErrorView.m_sTrName);
    ui->PrimDisp->setText(m_OwnErrorView.m_sPrim);
    ui->SekDisp->setText(m_OwnErrorView.m_sSec);
    ui->LoadpointDisp->setText(m_OwnErrorView.m_sLoad);

    if (m_OwnErrorView.m_bValid)
    {
        ui->AmplDisp->setText(m_OwnErrorView.m_sAmpl);
        ui->PhaseDisp->setText(m_OwnErrorView.m_sPhase);
    }
    else
    {
        ui->AmplDisp->setText("--------");
        ui->PhaseDisp->setText("--------");
    }
}


void WMOeViewBase::closeEvent( QCloseEvent* ce)
{
    m_widGeometry.setPoint(pos());
    m_widGeometry.setSize(size());
    m_widGeometry.setVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}


void WMOeViewBase::resizeEvent(QResizeEvent *)
{
    m_Timer.start(500);
}


void WMOeViewBase::moveEvent(QMoveEvent *)
{
    m_Timer.start(500);
}


void WMOeViewBase::onShowHide( bool b )
{
  if (b) show();else close();
}


void WMOeViewBase::onSaveSession(QString session)
{
    m_sessionReadWrite.writeSession(this, m_widGeometry, session);
}


bool WMOeViewBase::onLoadSession(QString session)
{
  WidgetGeometry tmpGeometry = m_sessionReadWrite.readSession(this, session);
  if(tmpGeometry.getSize().isValid())
  {
    m_widGeometry=tmpGeometry;
    return true;
  }
  else
  {
    return false;
  }
}


