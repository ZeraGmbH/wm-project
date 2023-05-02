#include "deviceserver.h"

cDeviceServer::cDeviceServer(quint16 port)
{
    listen(QHostAddress::Any, port);
}


void cDeviceServer::CloseConnection(cClientIODevice* ciod)
{
    for(auto *delIFace :m_SCPIFacePtrList) {
        if (delIFace->GetClientIODevice() == ciod){
            m_SCPIFacePtrList.remove(delIFace);
            // delete crashes
            delIFace->deleteLater();
            break;
        }
    }
    if ( m_SCPIFacePtrList.isEmpty() ) // wenn wir keinen mehr haben
        emit RemoteCtrlInfo( false); // wird der remote betrieb aufgehoben
    setPriority(); // alle clients werden informiert
}

void cDeviceServer::setPriority() // alle clients werden informiert
{
    int i;
    for (i = 0; i < m_SCPIFacePtrList.count(); i++)
        m_SCPIFacePtrList.at(i)->ReceivePriority(i);
}

void cDeviceServer::incomingConnection(int socketDescriptor)
{
    newConnection(socketDescriptor);
}

