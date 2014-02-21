/**
 * @file TCPServer.cpp
 * Implements the TCPServer class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "TCPServer.h"

TCPServer::TCPServer( void )
{
}

TCPServer::~TCPServer( void )
{
}

void
TCPServer::SetAddress( IPAddress& address )
{
    if ( -1 == bind( this->m_socketId, ( struct sockaddr* )address, IPAddress::GetSocketSize() ) )
    {
        std::cerr << "Socket Bind failed! Error: " << errno << std::endl;
    }
}

void
TCPServer::SetPort( const IPAddress::TPort port )
{
    IPAddress address;
    address.SetAddress( INADDR_ANY );
    address.SetPort( port );
    this->SetAddress( address );
}

bool
TCPServer::Listen( void )
{
    if ( listen( this->m_socketId, 1 ) == 0 )
    {
        return ( true );
    }
    std::cerr << "Socket Listen failed! Error: " << errno << std::endl;
    return ( false );
}

ThreadedTCPConnectedClient*
TCPServer::Accept( void )
{
    IPAddress address;
    IPAddress::TSocketSize length = IPAddress::GetSocketSize();
    TSocketId acceptedSocketId = accept( this->m_socketId, ( struct sockaddr* )address, &length );
    if ( acceptedSocketId <= 0 )
    {
        std::cerr << "Socket Accept failed! Error: " << errno << std::endl;
    }
    return ( new ThreadedTCPConnectedClient( acceptedSocketId, address ) );
}
