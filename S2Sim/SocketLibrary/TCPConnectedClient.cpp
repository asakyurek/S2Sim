/**
 * @file TCPConnectedClient.cpp
 * Implements the TCPConnectedClient class.
 *  @data Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "TCPConnectedClient.h"

TCPConnectedClient::TCPConnectedClient( const TSocketId socketId,
                                        IPAddress& clientAddress ) : SocketBase<SOCK_STREAM>( socketId ),
                                                                     m_clientAddress( clientAddress )
{
}

TCPConnectedClient::TCPConnectedClient( const TCPConnectedClient & copy ) : SocketBase<SOCK_STREAM>( copy ), m_clientAddress( copy.m_clientAddress )
{
}

TCPConnectedClient::~TCPConnectedClient( void )
{
}

TCPConnectedClient::TNumberOfBytes
TCPConnectedClient::SendData( const TBuffer buffer, const TNumberOfBytes length )
{
    return ( send( this->m_socketId, buffer, length, 0 ) );
}

TCPConnectedClient::TNumberOfBytes
TCPConnectedClient::ReceiveData( TBuffer buffer,
                                 const TNumberOfBytes receptionLength)
{
    return ( recv( this->m_socketId, buffer, receptionLength, 0 ) );
}

IPAddress
TCPConnectedClient::GetClientAddress( void ) const
{
    return ( this->m_clientAddress );
}
