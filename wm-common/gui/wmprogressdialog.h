#ifndef WMPROGRESSDIALOG_H
#define WMPROGRESSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QTime>

class wmProgressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit wmProgressDialog(QWidget *parent = nullptr);
    explicit wmProgressDialog(QString strTitle,int minValue, int maxValue, QWidget *parent = nullptr);
    ~wmProgressDialog();
    bool isAbort() {return mbAbort;};
    void setMinMaxValue(int min1, int max1, int min2 = 0, int max2 = 0, int min3 = 0, int max3 = 0);
    void setMinMax1st(int min, int max);
    void setMinMax2nd(int min, int max);
    void setMinMax3rd(int min, int max);
    void setValue(int value);
    void setValue2(int value);
    void setValue3(int value);
    void setTitel(QString str);
    void setLabelText(QString str);
    void setStatusStr(QString str);
    void setAbortButtonText(QString str);
    void setMessageStr(QString str);
    void set2ndDisabled();
    void set3rdDisabled();
signals:
    void aborted();
    void actualStateString(QString);

private:
    QPushButton *mAbort;
    QProgressBar *mProgressFirst;
    QProgressBar *mProgressSecond;
    QProgressBar *mProgressThird;
    QLabel *mPrompt, *mState, *mHint;
    QVBoxLayout *mvLay;
    QHBoxLayout *mhLay;
    bool mbAbort;
    QTime mStart0, mNext, mCalcRest, mEndZeit;
    bool mShow;
    int mLastNumber, mStart0ChangedAtVal;

    void seuptGui();
    void calcTimes(int val);

private slots:
    void abort();
};

#endif // WMPROGRSSEDIALOG_H
