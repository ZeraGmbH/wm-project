#include "wmoffsetvalbase.h"
#include "ui_wmoffsetvalbase.h"
#include <QFileInfo>
#include <QCloseEvent>

WMOffsetValBase::WMOffsetValBase( QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMOffsetValBase)
{
    ui->setupUi(this);
    m_JustValues.OffsetCorrDevN = 0.0;
    m_JustValues.OffsetCorrDevX = 0.0;
    setUiTexts(ui);
    actualizeDisplay(ui, &m_ConfData, &m_JustValues);
    m_Timer.setSingleShot(true);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(onSaveConfig()));
    onLoadSession(".ses");
}

WMOffsetValBase::~WMOffsetValBase()
{
    onSaveConfig();
    delete ui;
}

void WMOffsetValBase::closeEvent(QCloseEvent* ce)
{
    m_widGeometry.setPoint(pos());
    m_widGeometry.setSize(size());
    m_widGeometry.setVisible(0);
    emit sigIsVisible(false);
    m_Timer.start(500);
    ce->accept();
}

void WMOffsetValBase::resizeEvent ( QResizeEvent *)
{
    m_Timer.start(500);
}

void WMOffsetValBase::moveEvent(QMoveEvent *)
{
    m_Timer.start(500);
}

void WMOffsetValBase::onShowHide(bool shw)
{
    if (shw)
        show();
    else
        close();
}

void WMOffsetValBase::ReceiveJustDataSlot(tJustValues *JustValues)
{
    m_JustValues = *JustValues;
    actualizeDisplay(ui, &m_ConfData, &m_JustValues);
}

bool WMOffsetValBase::onLoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(QDir::homePath()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    if ( file.open( QIODevice::ReadOnly ) ) {
        QDataStream stream( &file );
        stream >> m_widGeometry;
        file.close();
        hide();
        resize(m_widGeometry.getSize());
        move(m_widGeometry.getPoint());
        if (m_widGeometry.getVisible()) {
            show();
            emit sigIsVisible(true);
        }
        // FVWM und Gnome verhalten sich anders
#ifndef FVWM
        move(m_widGeometry.getPoint());
#endif
        return true;
    }
    return false;
}

void WMOffsetValBase::onSaveSession(QString session)
{
    QFileInfo fi(session);
    if(!QDir(QString("%1/.wm3000u/").arg(QDir::homePath())).exists()) {
        //create temporary object that gets deleted when leaving the control block
        QDir().mkdir(QString("%1/.wm3000u/").arg(QDir::homePath()));
    }

    QString ls = QString("%1/.wm3000u/%2%3").arg(QDir::homePath()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    //    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
        file.at(0);

        int vi;
        vi = (isVisible()) ? 1 : 0;
        if (vi) {
            m_widGeometry.setPoint(pos());
            m_widGeometry.setSize(size());
        }
        m_widGeometry.setVisible(vi);

        QDataStream stream( &file );
        stream << m_widGeometry;
        file.close();
    }
}

void WMOffsetValBase::onSaveConfig()
{
    onSaveSession(".ses");
}

void WMOffsetValBase::setUiTexts(Ui::WMOffsetValBase *ui)
{
    ui->XnLabel->setText("Un:");
    ui->XxLabel->setText("Ux:");
}

void WMOffsetValBase::actualizeDisplay(Ui::WMOffsetValBase* ui, cConfData* conf, tJustValues* just)
{
    Q_UNUSED(conf)
    ui->XnOffsDisp -> setText(QString("%1 V").arg(just->OffsetCorrDevN,10, 'f', 5));
    ui->XxOffsDisp -> setText(QString("%1 V").arg(just->OffsetCorrDevX,10, 'f', 5));
}
