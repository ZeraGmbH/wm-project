#ifndef WMSCOPEDIALOG_H
#define WMSCOPEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

#include <wmsampledialog.h>

class wmScopeDialog : public QDialog
{
    Q_OBJECT
public:
    wmScopeDialog(QWidget *parent);
    void setComboBoxItems(int Channel, QStringList list);
    void setSampleValues(int Channel, float *val, QString name);
signals:
    void newCha0SelText(QString str);
    void newCha1SelText(QString str);
private:
    QHBoxLayout *mvlay;
    QVBoxLayout *mhlay;
    QComboBox *mChannel0, *mChannel1;
    wmSampleDialog *mSample;
private slots:
    void channel0Changed();
    void channel1Changed();
};

#endif // WMSCOPEDIALOG_H
