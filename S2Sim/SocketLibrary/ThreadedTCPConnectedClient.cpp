/**
 * @file ThreadedTCPConnectedClient.cpp
 * Implements the ThreadedTCPConnectedClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPConnectedClient.h"

ThreadedTCPConnectedClient::ThreadedTCPConnectedClient( const TSocketId socketId, IPAddress & clientAddress ) : TCPConnectedClient( socketId, clientAddress ),
                                                                                                                m_notification( nullptr ),
                                                                                                                m_started( true )
{
    LOG_FUNCTION_START();
    this->m_allowingMutex.lock();
    LOG_FUNCTION_END();
}

ThreadedTCPConnectedClient::ThreadedTCPConnectedClient( const TCPConnectedClient & client ) : TCPConnectedClient( client ),
                                                                                              m_notification( nullptr ),
                                                                                              m_started( true )
{
    LOG_FUNCTION_START();
    this->m_allowingMutex.lock();
    LOG_FUNCTION_END();
}

ThreadedTCPConnectedClient::~ThreadedTCPConnectedClient( void )
{
    LOG_FUNCTION_START();
    this->m_started = false;
    this->m_allowingMutex.lock();
    std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
    LOG_FUNCTION_END();
}

void
ThreadedTCPConnectedClient::ExecutionBody( void )
{
    LOG_FUNCTION_START();
    while ( this->m_started )
    {
        const TNumberOfBytes bufferSize = 100000;
        char buffer[bufferSize];
        memset( buffer, 0x00, bufferSize );
        TNumberOfBytes receivedBytes = this->ReceiveData( buffer, bufferSize );
        if ( this->m_notification != nullptr )
        {
            this->m_notification( this, buffer, receivedBytes );
        }
    }
    LOG_FUNCTION_END();
    this->m_allowingMutex.unlock();
}

void
ThreadedTCPConnectedClient::SetNotificationCallback( TNotification notification )
{
    LOG_FUNCTION_START();
    if ( this->m_notification != nullptr )
    {
        ErrorPrint( "Notification Callback can not be changed!" );
    }
    this->m_notification = notification;
    if ( this->m_notification != nullptr )
    {
        this->m_thread = std::thread( &ThreadedTCPConnectedClient::ExecutionBody, this );
        this->m_thread.detach();
    }
    LOG_FUNCTION_END();
}

void
ThreadedTCPConnectedClient::StopThread( void )
{
    LOG_FUNCTION_START();
    this->m_started = false;
    LOG_FUNCTION_END();
}
