#ifndef CCLIENTSOCKETDEVICE_H
#define CCLIENTSOCKETDEVICE_H

#include "clientiodevice.h"
#include <QObject>
#include <Q3Socket>
#include <QSocketNotifier>

class cClientSocketDevice : public cClientIODevice
{
    Q_OBJECT

public:
    cClientSocketDevice(int sock, QObject *parent = 0, const char*  name = 0);
    ~cClientSocketDevice();

public slots:
    virtual void ReceiveAnswer( QString&);

private slots:
    void ReadCommand(); // wenn daten da sind
    void SendAnswer( int); // von notifier aktiviert mit int fd als parameter
    void CloseConnection();

private:
    Q3Socket* m_pSock;
    QSocketNotifier* m_pZDSWriteNotifyer;
    QString m_sInput;
    QString m_sOutput;
};

#endif // CCLIENTSOCKETDEVICE_H
