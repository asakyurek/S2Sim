/*
 * IPAddress.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: asakyurek
 */

#include "IPAddress.h"

IPAddress::IPAddress( void )
{
    bzero( &this->m_address, sizeof( this->m_address ) );
    this->m_address.sin_family = AF_INET;
}

IPAddress::IPAddress( const IPAddress & copy )
{
    bcopy( &copy.m_address, &this->m_address, sizeof( this->m_address ) );
}

IPAddress::~IPAddress( void )
{
}

IPAddress&
IPAddress::operator = ( const IPAddress& rhs )
{
    if ( &( rhs ) == this )
    {
        return ( *this );
    }
    bcopy( &rhs.m_address, &this->m_address, sizeof( this->m_address ) );
    return ( *this );
}

bool
IPAddress::operator == ( const IPAddress & rhs ) const
{
    if ( ( this->GetPort() == rhs.GetPort() ) && ( this->GetAddress() == rhs.GetAddress() ) )
    {
        return ( true );
    }
    return ( false );
}

void
IPAddress::SetAddress( const TAddress address )
{
    this->GetAddress() = htonl( address );
}

IPAddress::TAddress&
IPAddress::GetAddress( void )
{
#if defined(_WIN32) || defined(_WIN64)
    return ( this->m_address.sin_addr.S_un.S_addr );
#elif defined( __linux__ ) || defined ( __APPLE__ )
    return ( this->m_address.sin_addr.s_addr );
#endif
}

IPAddress::TAddress
IPAddress::GetAddress( void ) const
{
#if defined(_WIN32) || defined(_WIN64)
    return ( ntohl( this->m_address.sin_addr.S_un.S_addr ) );
#elif defined( __linux__ ) || defined ( __APPLE__ )
    return ( ntohl( this->m_address.sin_addr.s_addr ) );
#endif
}

void
IPAddress::SetPort( const TPort port )
{
    this->GetPort() = htons( port );
}

IPAddress::TPort
IPAddress::GetPort( void ) const
{
    return ( ntohs( this->m_address.sin_port ) );
}

IPAddress::TPort&
IPAddress::GetPort( void )
{
    return ( this->m_address.sin_port );
}

IPAddress::operator sockaddr*( void )
{
    return ( ( sockaddr* )&this->m_address );
}
