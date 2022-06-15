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
    StatusLabelContainer(IStatusLabelXRangeParent* xRangeLabelParent);
    void updateLabels(cConfData *confData, bool adjusted, bool bFreqQuestionable);
    void CreateStatusBar(QStatusBar *statusBar, QWidget* labelParent);
private:
    IStatusLabelXRangeParent* m_xRangeLabelParent;
    QTimer m_iPPollTimer;
    QLabel* m_pOwnErrTableLabel = nullptr;
    QLabel* m_pResultFileLabel = nullptr;
    QLabel* m_pRangeNLabel = nullptr;
    QLabel* m_pRangeXLabel = nullptr;
    QLabel* m_pRunningLabel = nullptr;
    QLabel* m_pSimulationLabel = nullptr;
    QLabel* m_pStatusLabel = nullptr;
    QLabel* m_pFreqLabel = nullptr;
    QLabel* m_pIPLabel = nullptr;
    QLabel* m_pDummyLabel = nullptr;

private slots:
    void onIpPollTimer();
};

#endif // STATUSLABELCONTAINER_H
