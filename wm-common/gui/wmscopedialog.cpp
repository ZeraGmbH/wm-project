#include "wmscopedialog.h"

wmScopeDialog::wmScopeDialog(QWidget *parent)
    : QDialog{parent}
{
    mvlay = new QHBoxLayout;
    mChannel0 = new QComboBox;
    mChannel1 = new QComboBox;
    mvlay->addWidget(mChannel0);
    mvlay->addWidget(mChannel1);
    mhlay = new QVBoxLayout;
    mhlay->addItem(mvlay);
    mSample = new wmSampleDialog;
    mhlay->addWidget(mSample);
    setLayout(mhlay);
    connect(mChannel0,SIGNAL(currentIndexChanged()),this,SLOT(channel0Changed()));
    connect(mChannel1,SIGNAL(currentIndexChanged()),this,SLOT(channel1Changed()));
}

void wmScopeDialog::setComboBoxItems(int Channel, QStringList list)
{
    if (Channel == 0 )
        mChannel0->insertItems(0,list);
    if (Channel == 1 )
        mChannel1->insertItems(0,list);

}

void wmScopeDialog::setSampleValues(int Channel, float *val, QString name)
{
    mSample->setSingalProperties(name,Channel);
    mSample->setSampleValues(val,Channel);
}

void wmScopeDialog::channel0Changed()
{
    emit newCha0SelText(mChannel0->currentText());
    mSample->setSignalNameCh0(mChannel0->currentText());
}

void wmScopeDialog::channel1Changed()
{
    emit newCha1SelText(mChannel1->currentText());
    mSample->setSignalNameCh1(mChannel1->currentText());
}

