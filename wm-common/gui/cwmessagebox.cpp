#include "cwmessagebox.h"

cWMessageBox::cWMessageBox ( const QString & caption, const QString & text, Icon icon, int button0, int button1, int button2, QWidget * parent, const char * name , bool modal, Qt::WFlags f ) : QMessageBox ( caption, text, icon, button0, button1, button2, parent, name, modal, f)
{
    resetOVL();
}

cWMessageBox::~cWMessageBox()
{
}

bool cWMessageBox::analyseOvld(bool bOvld, bool bOldOvld)
{
    Q_UNUSED(bOldOvld);
    if (bOvld){
        countOVL();
    }
    else{
        resetOVL();
    }
    return OVLD();
}

int cWMessageBox::count()
{
    return mOVL;
}

void cWMessageBox::resetOVL()
{
    mOVL=0;
}

void cWMessageBox::countOVL()
{
    mOVL++;
}

bool cWMessageBox::OVLD()
{
    return (mOVL > 3);
}

void cWMessageBox::done(int r)
{
    emit WMBoxClosed();
    resetOVL();
    QMessageBox::done(r);
}

