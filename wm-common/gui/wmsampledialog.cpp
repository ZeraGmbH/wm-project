#include "wmsampledialog.h"

#include <QPainter>
#include <cmath>

wmSampleDialog::wmSampleDialog(QWidget *parent)
    : QDialog{parent}
{
    mDataCh0.mOffset = 10;
    mDataCh1.mOffset = 20;
    setMinimumSize(640,240);
}

void wmSampleDialog::setSingalProperties(QString &str,  int chan)
{   // Signalname, number of samples  "MESSSIGNAL0,640;"
    wmSampleData *values;
    if (chan ==  0)
        values = &mDataCh0;
    if (chan ==  1)
        values = &mDataCh1;

    str = str.remove(';');
    values->mSignalName = str.section(",",0,0);
    mSignalName = values->mSignalName;
    values->mSamples = str.section(",",1,1).toInt();
    mSamples = values->mSamples;
}

void wmSampleDialog::setSignalNameCh0(QString str)
{
    mDataCh0.mSignalName = str;
}

void wmSampleDialog::setSignalNameCh1(QString str)
{
    mDataCh1.mSignalName = str;
}


void wmSampleDialog::setSampleValues(float *val, int chan)
{
    if (chan == 0) {
        copyValuesToMemberList(val, mDataCh0);
        setMinMaxToMember(mDataCh0);
        //nastyCorrectioApproach(mDataCh0);
    }
    if (chan == 1){
        copyValuesToMemberList(val, mDataCh1);
        setMinMaxToMember(mDataCh1);
        //nastyCorrectioApproach(mDataCh1);
    }
    getMinMaxFromValuesToMember(getMinMaxFromValues(mDataCh0,mDataCh1));

    scaleToMemberIntValuesToHeight(mDataCh0);
    scaleToMemberIntValuesToHeight(mDataCh1);
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

void wmSampleDialog::copyValuesToMemberList(float *val, wmSampleData &values)
{
    values.mCurveValues = copyValuesToList(val);
}

QPair<float, float> wmSampleDialog::getMinMax(wmSampleData &values)
{
    QPair<float,float> minMax (0.0,0.0);
    for (auto value : values.mCurveValues){
        if (value < minMax.first) minMax.first = value;
        if (value > minMax.second) minMax.second = value;
    }
    if (minMax.first < -1e8) minMax.first = -1e8;
    if (minMax.second > 1e8) minMax.second = 1e8;
    return minMax;
}

void wmSampleDialog::setMinMaxToMember(wmSampleData &values)
{
    values.mMinMax = getMinMax(values);
}

QList<int> wmSampleDialog::scaleFloatMemberValuesToHight(wmSampleData &values)
{
    const int offset = 30;
    QList<int> list;
    float factor(1.0);
    int height = this->geometry().height();
    if (height != 0) {
        factor = (std::fabs(mMinMax.first) + std::fabs(mMinMax.second)) / (height-offset);
        int correction = static_cast<int>(mMinMax.second / factor);
        mNullLinie = correction+offset;
        for (auto value : values.mCurveValues){
            list.append(static_cast<int>((value/factor)*-1)+correction+offset);
        }
    }
    return list;
}

void wmSampleDialog::scaleToMemberIntValuesToHeight(wmSampleData &values)
{
    values.mDisplayValues = scaleFloatMemberValuesToHight(values);
}

void wmSampleDialog::nastyCorrectioApproach(wmSampleData &values)
{   // values smaler then 1 e-38 but bigger than 0 shall be multiplied with 7.07 e 44
    // this is proof of concept and not solution!
    float value;
    for (int i(0);i < values.mCurveValues.length();i++){
        value = values.mCurveValues.at(i);
        if (value > 0 && value < 1e-38){
            value = value *7.07e44;
            values.mCurveValues[i]= value;
        }
    }
}

QPair<float, float> wmSampleDialog::getMinMaxFromValues(wmSampleData &data0, wmSampleData &data1)
{   // wir wollen den größten Wert, am besten beide symetrisch!
    QPair<float,float> minMax;
    if (data0.mMinMax.first > data1.mMinMax.first)
        minMax.first = data1.mMinMax.first;
    else
        minMax.first = data0.mMinMax.first;
    if (data0.mMinMax.second > data1.mMinMax.second)
        minMax.second = data0.mMinMax.second;
    else
        minMax.second = data1.mMinMax.second;
    // symetrisches Skallieren!
    if(minMax.first*-1.0 < minMax.second)
        minMax.first = minMax.second*-1.0;
    if(minMax.first*-1.0 > minMax.second)
        minMax.second = minMax.first*-1.0;
    return minMax;
}

void wmSampleDialog::getMinMaxFromValuesToMember(QPair<float,float> value)
{
    mMinMax = value;
}

void wmSampleDialog::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter qp(this);
    //paintLines(&qp);
    paintNullinie(&qp);
    paintLegend(&qp,Qt::blue,mDataCh0);
    paintLegend(&qp,Qt::red,mDataCh1);
    paintCurve(&qp,Qt::blue,mDataCh0);
    paintCurve(&qp,Qt::red,mDataCh1);
}

void wmSampleDialog::paintLegend(QPainter *qp, QColor color, wmSampleData &values)
{
    QPen pen (color,1,Qt::SolidLine);
    qp->setPen(pen);
    qp->drawText(10,values.mOffset,values.mSignalName);
    qp->drawText(150,values.mOffset,"min: " + QString::number(values.mMinMax.first));
    qp->drawText(300,values.mOffset,"max: " + QString::number(values.mMinMax.second));
    qp->drawText(450,values.mOffset,"n :" + QString::number(values.mDisplayValues.count()));

}

void wmSampleDialog::paintCurve(QPainter *qp, QColor color, wmSampleData &values)
{
    QPen pen (color,1,Qt::SolidLine);
    qp->setPen(pen);
    int lastx, lasty, akty;
    int number = values.mDisplayValues.count();

    for ( int i(0); i< number;i++){
            akty = values.mDisplayValues.at(i);
        if (i!=0){
            qp->drawLine(lastx,lasty,i,akty);
        }
    lastx = i;
    lasty = akty;
    }

    pen.setWidth(2);
    qp->setPen(pen);

        for ( int i(0); i< number;i++){
            qp->drawPoint(i,values.mDisplayValues.at(i));
        }

}

void wmSampleDialog::paintNullinie(QPainter *qp)
{
    int width = this->geometry().width();
    QPen pen (Qt::black,1,Qt::SolidLine);
    qp->setPen(pen);
    qp->drawLine(0,mNullLinie,width,mNullLinie);

}
