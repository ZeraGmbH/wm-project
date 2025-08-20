#include "wmrangedialogbase.h"
#include "geometrytowidget.h"

wmRangeDialogBase::wmRangeDialogBase(QWidget *parent, QString machineName) :
    QDialog (parent),
    m_sessionStreamer(machineName, this)
{}

wmRangeDialogBase::~wmRangeDialogBase()
{

}

bool wmRangeDialogBase::onLoadSession(QString session)
{
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}

void wmRangeDialogBase::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
}

void wmRangeDialogBase::readStream(QDataStream &stream)
{
    stream >> m_geomToFromStream;
    geometryToWidget(m_geomToFromStream, this);
}

void wmRangeDialogBase::writeStream(QDataStream &stream)
{
    m_geomToFromStream = geometryFromWidget( this);
    stream << m_geomToFromStream;
}

void wmRangeDialogBase::setDefaults()
{

}

void wmRangeDialogBase::setScreenShooter(screenshooter *poi)
{
    mScrShooter = poi;
}

void wmRangeDialogBase::takeScreenshoots()
{
    mScrShooter->showed(false);
    if (!getChildThis()->isShown())
    {
        getChildThis()->show();
        mScrShooter->showed(true);
    }
    mScrShooter->useTimer(getChildThis(), 37);
}


void wmRangeDialogBase::takeScreenshootFinished()
{
    if(mScrShooter->showed())
    {
        getChildThis()->hide();
    }
}
