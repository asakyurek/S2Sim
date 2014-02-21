/**
 * @file ThreadedTCPServer.cpp
 * Implements the ThreadedTCPServer class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "ThreadedTCPServer.h"

ThreadedTCPServer::ThreadedTCPServer( void ) : m_notification( NULL ), m_started( false )
{
}

ThreadedTCPServer::~ThreadedTCPServer( void ) throw()
{
    this->m_started = false;
}

void*
ThreadedTCPServer::ExecutionBody( void* input )
{
    while ( this->m_started )
    {
        ThreadedTCPConnectedClient* connectedClient = this->Accept();
        this->m_notification( connectedClient );
    }
    return ( NULL );
}
