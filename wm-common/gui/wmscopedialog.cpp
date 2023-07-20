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
    m_bShow = false;
}

void wmScopeDialog::setSampleValues(int Channel, float *val, QString name)
{
    mSample->setSingalProperties(name,Channel);
    mSample->setSampleValues(val,Channel);
}

void wmScopeDialog::clearChannelPointerList()
{
    mDspMeasDataList.clear();
}

void wmScopeDialog::setChannelPointer(cDspMeasData<float> *dspMeasData)
{
    mDspMeasDataList.append(dspMeasData);
}

void wmScopeDialog::updateBoxItems()
{
    mChannel0->clear();
    mChannel1->clear();
    foreach (cDspMeasData<float>* poi, mDspMeasDataList ) {
        mChannel0->insertItem(poi->VarList());
        mChannel1->insertItem(poi->VarList());
    }
}

void wmScopeDialog::setViewEnabled(bool bshow)
{
    m_bShow = bshow;
}

bool wmScopeDialog::isShowEnabled()
{
    return !m_bShow;
}

cDspMeasData<float> *wmScopeDialog::getSelectedChannelPointer(int channel)
{
    cDspMeasData<float> * poi;

    if (channel == 0 ) {
        poi = mDspMeasDataList.at(mChannel0->currentItem());
        mSample->setSignalNameCh0(poi->MeasVarList());
    }
    if (channel == 1 ) {
        poi = mDspMeasDataList.at(mChannel1->currentItem());
        mSample->setSignalNameCh1(poi->MeasVarList());
    }
   return poi;
}

void wmScopeDialog::closeEvent(QCloseEvent *ce)
{
    m_bShow = false;
    emit sigIsVisible(false);
    //ce->accept();
}

