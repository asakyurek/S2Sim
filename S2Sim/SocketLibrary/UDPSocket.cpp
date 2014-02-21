/**
 * @file UDPSocket.cpp
 * Implements the UDPSocket class.
 *  @date Jun 21, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "UDPSocket.h"

UDPSocket::UDPSocket( void )
{
}

UDPSocket::~UDPSocket( void )
{
}

UDPSocket::TNumberOfBytes
UDPSocket::SendData( const TBuffer buffer, const TNumberOfBytes length, IPAddress & destinationAddress )
{
    return ( sendto( this->m_socketId, buffer, length, 0, ( struct sockaddr* )destinationAddress, IPAddress::GetSocketSize() ) );
}

UDPSocket::TNumberOfBytes
UDPSocket::ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength, IPAddress & destinationAddress )
{
    IPAddress::TSocketSize socketSize = IPAddress::GetSocketSize();
    return ( recvfrom( this->m_socketId, buffer, receptionLength, 0, ( struct sockaddr* )destinationAddress, &socketSize ) );
}

void
UDPSocket::SetIPAddress( IPAddress & address )
{
    bind( this->m_socketId, ( struct sockaddr* )address, IPAddress::GetSocketSize() );
}

void
UDPSocket::SetPort( const IPAddress::TPort port )
{
    IPAddress address;
    address.SetAddress( INADDR_ANY );
    address.SetPort( port );
    this->SetIPAddress( address );
}
