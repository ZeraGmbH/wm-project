/* Oh my good we are in a mess! there is no QCharts on Qt4, so we
 * need to paint on our own like explained here:
 * https://zetcode.com/gui/qt4/painting/
 * https://het.as.utexas.edu/HET/Software/html/application-windows.html
 *
 */

#ifndef WMSAMPLEDIALOG_H
#define WMSAMPLEDIALOG_H

#include <QDialog>
#include <QObject>

class wmSampleData : public QObject
{
    Q_OBJECT
public:
    int mOffset;
    int mSamples;
    bool mhScale;
    QString mSignalName;
    QList<int> mDisplayValues;
    QPair<float,float> mMinMax;
    QList<float> mCurveValues;
};

class wmSampleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit wmSampleDialog(QWidget *parent = nullptr);
    void setChannelProperties(QString str);
    void setSingalProperties(QString &str, int chan);
    void setSignalNameCh0(QString str);
    void setSignalNameCh1(QString str);
    void setSampleValuesList(QList<float> list, bool append);
    void setSampleValues(float *val, int chan = 0);
    QString getSignalName() {return mSignalName; };
    int getSignalSamples() {return mSamples;};
    QList<float> copyValuesToList(float *val);

    QPair<float,float> getMinMax(wmSampleData &values);
    QList<int> scaleFloatMemberValuesToHight(wmSampleData &values);
    void copyValuesToMemberList(float *val , wmSampleData &values);
    void setMinMaxToMember(wmSampleData &values);
    void nastyCorrectioApproach(wmSampleData &values);

    QPair<float,float> getMinMaxFromValues(wmSampleData &data0, wmSampleData &data1);
    void getMinMaxFromValuesToMember(QPair<float,float> value);

    void scaleToMemberIntValuesToHeight(wmSampleData &values);

protected:
    void paintEvent(QPaintEvent *e);
private:
    void dialogUpdate();

    wmSampleData mDataCh0, mDataCh1;
    QPair<float,float> mMinMax;

    QString mSignalName;
    int mNullLinie, mSamples ;

    void paintLegend(QPainter *qp, QColor color, wmSampleData &values);
    void paintCurve(QPainter *qp, QColor color, wmSampleData &values);
    void paintNullinie(QPainter *qp);

signals:

};

#endif // WMSAMPLEDIALOG_H
