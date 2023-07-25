#ifndef WMVIEWBASEI
#define WMVIEWBASEI

#include "wmglobal.h"
#include "wmviewbase.h"
#include "ui_wmviewbase.h"

class WMViewBaseI : public WMViewBase
{
public:
    explicit WMViewBaseI( WMViewBase* parent = 0);
    Q_OBJECT
private:
    virtual void updateXRangeLabel(QLabel* xRangeLabel) override;
    WMViewBase* m_parent;
};
#endif
