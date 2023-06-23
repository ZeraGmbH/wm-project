#ifndef WMSCOPEDIALOG_H
#define WMSCOPEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

#include <wmsampledialog.h>
#include <dspiface.h>

class wmScopeDialog : public QDialog
{
    Q_OBJECT
public:
    wmScopeDialog(QWidget *parent);
    void setSampleValues(int Channel, float *val, QString name);
    void clearChannelPointerList();
    void setChannelPointer(cDspMeasData *dspMeasData);
    void updateBoxItems();
    void setViewEnabled(bool bshow);
    bool isShowEnabled();
    cDspMeasData *getSelectedChannelPointer(int channel);
signals:
    void newCha0SelText(QString str);
    void newCha1SelText(QString str);
private:
    QList<cDspMeasData*> mDspMeasDataList;
    QHBoxLayout *mvlay;
    QVBoxLayout *mhlay;
    QComboBox *mChannel0, *mChannel1;
    wmSampleDialog *mSample;
    bool m_bShow;
private slots:
};

#endif // WMSCOPEDIALOG_H
