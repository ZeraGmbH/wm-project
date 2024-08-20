#include "scpideviface.h"

scpideviface::scpideviface() {}


QString scpideviface::GetInterface(cNode* RootCmd)
{
    QString str, temp;
    cNodeSCPI* actNode = static_cast<cNodeSCPI*>(RootCmd);
    mSCPItreeItemName.clear();
    temp = actNode->getNodeName();

  //  while (actNode->bIsNode()){
        str = str.append(getChildInterface(actNode));
        qDebug() << str;
        if(actNode->hasNext()){
            actNode = actNode->getNext();
        }
        else {
            return str;
        }
    //}
    return str;
}

QString scpideviface::getChildInterface(cNode *node)
{
    QString str, strNode;
    cNodeSCPI *scpiNode = static_cast<cNodeSCPI*>(node);
    strNode = scpiNode->getNodeName();
    mSCPItreeItemName.append(strNode);

    if(scpiNode->bIsQuery() || scpiNode->bIsCommand()){
        if(scpiNode->bIsQuery()){
            str = str.append(mSCPItreeItemName.join(":") + "?\n");
        }
        if(scpiNode->bIsCommand()){
            str = str.append(mSCPItreeItemName.join(":") + "!\n");
        }
        if(!scpiNode->bIsNode())
            mSCPItreeItemName.removeLast();
    }
    //if(scpiNode->bIsNode())
    //      str = str.append(getNextInterface(scpiNode->getNext()));

 //   }

    if (scpiNode->hasChild()){
        str = str.append(getChildInterface(scpiNode->getChild()));
    }

    if (strNode == mSCPItreeItemName.last())
        mSCPItreeItemName.removeLast();

    if (scpiNode->hasNext()){
        str = str.append(getChildInterface(scpiNode->getNext()));
    }

    return str;
}

QString scpideviface::getNextInterface(cNode *node)
{
    QString str;
    cNodeSCPI *scpiNode = static_cast<cNodeSCPI*>(node);
    str = str.append(scpiNode->getNodeName());
    if(scpiNode->hasChild()){
        str = str.append(":" + getChildInterface(scpiNode->getChild()));
    }
    if(scpiNode->hasNext()){
        str = str.append(":" + getChildInterface(scpiNode->getNext()));
    }
    return str;
}

