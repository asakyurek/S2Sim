/*
 * IPAddress.h
 *
 *  Created on: Jan 21, 2014
 *      Author: asakyurek
 */

#ifndef IPADDRESS_H_
#define IPADDRESS_H_

#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#elif defined( __linux__ ) || defined ( __APPLE__ )
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#endif

class IPAddress
{
    private:
        typedef sockaddr_in TAddressStruct;

    public:
        typedef unsigned int TAddress;
        typedef unsigned short TPort;
        typedef socklen_t TSocketSize;

    private:
        TAddressStruct m_address;

    private:
        TAddress&
        GetAddress( void );

        TPort&
        GetPort( void );

    public:
        IPAddress( void );
        IPAddress( const IPAddress & copy );
        ~IPAddress( void );

        IPAddress&
        operator = ( const IPAddress & rhs );

        bool
        operator == ( const IPAddress & rhs ) const;

        void
        SetAddress( const TAddress address );

        TAddress
        GetAddress( void ) const;

        void
        SetPort( const TPort port );

        TPort
        GetPort( void ) const;

        operator sockaddr* ( void );

        static TSocketSize
        GetSocketSize( void )
        {
            return ( sizeof( sockaddr_in ) );
        }
};

#endif /* IPADDRESS_H_ */
