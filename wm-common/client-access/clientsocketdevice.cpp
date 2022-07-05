#include "clientsocketdevice.h"

cClientSocketDevice::cClientSocketDevice(int sock)
{
    m_socket.setSocket(sock);
    m_pZDSWriteNotifyer = new QSocketNotifier ( sock, QSocketNotifier::Write);
    m_pZDSWriteNotifyer->setEnabled (false);
    connect(m_pZDSWriteNotifyer,SIGNAL(activated(int)),this,SLOT(SendAnswer(int)));
    connect( &m_socket, SIGNAL(connectionClosed()), SLOT(CloseConnection()) );
    connect( &m_socket, SIGNAL(readyRead()), SLOT(ReadCommand()) );
}


cClientSocketDevice::~cClientSocketDevice()
{
    delete m_pZDSWriteNotifyer;
}


void cClientSocketDevice::CloseConnection()
{
    emit connectionClosed(this);
}


void cClientSocketDevice::ReadCommand()
{
    m_sInput = "";
    while ( m_socket.canReadLine() )
    {
        m_sInput = m_socket.readLine();
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
    m_socket.writeBlock(m_sOutput.latin1(), m_sOutput.length());
}
