#include "deviceserver.h"

cDeviceServer::cDeviceServer(quint16 port) :
    Q3ServerSocket(port, 1, nullptr, nullptr)
{
}


void cDeviceServer::CloseConnection(cClientIODevice* ciod)
{
    int i;
    cSCPIFace* delIFace;
    for (i = 0;;i++) // wir suchen nach dem interface dessen client io device sich abgemeldet hat
    if ( m_SCPIFacePtrList.at(i)->GetClientIODevice() == ciod )
        break;

    delIFace = m_SCPIFacePtrList.at(i);
    m_SCPIFacePtrList.remove(delIFace); // entfernen es aus der liste
    delete delIFace; // und löschen es

    if ( m_SCPIFacePtrList.isEmpty() ) // wenn wir keinen mehr haben
    emit RemoteCtrlInfo( false); // wird der remote betrieb aufgehoben
    setPriority(); // alle clients werden informiert
}


void cDeviceServer::setPriority() // alle clients werden informiert
{
    uint i;
    for (i = 0; i < m_SCPIFacePtrList.count(); i++)
    m_SCPIFacePtrList.at(i)->ReceivePriority(i);
}

