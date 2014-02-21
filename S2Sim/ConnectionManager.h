/**
 * @file ConnectionManager.h
 * This file defines the ConnectionManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
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

/**
 *  @brief Wrapper callback method for reception notification.
 *
 *  This method is called to notify that a data has been received. This method relays the information directly to ConnectionManager.
 *
 *  @param client Client information of the sender.
 *  @param buffer Buffer containing the message.
 *  @param size   Size of the message.
 */
void
ConnectionReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size );

/**
 *  @brief Wrapper callback method for connection notification.
 *
 *  This method is called to notify that a new connection is established. This method relays the notification directly to the ConnectionManager.
 *
 *  @param acceptedClient Information about the newly accepted client.
 */
void
ConnectionNotificationHandler( TCPConnectedClient* acceptedClient );

/**
 *  @brief Manages connections to all clients.
 *
 *  This class manages the connections to all clients. It implements a TCP server and answers any connection attempts. It forwards received data to the correct ClientManager instance for processing.
 */
class ConnectionManager
{
    /**
     *  Friend method for singleton implementation.
     *
     *  @return Returns the reference to the ConnectionManager.
     */
    friend ConnectionManager &
    GetConnectionManager( void );

    private:
    /**
     *  Type representing the Unique Id of the client.
     */
        typedef TerraSwarm::MessageHeader::TId TClientId;

    /**
     *  Type mapping TCP client connection pointers to their respective managers.
     */
        typedef std::map<ThreadedTCPConnectedClient*,ClientManager*> TClientList;
    
    /**
     *  Type mapping the client managers to their respective TCP client connections.
     */
        typedef std::map<ClientManager*,ThreadedTCPConnectedClient*> TReversedClientList;
    
    /**
     *  Type mapping the client unique id's to respective managers.
     */
        typedef std::map<TClientId, ClientManager*> TClientIdList;

    public:
    /**
     *  Represents the type for number of clients.
     */
        typedef TClientId TNumberOfClients;

    private:
    /**
     *  Holds the Connection->Manager based list.
     */
        TClientList m_clientList;
    
    /**
     *  Holds the Manager->Connection based list for speed.
     */
        TReversedClientList m_reversedClientList;
    
    /**
     *  Instance of the TCP server.
     */
        ThreadedTCPServer m_server;

    private:
    /**
     *  Private constructor for singleton implementation.
     */
        ConnectionManager( void );

    public:
    /**
     *  @brief Incoming connection handler.
     *
     *  This method handles a new connection from a new client. A new ClientManager is created and the information is added to the lists for book-keeping.
     *
     *  @param newClient TCP Information of the new connection.
     */
        void
        IncomingConnection( ThreadedTCPConnectedClient* newClient );

    /**
     *  @brief Broken connection handler.
     *
     *  This method handles a broken connection to an existing client. The ClientManager is notified, leaving the handling to it.
     *
     *  @param brokenClient TCP Information of the broken connection.
     */
        void
        BrokenConnection( ThreadedTCPConnectedClient* brokenClient );

    /**
     *  @brief New message processor.
     *
     *  This method takes the received information and relays it to the respective ClientManager.
     *
     *  @param senderClient TCP Information of the sender.
     *  @param buffer       Buffer containing the received message.
     *  @param size         Size of the received message.
     */
        void
        IncomingMessage( ThreadedTCPConnectedClient* senderClient, void* buffer, size_t size );

    /**
     *  Deletes the ClientManager from all lists and finally deletes itself.
     *
     *  @param clientManager ClientManager to be deleted.
     */
        void
        DeleteClient( ClientManager* clientManager );

    /**
     *  Returns the current number of clients connected.
     *
     *  @return Number of connected clients.
     */
        TNumberOfClients
        GetNumberOfClients( void ) const
        {
            return ( this->m_clientList.size() );
        }
};

ConnectionManager &
GetConnectionManager( void );

#endif /* CONNECTIONMANAGER_H_ */
