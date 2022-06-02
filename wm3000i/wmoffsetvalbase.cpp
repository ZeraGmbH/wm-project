//Added by qt3to4:

#include <QFileInfo>
#include <QCloseEvent>
#include "wmoffsetvalbase.h"
#include "ui_wmoffsetvalbase.h"

const double PI2 = 6.283185307;


WMOffsetValBase::WMOffsetValBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMOffsetValBase)
{
    ui->setupUi(this);
    init();
}


WMOffsetValBase::~WMOffsetValBase()
{
    destroy();
    delete ui;
}

void WMOffsetValBase::init()
{
    m_JustValues.OffsetCorrDevN = 0.0;
    m_JustValues.OffsetCorrDevX = 0.0;
    ActualizeDisplay();
    m_Timer.setSingleShot(true);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(saveConfiguration()));
    LoadSession(".ses");

}


void WMOffsetValBase::destroy()
{
    SaveSession(".ses");
}


void WMOffsetValBase::ActualizeDisplay()
{
    if (isVisible())
    {
        ui->XnOffsDisp -> setText( QString("%1 A").arg(m_JustValues.OffsetCorrDevN,10,'f',5) );
        if (m_ConfData.m_nMeasMode == In_ECT)
            ui->XxOffsDisp -> setText( QString("%1 V").arg(m_JustValues.OffsetCorrDevX,10,'f',5) );
        else
            ui->XxOffsDisp -> setText( QString("%1 A").arg(m_JustValues.OffsetCorrDevX,10,'f',5) );
    }
}


void WMOffsetValBase::closeEvent(QCloseEvent* ce)
{
    m_widGeometry.SetGeometry(pos(),size()); // wir halten visible und geometrie nach
    m_widGeometry.SetVisible(0);
    emit isVisibleSignal(false);
    m_Timer.start(500);
    ce->accept();
}


void WMOffsetValBase::resizeEvent ( QResizeEvent *)
{
    m_Timer.start(500);
}


void WMOffsetValBase::moveEvent( QMoveEvent *)
{
    m_Timer.start(500);
}


void WMOffsetValBase::ShowHideJustSlot( bool b)
{
    if (b) show();else close();
}


void WMOffsetValBase::ReceiveJustDataSlot( tJustValues *JustValues )
{
    m_JustValues = *JustValues;
    ActualizeDisplay();
}


void WMOffsetValBase::SetConfInfoSlot(cConfData *cd)
{
    m_ConfData = *cd;
    ActualizeDisplay();
}


bool WMOffsetValBase::LoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000i/%2%3").arg(wm3000iHome).arg(name()).arg(fi.fileName());
    QFile file(ls);
    if ( file.open( QIODevice::ReadOnly ) ) {
    QDataStream stream( &file );
    stream >> m_widGeometry;
    file.close();
    hide();
    resize(m_widGeometry.m_Size);
    move(m_widGeometry.m_Point);
    if (m_widGeometry.vi)
    {
        show();
        emit isVisibleSignal(true);
    }
// FVWM und Gnome verhalten sich anders
#ifndef FVWM
    move(m_widGeometry.m_Point);
#endif
    return true;
    }
    return false;
}


void WMOffsetValBase::SaveSession(QString session)
{
    QFileInfo fi(session);
    if(!QDir(QString("%1/.wm3000i/").arg(wm3000iHome)).exists())
    {
      //create temporary object that gets deleted when leaving the control block
      QDir().mkdir(QString("%1/.wm3000i/").arg(wm3000iHome));
    }

    QString ls = QString("%1/.wm3000i/%2%3").arg(wm3000iHome).arg(name()).arg(fi.fileName());
    QFile file(ls);
//    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
    file.at(0);

    int vi;
    vi = (isVisible()) ? 1 : 0;
    if (vi)
        m_widGeometry.SetGeometry(pos(),size());
    m_widGeometry.SetVisible(vi);

    QDataStream stream( &file );
    stream << m_widGeometry;
    file.close();
    }
}


void WMOffsetValBase::saveConfiguration()
{
    SaveSession(".ses");
}
