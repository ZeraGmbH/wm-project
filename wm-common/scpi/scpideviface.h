#ifndef SCPIDEVIFACE_H
#define SCPIDEVIFACE_H

#include <qstring.h>
#include <qstringlist.h>
#include "scpi.h"
#include <QFile>
#include <QDir>

#define DeviceInterfaceFilePath QDir::homePath()+"/wm3000deviface.txt"

class cNode;

class scpideviface
{
public:
    scpideviface();
    QString GetInterface(cNode *RootCmd);
    bool exportToFile();
private:
    void getChildInterface(cNode *node);
    void GetIChildntreface(cNodeSCPI *node);
    void GetChildIntreface(cNodeSCPIVar *node);
    void GetChildInterfaceString(cNode *scpiNode, const QString nodeName);
    QStringList mSCPItreeItemName, mInterface;
    QFile mDeviceInterfaceFile;

};

#endif // SCPIDEVIFACE_H
