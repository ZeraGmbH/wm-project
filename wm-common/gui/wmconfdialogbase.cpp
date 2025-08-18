#include "wmconfdialogbase.h"

wmconfdialogbase::wmconfdialogbase(QWidget *parent):
    QDialog(parent)
{
    this->setModal(false);
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

bool wmconfdialogbase::is_3(const QString &s)
{
     return (s.contains("/3"));
}

bool wmconfdialogbase::is_w3(const QString &s)
{
    return (s.contains("/w3"));
}

