#ifndef CCLIENTIODEVICE_H
#define CCLIENTIODEVICE_H

#include <QObject>

class cClientIODevice: public QObject // dies haben alle client io devices gemein, egal ob tcp/ip socket oder serial
{
    Q_OBJECT
public:
    cClientIODevice(){};
signals:
    void SendCommand( QString&); // der client hat ein kommando und sendet dieses
    void connectionClosed(cClientIODevice*); // der client meldet sich ab
public slots:
    virtual void ReceiveAnswer( QString&) = 0; // pure virtual
};

#endif // CCLIENTIODEVICE_H
