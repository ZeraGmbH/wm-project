#include "wmoffsetvalbase.h"
#include "ui_wmoffsetvalbase.h"
#include <geometrytowidget.h>
#include <QFileInfo>
#include <QCloseEvent>
#include <QDir>

WMOffsetValBase::WMOffsetValBase(QString machineName, IWmOffsetCustomLabels *customLabels, QWidget* parent):
    QDialog(parent),
    ui(new Ui::WMOffsetValBase),
    m_customLabels(customLabels),
    m_sessionStreamer(machineName, this)
{
    ui->setupUi(this);
    m_JustValues.OffsetCorrDevN = 0.0;
    m_JustValues.OffsetCorrDevX = 0.0;
    m_customLabels->setUiTexts(ui->XnLabel, ui->XxLabel);
    actualizeDisplay(ui, &m_ConfData, &m_JustValues);
    connect(&m_settingsChangeTimer, SIGNAL(sigWriteStreamForGeomChange()), this, SLOT(onWriteStreamForGeomChange()));
    onLoadSession(".ses");
}

WMOffsetValBase::~WMOffsetValBase()
{
    delete ui;
    delete m_customLabels;
}

void WMOffsetValBase::closeEvent(QCloseEvent* ce)
{
    m_settingsChangeTimer.startDelayed();
    emit sigIsVisible(false);
    ce->accept();
}

void WMOffsetValBase::resizeEvent (QResizeEvent *)
{
    m_settingsChangeTimer.startDelayed();
}

void WMOffsetValBase::moveEvent(QMoveEvent *)
{
    m_settingsChangeTimer.startDelayed();
}

void WMOffsetValBase::onShowHide(bool shw)
{
    m_settingsChangeTimer.startDelayed();
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
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}

void WMOffsetValBase::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
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

void WMOffsetValBase::readStream(QDataStream &stream)
{
    stream >> m_geomToFromStream;
    geometryToWidget(m_geomToFromStream, this);
}

void WMOffsetValBase::writeStream(QDataStream &stream)
{
    stream << m_geomToFromStream;
}

void WMOffsetValBase::setDefaults()
{
}

void WMOffsetValBase::actualizeDisplay(Ui::WMOffsetValBase* ui, cConfData* conf, tJustValues* just)
{
    m_customLabels->updateValues(ui->XnOffsDisp, ui->XxOffsDisp, conf, just);
}
