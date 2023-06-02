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

class wmSampleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit wmSampleDialog(QWidget *parent = nullptr);
    void setSingalProperties(QString &str);
    void setSampleValues(float *val);
    QString getSignalName() {return mSignalName; };
    int getSignalSamples() {return mSamples;};
    QList<float> copyValuesToList(float *val);
    void copyValuesToMemberList(float *val);
    QPair<float,float> getMinMax();
    void setMinMaxToMember();
    QList<int> scaleFloatMemberValuesToHight();
    void scaleToMemberIntValuesToHeight();

protected:
    void paintEvent(QPaintEvent *e);
private:
    QString mSignalName;
    int mSamples;
    QList<int> mDisplayValues;
    QList<float> mCurveValues;
    QPair<float,float> mMinMax;
    int mNullLinie;

    void paintLines(QPainter *qp);


signals:

};

#endif // WMSAMPLEDIALOG_H
