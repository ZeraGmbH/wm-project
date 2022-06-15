#ifndef CDEVICESERVER_H
#define CDEVICESERVER_H

#include "clientiodevice.h"
#include "scpiface.h"
#include <QObject>
#include <Q3ServerSocket>
#include <Q3PtrList>

class cDeviceServer:  public Q3ServerSocket // asynchroner device server pure virtual
{
    Q_OBJECT

public:
    cDeviceServer(quint16 port);
    virtual void newConnection(int) = 0; // instanziiert ein client socket device und trägt es in liste ein

signals:
    void RemoteCtrlInfo(bool); // wir informieren ob remote control oder nicht

private slots:
    void CloseConnection(cClientIODevice*); // clients melden sich hierüber ab

protected:
    Q3PtrList<cSCPIFace> m_SCPIFacePtrList; // die liste aller angelegten interfaces
    void setPriority(); // alle clients werden informiert
};

#endif // CDEVICESERVER_H
