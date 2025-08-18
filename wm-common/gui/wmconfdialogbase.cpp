#include "wmconfdialogbase.h"

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

bool wmconfdialogbase::is_3(const QString &s)
{
     return (s.contains("/3"));
}

bool wmconfdialogbase::is_w3(const QString &s)
{
    return (s.contains("/w3"));
}

