/**
 * @file ThreadedTCPConnectedClient.cpp
 * Implements the ThreadedTCPConnectedClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPConnectedClient.h"

ThreadedTCPConnectedClient::ThreadedTCPConnectedClient( const TSocketId socketId, IPAddress & clientAddress ) : TCPConnectedClient( socketId, clientAddress ),
                                                                                                                m_notification( NULL ),
                                                                                                                m_started( true ),
                                                                                                                m_allowingSemaphore( 0 )
{
    this->StartThread( NULL );
}

ThreadedTCPConnectedClient::ThreadedTCPConnectedClient( const TCPConnectedClient & client ) : TCPConnectedClient( client ),
                                                                                              m_notification( NULL ),
                                                                                              m_started( true ),
                                                                                              m_allowingSemaphore( 0 )
{
    this->StartThread( NULL );
}

ThreadedTCPConnectedClient::~ThreadedTCPConnectedClient( void ) throw()
{
    this->m_started = false;
}

void*
ThreadedTCPConnectedClient::ExecutionBody( void* input )
{
    while ( this->m_started )
    {
        this->m_allowingSemaphore.TakeSemaphore();
        const TNumberOfBytes bufferSize = 100000;
        char buffer[bufferSize];
        memset( buffer, 0x00, bufferSize );
        TNumberOfBytes receivedBytes = this->ReceiveData( buffer, bufferSize );
        this->m_notification( this, buffer, receivedBytes );
        this->m_allowingSemaphore.ReleaseSemaphore();
    }
    return ( NULL );
}
