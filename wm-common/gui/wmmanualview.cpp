#include "wmmanualview.h"
#include "ui_wmmanualview.h"
#include "wmmessagebox.h"

// path of the dokumentation may be:
// /usr/share/wm3000/wm-docu/html-output
//                          /xml-input/pic/

wmManualView::wmManualView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wmManualView)
{
    ui->setupUi(this);
    mbDC = false;
    mbConventional = false;

}

void wmManualView::setScreenShooter(screenshooter* poi)
{
    mScrShooter = poi;
}

void wmManualView::takeScreenshoots()
{
    mScrShooter->showed(false);
    if (!this->isShown())
    {
        this->show();
        mScrShooter->showed(true);
    }
    mScrShooter->useTimer(this, 43);
}

void wmManualView::takeScreenshootFinished()
{
    if(mScrShooter->showed())
    {
        this->hide();
    }
}

void wmManualView::setLanguage(const QString str)
{
    mLanguage = str;
}

void wmManualView::setTyp(const QString str)
{
    mTyp = str;
}

void wmManualView::setConventional(bool b)
{
    mbConventional = b;
}

void wmManualView::setDC(bool b)
{
    mbDC = b;
}

void wmManualView::setPath()
{
    mPath = "/usr/share/wm3000/";
    mPath = mPath.append("wm-docu/html-output/");
    mPath = mPath.append(mLanguage);
    mPath = mPath.append(getDevice());
    if(mbDC) mPath = mPath.append("dc");
    mPath = mPath.append(".html");
}

QString wmManualView::getDevice()
{
    QString str;
    if (mbConventional) str = "wm1000"; else str = "wm3000";
    str = str.append(mTyp);
    return str;
}

void wmManualView::myExecute()
{
    show();
}

void wmManualView::show()
{
    setPath();
    QFile file;
    if(!file.exists(mPath))
    {
        wmMessageBox msgb;
        msgb.noDocu(mPath);
    }
    else
    {
        ui->wvIntroduction->load(QUrl(mPath));
        QWidget::show();
    }
}

wmManualView::~wmManualView()
{
    delete ui;
}

