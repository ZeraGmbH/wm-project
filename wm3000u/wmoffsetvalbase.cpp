#include "wmoffsetvalbase.h"
#include "ui_wmoffsetvalbase.h"
#include <geometrytowidget.h>
#include <QFileInfo>
#include <QCloseEvent>
#include <QDir>

WMOffsetValBase::WMOffsetValBase(IWmOffsetCustomLabels *customLabels, QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMOffsetValBase),
    m_customLabels(customLabels)
{
    ui->setupUi(this);
    m_JustValues.OffsetCorrDevN = 0.0;
    m_JustValues.OffsetCorrDevX = 0.0;
    m_customLabels->setUiTexts(ui->XnLabel, ui->XxLabel);
    actualizeDisplay(ui, &m_ConfData, &m_JustValues);
    connect(&m_geomChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
}

WMOffsetValBase::~WMOffsetValBase()
{
    onSaveConfig();
    delete ui;
    delete m_customLabels;
}

void WMOffsetValBase::closeEvent(QCloseEvent* ce)
{
    m_geomChangeTimer.handleGeomChange();
    emit sigIsVisible(false);
    ce->accept();
}

void WMOffsetValBase::resizeEvent (QResizeEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void WMOffsetValBase::moveEvent(QMoveEvent *)
{
    m_geomChangeTimer.handleGeomChange();
}

void WMOffsetValBase::onShowHide(bool shw)
{
    m_geomChangeTimer.handleGeomChange();
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

void WMOffsetValBase::SetConfInfoSlot(cConfData *cd)
{
    m_ConfData = *cd;
    actualizeDisplay(ui, &m_ConfData, &m_JustValues);
}

bool WMOffsetValBase::onLoadSession(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/%2%3").arg(QDir::homePath()).arg(name()).arg(fi.fileName());
    QFile file(ls);
    if ( file.open( QIODevice::ReadOnly ) ) {
        QDataStream stream( &file );
        stream >> m_geomToFromStream;
        file.close();
        hide();
        resize(m_geomToFromStream.getSize());
        move(m_geomToFromStream.getPoint());
        if (m_geomToFromStream.getVisible()) {
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
            m_geomToFromStream.setPoint(pos());
            m_geomToFromStream.setSize(size());
        }
        m_geomToFromStream.setVisible(vi);

        QDataStream stream( &file );
        stream << m_geomToFromStream;
        file.close();
    }
}

void WMOffsetValBase::onSaveConfig()
{
    onSaveSession(".ses");
}

void WMOffsetValBase::onWriteStreamForGeomChange()
{
    m_geomToFromStream = geometryFromWidget(this);
    onSaveConfig();
}

void WMOffsetValBase::actualizeDisplay(Ui::WMOffsetValBase* ui, cConfData* conf, tJustValues* just)
{
    m_customLabels->updateValues(ui->XnOffsDisp, ui->XxOffsDisp, conf, just);
}
