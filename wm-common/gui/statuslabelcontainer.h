#ifndef STATUSLABELCONTAINER_H
#define STATUSLABELCONTAINER_H

#include "confdata.h"
#include <QStatusBar>
#include <QLabel>

class StatusLabelContainer
{
public:
    StatusLabelContainer(QStatusBar *statusBar);
    void updateLabels(cConfData *confData);
private:
    QStatusBar *m_statusBar;
    QLabel* m_pOwnErrTableLabel;
};

#endif // STATUSLABELCONTAINER_H
