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
    void getChildInterface(cNode *node);
    void GetIChildntreface(cNodeSCPI *node);
    void GetChildIntreface(cNodeSCPIVar *node);
    void GetChildInterfaceString(cNode *scpiNode, const QString nodeName);
    QStringList mSCPItreeItemName, mInterface;

};

#endif // SCPIDEVIFACE_H
