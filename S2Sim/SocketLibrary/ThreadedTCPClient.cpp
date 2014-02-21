/**
 * @file ThreadedTCPClient.cpp
 * Implements the ThreadedTCPClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPClient.h"

ThreadedTCPClient::ThreadedTCPClient( void ) : m_notification( NULL ), m_started( true ), m_allowingSemaphore( 0 )
{
    this->StartThread( NULL );
}

ThreadedTCPClient::ThreadedTCPClient( const TCPClient & client ) : TCPClient( client ), m_notification( NULL ), m_started( true ), m_allowingSemaphore( 0 )
{
    this->StartThread( NULL );
}

ThreadedTCPClient::~ThreadedTCPClient( void ) throw()
{
    this->m_started = false;
}

void*
ThreadedTCPClient::ExecutionBody( void* input )
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
