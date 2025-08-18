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

