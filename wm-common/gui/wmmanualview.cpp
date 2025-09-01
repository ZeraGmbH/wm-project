#include "wmmanualview.h"
#include "ui_wmmanualview.h"
#include "wmmessagebox.h"

// path of the dokumentation may be:
// /usr/share/wm3000(i,u)/wm-docu/html-output

wmManualView::wmManualView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wmManualView)
{
    ui->setupUi(this);
    mbDC = false;
    mbConventional = false;

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
    mPath = "/usr/share/";
    mPath = mPath.append(getDevice()+"/");
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
        show();
    }
}

wmManualView::~wmManualView()
{
    delete ui;
}

