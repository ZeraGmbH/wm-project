#ifndef STATUSLABELCONTAINER_H
#define STATUSLABELCONTAINER_H

#include "confdata.h"
#include <QStatusBar>
#include <QLabel>


class IStatusLabelXRangeParent
{
private:
    virtual void updateXRangeLabel(QLabel* xRangeLabel) = 0;
    friend class StatusLabelContainer;
};

class StatusLabelContainer
{
public:
    StatusLabelContainer(QStatusBar *statusBar, IStatusLabelXRangeParent* xRangeLabelParent);
    void updateLabels(cConfData *confData);
private:
    QStatusBar *m_statusBar;
    IStatusLabelXRangeParent* m_xRangeLabelParent;
    QLabel* m_pOwnErrTableLabel;
    QLabel* m_pResultFileLabel;
    QLabel* m_pRangeNLabel;
    QLabel* m_pRangeXLabel;
};

#endif // STATUSLABELCONTAINER_H
