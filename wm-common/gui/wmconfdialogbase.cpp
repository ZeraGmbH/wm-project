#include "wmconfdialogbase.h"

wmconfdialogbase::wmconfdialogbase(QWidget *parent):
    QDialog(parent)
{

}

wmconfdialogbase::~wmconfdialogbase()
{

}

void wmconfdialogbase::setScreenShooter(screenshooter *poi)
{
    mScrShooter = poi;
    connect(mScrShooter,SIGNAL(keyboardScreenShot()),this,SLOT(keyboardScreenshorTriggerd()));

}

