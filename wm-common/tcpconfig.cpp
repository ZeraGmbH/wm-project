#include "tcpconfig.h"
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>


QStringList getIPv4AddressList()
{
    QStringList listAddresses;
    for(auto address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
            listAddresses.append(address.toString());
        }
    }
    return listAddresses;
}

