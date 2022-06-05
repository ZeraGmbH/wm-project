#include "clientsocketdevice.h"

cClientSocketDevice::cClientSocketDevice(int sock, QObject *parent, const char*  name)
{
    m_pSock = new Q3Socket(parent, name);
    m_pSock->setSocket(sock);
    m_pZDSWriteNotifyer = new QSocketNotifier ( sock, QSocketNotifier::Write);
    m_pZDSWriteNotifyer->setEnabled (false);
    connect(m_pZDSWriteNotifyer,SIGNAL(activated(int)),this,SLOT(SendAnswer(int)));
    connect( m_pSock, SIGNAL(connectionClosed()), SLOT(CloseConnection()) );
    connect( m_pSock, SIGNAL(readyRead()), SLOT(ReadCommand()) );
}


cClientSocketDevice::~cClientSocketDevice()
{
    delete m_pSock;
    delete m_pZDSWriteNotifyer;
}


void cClientSocketDevice::CloseConnection()
{
    emit connectionClosed(this);
}


void cClientSocketDevice::ReadCommand()
{
    m_sInput = "";
    while ( m_pSock->canReadLine() )
    {
        m_sInput = m_pSock->readLine();
        m_sInput.remove('\r');
        m_sInput.remove('\n');
        emit SendCommand( m_sInput);
    }
}


void cClientSocketDevice::ReceiveAnswer( QString& s)
{
    m_sOutput = s + "\n"; // setzen wir den
    m_pZDSWriteNotifyer->setEnabled (true);
}


void cClientSocketDevice::SendAnswer(int)
{
    m_pZDSWriteNotifyer->setEnabled(false);
    m_pSock->writeBlock(m_sOutput.latin1(), m_sOutput.length());
}
