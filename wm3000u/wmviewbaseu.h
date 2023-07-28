#ifndef WMVIEWBASEU
#define WMVIEWBASEU

#include "wmglobal.h"
#include "wmviewbase.h"

class WMViewBaseU : public WMViewBase
{
    Q_OBJECT
public:
    explicit WMViewBaseU( WMViewBase* parent = 0);
private:
    virtual void updateXRangeLabel(QLabel* xRangeLabel) override;
    WMViewBase* m_parent;
};
#endif
