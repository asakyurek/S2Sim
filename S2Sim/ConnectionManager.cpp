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
    this->m_listProtectionMutex.lock();
    this->m_clientList.insert( std::make_pair( newClient, newClientManager ) );
    this->m_reversedClientList.insert( std::make_pair( newClientManager, newClient ) );
    this->m_keepAliveMap.insert( std::make_pair( newClientManager->GetClientId(), 0) );
    this->m_listProtectionMutex.unlock();
    LOG_FUNCTION_END();
}

void
ConnectionManager::BrokenConnection( ThreadedTCPConnectedClient* brokenClient )
{
    LOG_FUNCTION_START();
    WarningPrint( "Client connection is broken, notifying the client manager" );
    if ( this->m_clientList.find( brokenClient ) != this->m_clientList.end() )
    {
        this->m_clientList[brokenClient]->ConnectionBroken();
    }
    else
    {
        WarningPrint( "Client already deleted" );
    }
    LOG_FUNCTION_END();
}

void
ConnectionManager::IncomingMessage( ThreadedTCPConnectedClient* senderClient, void* buffer, size_t size )
{
    LOG_FUNCTION_START();
    LogPrint( "Client data received of size ", size, ". Relaying to client manager" );
    if ( this->m_clientList.find( senderClient ) != this->m_clientList.end() )
    {
        this->m_clientList[senderClient]->MessageReceived( buffer, size );
    }
    LOG_FUNCTION_END();
}

void
ConnectionManager::DeleteClient( TClientManagerPointer clientManager )
{
    LOG_FUNCTION_START();
    LogPrint( "Deleting Client from the lists" );
    this->m_listProtectionMutex.lock();
    ThreadedTCPConnectedClient* clientThread = this->m_reversedClientList[clientManager];
    std::thread clientKillingThread( &ClientManager::StopClientAndClean, clientManager );
    clientKillingThread.detach();
    this->m_clientList.erase( clientThread );
    this->m_reversedClientList.erase( clientManager );
    this->m_keepAliveMap.erase( clientManager->GetClientId() );
    LogPrint( "Deleting Client Manager" );
    this->m_listProtectionMutex.unlock();
    LOG_FUNCTION_END();
}

ConnectionManager::TNumberOfClients
ConnectionManager::GetNumberOfSynchronousClients( void )
{
    LOG_FUNCTION_START();
    this->m_listProtectionMutex.lock();
    TNumberOfClients numberOfSynchronousClients = 0;
    for ( auto clientIterator = this->m_clientList.begin(); clientIterator != this->m_clientList.end(); ++clientIterator )
    {
        if ( clientIterator->second->IsSynchronous() )
        {
            ++numberOfSynchronousClients;
        }
    }
    this->m_listProtectionMutex.unlock();
    LOG_FUNCTION_END();
    return ( numberOfSynchronousClients );
}

void
ConnectionManager::PrintClientList( void )
{
    LOG_FUNCTION_START();
    this->m_listProtectionMutex.lock();
    std::ofstream outputFile( "clientList.txt", std::ios::out );
    for ( auto clientIterator = this->m_clientList.begin(); clientIterator != this->m_clientList.end(); ++clientIterator )
    {
        clientIterator->second->PrintClientInformation( outputFile );
    }
    outputFile.close();
    this->m_listProtectionMutex.unlock();
    LOG_FUNCTION_END();
}

void
ConnectionManager::UpdateAliveClients( const std::vector<TClientId> & aliveClients )
{
    LOG_FUNCTION_START();
    this->m_listProtectionMutex.lock();
    std::vector<TClientManagerPointer> clientList;
    for ( auto clientIterator = this->m_clientList.begin(); clientIterator != this->m_clientList.end(); ++clientIterator )
    {
        LogPrint( "Checking ", clientIterator->second->GetClientId() );
        bool clientFound = false;
        for ( auto aliveIterator = aliveClients.begin(); aliveIterator != aliveClients.end(); ++aliveIterator )
        {
            LogPrint( *aliveIterator, " is alive" );
            if ( clientIterator->second->GetClientId() == *aliveIterator )
            {
                this->m_keepAliveMap[*aliveIterator] = 0;
                clientFound = true;
                break;
            }
        }
        if ( !clientFound )
        {
            WarningPrint( clientIterator->second->GetClientId(), " did not respond for ", ( unsigned int )( this->m_keepAliveMap[clientIterator->second->GetClientId()] ) );
            this->m_keepAliveMap[clientIterator->second->GetClientId()]++;
            if ( this->m_keepAliveMap[clientIterator->second->GetClientId()] == 3 )
            {
                clientList.push_back( clientIterator->second );
            }
        }
    }
    for ( auto clientIterator = clientList.begin(); clientIterator != clientList.end(); ++clientIterator )
    {
        this->DeleteClient( *clientIterator );
    }
    this->m_listProtectionMutex.unlock();
    LOG_FUNCTION_END();
}
