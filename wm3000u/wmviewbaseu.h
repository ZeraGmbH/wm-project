#ifndef WMVIEWBASEU
#define WMVIEWBASEU

#include "wmglobal.h"
#include "wmviewbase.h"
#include "ui_wmviewbase.h"

class WMViewBaseU : public WMViewBase
{
    Q_OBJECT
public:
    explicit WMViewBaseU( WMViewBase* parent = 0);
    void ActualizeStates() override;
private:
    virtual void updateXRangeLabel(QLabel* xRangeLabel) override;
    WMViewBase* m_parent;
};
#endif
