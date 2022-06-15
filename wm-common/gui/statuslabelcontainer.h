#ifndef STATUSLABELCONTAINER_H
#define STATUSLABELCONTAINER_H

#include "confdata.h"
#include <QStatusBar>
#include <QLabel>
#include <QTimer>

class IStatusLabelXRangeParent
{
private:
    virtual void updateXRangeLabel(QLabel* xRangeLabel) = 0;
    friend class StatusLabelContainer;
};

class StatusLabelContainer : public QObject
{
    Q_OBJECT
public:
    StatusLabelContainer(QStatusBar *statusBar, IStatusLabelXRangeParent* xRangeLabelParent);
    void updateLabels(cConfData *confData, bool adjusted, bool bFreqQuestionable);
private:
    QStatusBar *m_statusBar;
    IStatusLabelXRangeParent* m_xRangeLabelParent;
    QTimer m_iPPollTimer;
    QLabel* m_pOwnErrTableLabel;
    QLabel* m_pResultFileLabel;
    QLabel* m_pRangeNLabel;
    QLabel* m_pRangeXLabel;
    QLabel* m_pRunningLabel;
    QLabel* m_pSimulationLabel;
    QLabel* m_pStatusLabel;
    QLabel* m_pFreqLabel;
    QLabel* m_pIPLabel;
    QLabel* m_pDummyLabel;

private slots:
    void onIpPollTimer();
};

#endif // STATUSLABELCONTAINER_H
