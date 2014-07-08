/**
 * @file ThreadedTCPServer.cpp
 * Implements the ThreadedTCPServer class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPServer.h"

ThreadedTCPServer::ThreadedTCPServer( void ) : m_notification( nullptr ), m_started( false )
{
}

ThreadedTCPServer::~ThreadedTCPServer( void ) throw()
{
    this->m_started = false;
}

void
ThreadedTCPServer::ExecutionBody( void )
{
    while ( this->m_started )
    {
        ThreadedTCPConnectedClient* connectedClient = this->Accept();
        this->m_notification( connectedClient );
    }
}

void
ThreadedTCPServer::SetNotificationCallback( TNotification notification )
{
    LOG_FUNCTION_START();
    if ( this->m_notification != nullptr && this->m_started )
    {
        ErrorPrint( "Multiple notification callbacks not allowed" );
    }
    this->m_notification = notification;
    if ( this->m_notification != nullptr )
    {
        this->m_started = true;
        this->Listen();
        this->m_thread = std::thread( &ThreadedTCPServer::ExecutionBody, this );
        this->m_thread.detach();
    }
    LOG_FUNCTION_END();
}
