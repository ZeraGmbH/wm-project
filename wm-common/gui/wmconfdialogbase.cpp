#include "wmconfdialogbase.h"
#include "geometrytowidget.h"
#include "wmglobalcommon.h"

wmconfdialogbase::wmconfdialogbase(QWidget *parent, QString machineName):
    QDialog(parent),
    m_sessionStreamer(machineName, this)
{
    mGuiHelper = new confGuiHelper();
}

wmconfdialogbase::~wmconfdialogbase()
{
    delete mGuiHelper;
}

void wmconfdialogbase::setScreenShooter(screenshooter *poi)
{
    mScrShooter = poi;
    connect(mScrShooter,SIGNAL(keyboardScreenShot()),this,SLOT(keyboardScreenshorTriggerd()));

}

bool wmconfdialogbase::onLoadSession(QString session)
{
    m_sessionStreamer.readSession(objectName(), session);
    return true;
}

void wmconfdialogbase::onSaveSession(QString session)
{
    m_sessionStreamer.writeSession(objectName(), session);
}

void wmconfdialogbase::showRatio(QWidget *poi)
{
    mpTransRatio = poi;
    poi->setParent(0);
    poi->setWindowTitle("Transformer Ratio");
    poi->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);
    onLoadSession(".ses");
    poi->show();
}

void wmconfdialogbase::cancelRatio(QWidget *poi)
{
    mWmKeyBoard->hide();
    onSaveSession(".ses");
    poi->close();
}

void wmconfdialogbase::SuggestASDUs()
{
    m_ConfDataTemp.FirstASDU = 1;
    m_ConfDataTemp.LastASDU = 1;
    switch (m_ConfDataTemp.m_nSFreq)
    {
    case F16: // 16.67 hz sind bei nconventional nicht unterstützt
        break;

    case F50:
        switch (m_ConfDataTemp.m_nSRate)
        {
        case S80:
            break;
        case S96:
            m_ConfDataTemp.LastASDU = 2;
            break;
        case S256:
            m_ConfDataTemp.LastASDU = 8;
            break;
        case S288:
            m_ConfDataTemp.LastASDU = 6;
            break;
        }
        break;

    case F60:
        switch (m_ConfDataTemp.m_nSRate)
        {
        case S80:
        case S96:
            break;
        case S240:
            m_ConfDataTemp.LastASDU = 6;
            break;
        case S256:
            m_ConfDataTemp.LastASDU = 8;
            break;
        }
        break;
    }
}

void wmconfdialogbase::readStream(QDataStream &stream)
{
    if (mpTransRatio != nullptr)
    {
        stream >> m_geomToFromStream;
        geometryToWidget(m_geomToFromStream, mpTransRatio);
    }
}

void wmconfdialogbase::writeStream(QDataStream &stream)
{
    if (mpTransRatio != nullptr)
    {
        m_geomToFromStream = geometryFromWidget( mpTransRatio);
        stream << m_geomToFromStream;
    }
}

void wmconfdialogbase::setDefaults()
{

}

bool wmconfdialogbase::is_3(const QString &s)
{
     return (s.contains("/3"));
}

bool wmconfdialogbase::is_w3(const QString &s)
{
    return (s.contains("/w3"));
}

