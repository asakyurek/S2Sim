/*
 * SocketBase.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Alper
 */

#ifndef SOCKETBASE_H_
#define SOCKETBASE_H_

#include <iostream>

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

template <int SocketType>
class SocketBase
{
    public:
        /* Type Definitions */
        typedef unsigned int TErrorCode;
        enum ErrorCodeValues
        {
            NoError = 0
        };

        typedef size_t TNumberOfBytes;
        typedef void* TBuffer;

    public:
        /* Function Definitions */

    public:
        /* Member Definitions */

    protected:
        /* Type Definitions */
        typedef int SOCKET;
        typedef SOCKET TSocketId;

        enum SocketIdValues
        {
            InvalidSocketId = ( TSocketId )( -1 )
        };

    protected:
        /* Function Definitions */
        SocketBase( const TSocketId socketId ) : m_socketId( socketId ){}
        SocketBase( const SocketBase<SocketType> & copy ) : m_socketId( copy.m_socketId ){}
        SocketBase( void ) throw ( TErrorCode );
        ~SocketBase( void ) throw ( TErrorCode );

    protected:
        /* Member Definitions */
        TSocketId m_socketId;

    private:
        /* Type Definitions */

    private:
        /* Function Definitions */
        void
        OpenSocket( void ) throw ( TErrorCode );

        void
        CloseSocket( void ) throw ( TErrorCode );

    private:
        /* Member Definitions */
};

template <int SocketType>
SocketBase<SocketType>::SocketBase( void ) throw ( TErrorCode ) : m_socketId( InvalidSocketId )
{
    this->OpenSocket();
}

template <int SocketType>
SocketBase<SocketType>::~SocketBase( void ) throw ( TErrorCode )
{
    this->CloseSocket();
}

template <int SocketType>
void
SocketBase<SocketType>::OpenSocket( void ) throw ( TErrorCode )
{
    this->m_socketId = socket( AF_INET, SocketType, 0 );
    if ( this->m_socketId <= 0 )
    {
        std::cerr << "Invalid Socket" << std::endl;
    }
    int value = 1;
    if ( 0 != setsockopt( this->m_socketId, SOL_SOCKET, SO_REUSEADDR, &value, sizeof( value ) ) )
    {
        std::cerr << "Socket Option Not Set! Error: " << errno << std::endl;
    }
    if ( 0 != setsockopt( this->m_socketId, SOL_SOCKET, SO_REUSEPORT, &value, sizeof( value ) ) )
    {
        std::cerr << "Socket Option Not Set! Error: " << errno << std::endl;
    }
}

template <int SocketType>
void
SocketBase<SocketType>::CloseSocket( void ) throw ( TErrorCode )
{
#if defined(_WIN32) || defined(_WIN64)
    closesocket( this->m_socketId );
#elif defined( __linux__ ) || defined ( __APPLE__ )
    std::cout << "Socket Closed" << std::endl;
    close( this->m_socketId );
#endif
}

#endif /* SOCKETBASE_H_ */
