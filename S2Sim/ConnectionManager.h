/*
 * ConnectionManager.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include "ClientManager.h"
#include "ThreadedTCPServer.h"
#include "ThreadedTCPConnectedClient.h"

#include "LogPrint.h"

#include <map>
#include <algorithm>

class ClientManager;

void
ConnectionReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size );

void
ConnectionNotificationHandler( TCPConnectedClient* acceptedClient );

class ConnectionManager
{
    friend ConnectionManager &
    GetConnectionManager( void );

    private:
        typedef TerraSwarm::MessageHeader::TId TClientId;

        typedef std::map<ThreadedTCPConnectedClient*,ClientManager*> TClientList;
        typedef std::map<ClientManager*,ThreadedTCPConnectedClient*> TReversedClientList;
        typedef std::map<TClientId, ClientManager*> TClientIdList;

    public:
        typedef unsigned short TNumberOfClients;

    private:
        TClientList m_clientList;
        TReversedClientList m_reversedClientList;
        ThreadedTCPServer m_server;

    private:
        ConnectionManager( void );

    public:
        void
        IncomingConnection( ThreadedTCPConnectedClient* newClient );

        void
        BrokenConnection( ThreadedTCPConnectedClient* brokenClient );

        void
        IncomingMessage( ThreadedTCPConnectedClient* senderClient, void* buffer, size_t size );

        void
        DeleteClient( ClientManager* clientManager );

        TNumberOfClients
        GetNumberOfClients( void ) const
        {
            return ( this->m_clientList.size() );
        }
};

ConnectionManager &
GetConnectionManager( void );

#endif /* CONNECTIONMANAGER_H_ */
