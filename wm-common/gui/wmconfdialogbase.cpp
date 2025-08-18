#include "wmconfdialogbase.h"
#include "wmglobalcommon.h"

wmconfdialogbase::wmconfdialogbase(QWidget *parent):
    QDialog(parent)
{
    mGuiHelper = new confGuiHelper(true);
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

void wmconfdialogbase::showRatio(QWidget *poi)
{
    poi->setParent(0);
    poi->setWindowTitle("Transformer Ratio");
    poi->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);
    poi->show();
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

bool wmconfdialogbase::is_3(const QString &s)
{
     return (s.contains("/3"));
}

bool wmconfdialogbase::is_w3(const QString &s)
{
    return (s.contains("/w3"));
}

