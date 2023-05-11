#include "wmprogressdialog.h"

wmProgressDialog::wmProgressDialog(QWidget *parent)
    : QDialog{parent}
{
    mvLay = new QVBoxLayout;
    mAbort = new QPushButton;
    mProgressFirst = new QProgressBar;
    mProgressSecond = new QProgressBar;
    mProgressThird = new QProgressBar;
    mMessage = new QPlainTextEdit;
    mPrompt = new QLabel;
    mState = new QLabel;
    mvLay->addWidget(mPrompt);
    mvLay->addWidget(mProgressFirst);
    mvLay->addWidget(mProgressSecond);
    mvLay->addWidget(mProgressThird);
    mvLay->addWidget(mMessage);
    mhLay = new QHBoxLayout;
    mhLay->addWidget(mState);
    mhLay->addWidget(mAbort);
    mvLay->addItem(mhLay);
    setLayout(mvLay);
    mbAbort = false;
    mProgressSecond->hide();
    mProgressThird->hide();
    mAbort->hide();
    mMessage->hide();

    connect(mAbort,SIGNAL(clicked()),this,SLOT(abort()));
    this->show();
}

wmProgressDialog::~wmProgressDialog()
{
    delete mAbort;
    delete mProgressFirst;
    delete mProgressSecond;
    delete mMessage ;
    delete mPrompt;
    delete mState;
    delete mvLay;
}

void wmProgressDialog::setMinMaxValue(int min1, int max1, int min2, int max2,int min3, int max3)
{
    if ((min1 == 0) && (max1 ==0)){
        mProgressFirst->hide();
    }
    else {
        setMinMax1st(min1,max1);
    }

    if ((min2 == 0) && (max2 ==0)){
        mProgressSecond->hide();
    }
    else {
        setMinMax2nd(min2,max2);
    }
    if ((min3 == 0) && (max3 ==0)){
        mProgressThird->hide();
    }
    else {
        setMinMax3rd(min3,max3);
    }
}

void wmProgressDialog::setValue(int value)
{
    mProgressFirst->setValue(value);
}

void wmProgressDialog::setValue2(int value)
{
    mProgressSecond->setValue(value);
}

void wmProgressDialog::setValue3(int value)
{
    int val;
    val = mProgressThird->maximum();
    mProgressThird->setValue(val-value);
}

void wmProgressDialog::setTitel(QString str)
{
    this->setWindowTitle(str);
}

void wmProgressDialog::setLabelText(QString str)
{
    mPrompt->setText(str);
}

void wmProgressDialog::setStatusStr(QString str)
{
    mState->setText(str);
}

void wmProgressDialog::setAbortButtonText(QString str)
{
    mAbort->setText(str);
    mAbort->show();
}

void wmProgressDialog::setMessageStr(QString str)
{
    mMessage->appendPlainText(str);
    mMessage->show();
}

void wmProgressDialog::abort()
{
    mAbort->setDisabled(true);
    mbAbort = true;
}

void wmProgressDialog::setMinMax1st(int min, int max)
{
    mProgressFirst->setMinimum(min);
    mProgressFirst->setMaximum(max);
    mProgressFirst->show();
}

void wmProgressDialog::setMinMax2nd(int min, int max)
{
    mProgressSecond->setMinimum(min);
    mProgressSecond->setMaximum(max);
    mProgressSecond->show();
}

void wmProgressDialog::setMinMax3rd(int min, int max)
{
    mProgressThird->setMinimum(min);
    mProgressThird->setMaximum(max);
    mProgressThird->show();
}

void wmProgressDialog::set2ndDisabled()
{
    mProgressSecond->hide();
}

void wmProgressDialog::set3rdDisabled()
{
    mProgressThird->hide();
}
