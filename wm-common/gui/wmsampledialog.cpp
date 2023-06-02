#include "wmsampledialog.h"

#include <QPainter>
#include <cmath>

wmSampleDialog::wmSampleDialog(QWidget *parent)
    : QDialog{parent}
{

}

void wmSampleDialog::setSingalProperties(QString &str)
{   // Signalname, number of samples  "MESSSIGNAL0,640;"
    str = str.remove(';');
    mSignalName = str.section(",",0,0);
    mSamples = str.section(",",1,1).toInt();
}

void wmSampleDialog::setSampleValues(float *val)
{
    copyValuesToMemberList(val);
    setMinMaxToMember();
    scaleToMemberIntValuesToHeight();
    // Werte liegen in einer Qlist<int> so vor, dass diese ins Fenster
    //"gemalt" werden könnten, Horizontale Scalierung fehlt noch.
    QDialog::update();
}


QList<float> wmSampleDialog::copyValuesToList(float *val)
{
    QList<float> list;
    for (int i(0); i < mSamples; i++ ){
        list.append(*val);
        val++;
    }
    return list;
}

void wmSampleDialog::copyValuesToMemberList(float *val)
{
    mCurveValues = copyValuesToList(val);
}

QPair<float, float> wmSampleDialog::getMinMax()
{
    QPair<float,float> minMax (0.0,0.0);
    for (auto value : mCurveValues){
        if (value < minMax.first) minMax.first = value;
        if (value > minMax.second) minMax.second = value;
    }
    if (minMax.first < -1e8) minMax.first = -1e8;
    if (minMax.second > 1e8) minMax.second = 1e8;
    return minMax;
}

void wmSampleDialog::setMinMaxToMember()
{
    mMinMax = getMinMax();
}

QList<int> wmSampleDialog::scaleFloatMemberValuesToHight()
{
    QList<int> list;
    float factor(1.0);
    int height = this->geometry().height();
    if (height != 0) {
        factor = (std::fabs(mMinMax.first) + std::fabs(mMinMax.second)) / height;
        int correction = static_cast<int>(mMinMax.second / factor);
        mNullLinie = correction;
        for (auto value : mCurveValues){
            list.append(static_cast<int>((value/factor)*-1)+correction);
        }
    }
    return list;
}

void wmSampleDialog::scaleToMemberIntValuesToHeight()
{
    mDisplayValues = scaleFloatMemberValuesToHight();
}

void wmSampleDialog::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter qp(this);
    paintLines(&qp);
}

void wmSampleDialog::paintLines(QPainter *qp)
{
    int width(1);
    width = this->geometry().width();
    QPen pen (Qt::black,1,Qt::SolidLine);
    qp->setPen(pen);
    qp->drawLine(0,mNullLinie,width,mNullLinie);
    qp->drawText(20,20,mSignalName);
    qp->drawText(20,40,"min: " + QString::number(mMinMax.first));
    qp->drawText(20,60,"max: " + QString::number(mMinMax.second));
    qp->drawText(20,80,"n :" + QString::number(mSamples));
    qp->drawText(20,100,"w: " + QString::number(width));

    pen.setWidth(2);
    pen.setColor(Qt::blue);
    qp->setPen(pen);

    int horiVal;
    int number = mDisplayValues.count();

    if (number > width) {
        horiVal = (number / width)+1;
        horiVal = 1;
        for ( int i(0); i< number;i++){
            qp->drawPoint(i*horiVal,mDisplayValues.at(i));
        }
    }
    if (number < width) {
        horiVal = (width / number)+1;
        horiVal = 1;
        for ( int i(0); i< number;){
            qp->drawPoint(i/horiVal,mDisplayValues.at(i));
            i = i + horiVal;
        }
    }
}
