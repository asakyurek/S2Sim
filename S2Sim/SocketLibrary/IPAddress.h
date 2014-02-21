/**
 * @file IPAddress.h
 * Defines the IPAddress class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
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

/**
 *  This class is an abstraction of the OS IP Address structure. Byte order conversion is handled automatically.
 */
class IPAddress
{
    private:
    /**
     *  BSD Socket structure.
     */
        typedef sockaddr_in TAddressStruct;

    public:
    /**
     *  IP Address value.
     */
        typedef unsigned int TAddress;
    
    /**
     *  Port value.
     */
        typedef unsigned short TPort;
    
    /**
     *  Size of the socket structure.
     */
        typedef socklen_t TSocketSize;

    private:
    /**
     *  OS Address structure to be abstracted.
     */
        TAddressStruct m_address;

    private:
    /**
     *  Returns a reference to the address value.
     *
     *  @return Reference to the address field.
     */
        TAddress&
        GetAddress( void );

    /**
     *  Return a reference to the port value.
     *
     *  @return Reference to the port field.
     */
        TPort&
        GetPort( void );

    public:
    /**
     *  Sets the address family and clears the structure.
     */
        IPAddress( void );
    
    /**
     *  Binary copy of the structure.
     
        @param copy Address to be copied.
     */
        IPAddress( const IPAddress & copy );
    
    /**
     *  No use.
     */
        ~IPAddress( void );

    /**
     *  Assignment operator for binary copy.
     *
     *  @param rhs Right hand side of (=).
     *
     *  @return Returns a reference to itself.
     */
        IPAddress&
        operator = ( const IPAddress & rhs );

    /**
     *  Checks whether the addresses and ports are the same.
     *
     *  @param rhs Right hand size of (==).
     *
     *  @return Returns the equality between the classes.
     */
        bool
        operator == ( const IPAddress & rhs ) const;

    /**
     *  Sets the address field of the structure.
     *
     *  @param address 4 byte IP Address.
     */
        void
        SetAddress( const TAddress address );

    /**
     *  Reads the address field of the structure.
     *
     *  @return 4 byte IP Address.
     */
        TAddress
        GetAddress( void ) const;

    /**
     *  Sets the port field of the structure.
     *
     *  @param port 2 byte port value.
     */
        void
        SetPort( const TPort port );

    /**
     *  Reads the port field of the structure.
     *
     *  @return 2 byte port value.
     */
        TPort
        GetPort( void ) const;

    /**
     *  Returns the cast version of the address structure to be used in OS utilities.
     *
     *  @return Address of the internal structure.
     */
        operator sockaddr* ( void );

    /**
     *  Returns the size of the socket structure.
     *
     *  @return Size of the socket structure.
     */
        static TSocketSize
        GetSocketSize( void )
        {
            return ( sizeof( sockaddr_in ) );
        }
};

#endif /* IPADDRESS_H_ */
