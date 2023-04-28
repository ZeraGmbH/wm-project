#ifndef CDEVICESERVER_H
#define CDEVICESERVER_H

#include "clientiodevice.h"
#include "scpiface.h"
#include <QObject>
#include <QTcpServer>
#include <Q3PtrList>

class cDeviceServer:  public QTcpServer // asynchroner device server pure virtual
{
    Q_OBJECT
public:
    cDeviceServer(quint16 port);
    virtual void newConnection(int socketDescriptor) = 0; // instanziiert ein client socket device und trägt es in liste ein

signals:
    void RemoteCtrlInfo(bool); // wir informieren ob remote control oder nicht

private slots:
    void CloseConnection(cClientIODevice*); // clients melden sich hierüber ab

protected:
    QList<cSCPIFace*> m_SCPIFacePtrList; // die liste aller angelegten interfaces
    void setPriority(); // alle clients werden informiert
    void incomingConnection(int socketDescriptor) override;
};

#endif // CDEVICESERVER_H
