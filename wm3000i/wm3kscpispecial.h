#ifndef WM3KSCPISPECIAL_H
#define WM3KSCPISPECIAL_H

#include "wmglobal.h"
#include <QString>

class WM3kSCPISpecial
{
public:
    WM3kSCPISpecial();
    QString getExTName();
    QString getMaxRange();
    QString getConfOperModeCatalog();
};

#endif // WM3KSCPISPECIAL_H
