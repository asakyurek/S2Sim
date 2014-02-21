/**
 * @file ConnectionManager.cpp
 * This file implements the ConnectionManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "ConnectionManager.h"

ConnectionManager &
GetConnectionManager( void )
{
    static ConnectionManager connectionManager;
    return ( connectionManager );
}

void
ConnectionNotificationHandler( ThreadedTCPConnectedClient* acceptedClient )
{
    LOG_FUNCTION_START();
    LogPrint( "New Client Connection, notifying the connection manager" );
    GetConnectionManager().IncomingConnection( acceptedClient );
    acceptedClient->SetNotificationCallback( &ConnectionReceiveHandler );
    LOG_FUNCTION_END();
}

void
ConnectionReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size )
{
    LOG_FUNCTION_START();
    LogPrint( "New Client Data with size ", size );
    if ( size <= 0 )
    {
        WarningPrint( "Client Connection broken, notifying Connection manager" );
        GetConnectionManager().BrokenConnection( client );
    }
    else
    {
        LogPrint( "Relaying data to Connection manager" );
        GetConnectionManager().IncomingMessage( client, buffer, size );
    }
    LOG_FUNCTION_END();
}

ConnectionManager::ConnectionManager( void )
{
    LOG_FUNCTION_START();
    this->m_server.SetPort( 26999 );
    this->m_server.SetNotificationCallback( &ConnectionNotificationHandler );
    LOG_FUNCTION_END();
}

void
ConnectionManager::IncomingConnection( ThreadedTCPConnectedClient* newClient )
{
    LOG_FUNCTION_START();
    LogPrint( "Creating a client manager for the new connection" );
    ClientManager* newClientManager = new ClientManager( newClient );
    LogPrint( "Adding client to the client lists" );
    this->m_clientList.insert( std::make_pair( newClient, newClientManager ) );
    this->m_reversedClientList.insert( std::make_pair( newClientManager, newClient ) );
    LOG_FUNCTION_END();
}

void
ConnectionManager::BrokenConnection( ThreadedTCPConnectedClient* brokenClient )
{
    LOG_FUNCTION_START();
    WarningPrint( "Client connection is broken, notifying the client manager" );
    this->m_clientList[brokenClient]->ConnectionBroken();
    LOG_FUNCTION_END();
}

void
ConnectionManager::IncomingMessage( ThreadedTCPConnectedClient* senderClient, void* buffer, size_t size )
{
    LOG_FUNCTION_START();
    LogPrint( "Client data received of size ", size, ". Relaying to client manager" );
    this->m_clientList[senderClient]->MessageReceived( buffer, size );
    LOG_FUNCTION_END();
}

void
ConnectionManager::DeleteClient( ClientManager* clientManager )
{
    LOG_FUNCTION_START();
    LogPrint( "Deleting Client from the lists" );
    this->m_clientList.erase( this->m_reversedClientList[clientManager] );
    this->m_reversedClientList.erase( clientManager );
    LogPrint( "Deleting Client Manager" );
    delete clientManager;
    LOG_FUNCTION_END()
}
