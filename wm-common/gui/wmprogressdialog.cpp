#include "wmprogressdialog.h"

wmProgressDialog::wmProgressDialog(QWidget *parent)
    : QDialog{parent}
{
    seuptGui();
}

wmProgressDialog::wmProgressDialog(QString strTitle, int minValue, int maxValue, QWidget *parent)
    : QDialog{parent}
{
    seuptGui();
    setTitel(strTitle);
    setMinMaxValue(minValue,maxValue);

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
    calcTimes(value);
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
    if (mShow){
    mState->setText("Restzeit :" + mCalcRest.toString("hh:mm:ss") +"\nEndzeit" + mEndZeit.toString("hh:mm:ss") );
    mCalcRest = mCalcRest.addSecs(-1);
    }
    else
        mState->clear();
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
    emit aborted();
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

void wmProgressDialog::seuptGui()
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

void wmProgressDialog::calcTimes(int val)
{
    int a,b,c;
    if (val == 0) {
        mStart0 = QTime::currentTime();
        mShow = false;
        mLastNumber = 0;
        mStart0ChangedAtVal = 1;
    }
    else {
        if (mLastNumber != val) {
                mNext = QTime::currentTime();
                QTime time;
                time.setHMS(0,0,0);
                a = mStart0.secsTo(mNext);
                if (a<0) { // time is negativ
                    qDebug("times are worng");
                    mStart0 = mNext;
                    mStart0ChangedAtVal = val;
                }
                if (val > mStart0ChangedAtVal){
                    b= a / (val-mStart0ChangedAtVal);
                    c= (b * mProgressFirst->maximum()) - ((val-1)*b);
                    mCalcRest = time.addSecs(c);
                    time = QTime::currentTime();
                    mEndZeit = time.addSecs(c);
                    qDebug("calcTimes: number: %i duration: %i sum: %i sec",val-1,b,c);
                    mShow = true;
                }
                mLastNumber = val;
            }
        }
}
