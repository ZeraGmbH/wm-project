#ifndef SCPIDEVIFACE_H
#define SCPIDEVIFACE_H

#include <qstring.h>
#include <qstringlist.h>
#include "scpi.h"
#include "QDebug"

class cNode;

class scpideviface
{
public:
    scpideviface();
    QString GetInterface(cNode *RootCmd);
private:
    QString getChildInterface(cNode *node);
    QString getNextInterface(cNode *node);
    QStringList mSCPItreeItemName;

};

#endif // SCPIDEVIFACE_H
