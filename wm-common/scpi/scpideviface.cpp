#include "scpideviface.h"

scpideviface::scpideviface() {}


QString scpideviface::GetInterface(cNode* RootCmd)
{
    mInterface.clear();
    mSCPItreeItemName.clear();
    cNode* actNode = RootCmd;
    getChildInterface(actNode);
    return mInterface.join("\n");
}

void scpideviface::getChildInterface(cNode *node)
{
    cNodeSCPI *scpiNode = dynamic_cast<cNodeSCPI*>(node);
    if (scpiNode != 0)
        GetIChildntreface(scpiNode);
    cNodeSCPIVar *scpiNodeVar = dynamic_cast<cNodeSCPIVar*>(node);
    if (scpiNodeVar != 0)
        GetChildIntreface(scpiNodeVar);

}


void scpideviface::GetIChildntreface(cNodeSCPI *node)
{
    QString strNode = node->getNodeName();
    mSCPItreeItemName.append(strNode);
    GetChildInterfaceString(node, strNode);
}


void scpideviface::GetChildIntreface(cNodeSCPIVar *node)
{
    QStringList* list;
    list = node->getNoteNameList();
    foreach(QString str,*list){
        mSCPItreeItemName.append(str);
        GetChildInterfaceString(node, str);
        if(mSCPItreeItemName.last() == str)
            mSCPItreeItemName.removeLast();
    }
}

void scpideviface::GetChildInterfaceString(cNode *scpiNode, const QString nodeName)
{
    if(scpiNode->bIsQuery() || scpiNode->bIsCommand()){
        if(scpiNode->bIsQuery()){
            mInterface.append(mSCPItreeItemName.join(":") + "?");
        }
        if(scpiNode->bIsCommand()){
            mInterface.append(mSCPItreeItemName.join(":") + "!");
        }
        if(!scpiNode->bIsNode())
            mSCPItreeItemName.removeLast();
    }
    if (scpiNode->hasChild()){
        getChildInterface(scpiNode->getChild());
    }

    if (mSCPItreeItemName.length()>0){
        if (nodeName == mSCPItreeItemName.last())
            mSCPItreeItemName.removeLast();
    }

    if (scpiNode->hasNext()){
        getChildInterface(scpiNode->getNext());
    }
}


