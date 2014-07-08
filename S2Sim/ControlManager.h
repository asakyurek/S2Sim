/**
 * @file ControlManager.h
 * This file defines the ControlManager class.
 *  @date Oct 27, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef CONTROLMANAGER_H_
#define CONTROLMANAGER_H_

#include "SystemManager.h"
#include "ThreadedTCPServer.h"
#include "MatlabManager.h"
#include "ThreadedTCPConnectedClient.h"
#include "ConnectionManager.h"
#include "SystemManager.h"
#include "ClientManager.h"
#include "LogPrint.h"

#include <mutex>

using namespace TerraSwarm;

class ConnectionManager;
class SystemManager;
class MatlabManager;
class ControlManager;
class ClientManager;

/**
 *  @brief Returns the only instance of ControlManager.
 *
 *  @return Only instance of ControlManager.
 */
ControlManager&
GetControlManager( void );

/**
 *  @brief Wrapper callback method for connection notification.
 *
 *  This method is called to notify that a new external controller connection is accepted. The information is relayed directly to ControlManager.
 *
 *  @param acceptedClient Information about the newly accepted external controller.
 */
void
ControlNotificationHandler( TCPConnectedClient* acceptedClient );

/**
 *  @brief Wrapper callback method for reception notification.
 *
 *  This method is called to notify that a new data is received from the external controller. The data is relayed directly to ControlManager.
 *
 *  @param client Connection information of the external controller.
 *  @param buffer Buffer containing the received data.
 *  @param size   Size of the received data.
 */
void
ControlReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size );

/**
 *  @brief Manages the connection with the External Controller.
 *
 *  This class manages the connection to the External Controller. All received commands are processed and necessary updates are sent back.
 */
class ControlManager
{
    /**
     *  Friend method for singleton implementation.
     *
     *  @return Returns the reference to the ControlManager.
     */
    friend ControlManager&
    GetControlManager( void );

    public:
    /**
     *  Defines the type of Voltage information.
     */
        typedef Asynchronous::ClientData::TDataPoint TVoltage;
    
    /**
     *  Defines the type of Wattage/Power information.
     */
        typedef Asynchronous::ClientData::TDataPoint TWattage;
    
    /**
     *  Defines the type of number of clients.
     */
        typedef MessageHeader::TId TNumberOfClients;
    
    /**
     *  Redefines the type for unique id for rapid development.
     */
        typedef MessageHeader::TId TClientId;
    
    /**
     *  Redefines the type for object name for rapid development.
     */
        typedef Asynchronous::ClientConnectionRequest::TClientName TClientName;
    
    /**
     *  Defines the type for the price feedback signal.
     */
        typedef Asynchronous::ClientData::TDataPoint TPrice;
    
    /**
     *  Defines the number of price points in the signal.
     */
        typedef Synchronous::SetCurrentPrice::TNumberOfPricePoints TNumberOfPricePoints;

    /**
     *  Redefines the general data type for rapid development.
     */
        typedef Asynchronous::ClientData::TDataPoint TDataPoint;
    
    /**
     *  Redefines the number of data points type for rapid development.
     */
        typedef Asynchronous::ClientData::TNumberOfDataPoints TNumberOfDataPoints;

    private:
    /**
     *  Defines the type for size of a data chunk.
     */
        typedef unsigned int TDataSize;
    
    /**
     *  Defines the message type field used in the communication for message processing.
     */
        typedef unsigned int TMessageType;

    /**
     *  Defines the available values for different message types.
     */
        enum MessageTypeValues
        {
            MakeDecisionType = 0x00000001, /**< Sent to external controller to indicate the beginning of a frame. */
            DecisionFinishedType = 0x00000002, /**< Received from external controller for the end of a frame. */
            SetPriceType = 0x00000003, /**< Sent by external controller to send a price signal to a client. */
            SendPriceProposalType = 0x00000004, /**< Sent by external controller to send a price proposal to a client. */
            PriceRequestType = 0x00000005, /**< Sent to external controller to indicate a price request by a client. */
            DemandNegotiationType = 0x00000006 /**< Sent to external controller to indicate a price proposal response by a client. */
        };

    /**
     *  Defines the type holding a ClientId->ClientName mapping.
     */
        typedef std::map<TClientId, TClientName> TClientIdMap;
    
    /**
     *  Defines the type holding a ClientId->ClientManager mapping.
     */
        typedef std::map<TClientId,ClientManager*> TClientManagerMap;

    private:
    /**
     *  Implements the TCP server for external controller communication.
     */
        ThreadedTCPServer m_server;
    
    /**
     *  Pointer to the currently accepted external controller communication handler.
     */
        ThreadedTCPConnectedClient* m_client;
    
    /**
     *  Timed mutex that forces the syncrhonization between the caller and the External Controller manager.
     */
        std::timed_mutex m_readyMutex;
    
    /**
     *  Map containing the unique id to object name mapping.
     */
        TClientIdMap m_clientIdMap;
    
    /**
     *  Map containing the unique id to ClientManager mapping.
     */
        TClientManagerMap m_clientManagerMap;
    
    /**
     *  Mutex protecting the m_clientIdMap and m_clientManagerMap members.
     */
        std::mutex m_clientMapLock;
    
    /**
     *  Mutex protecting the client thread object from deletion.
     */
        std::mutex m_clientThreadMutex;

    private:
    /**
     *  Private constructor for singleton implementation.
     */
        ControlManager( void );
    
    /**
     *  Deletes the TCP thread safely.
     */
        void
        DeleteClientThread( void );

    public:
    /**
     *  Sets the connection information to the accepted External Controller.
     *
     *  @param client Connection information fo the newly accepted External Controller.
     */
        void
        SetClient( ThreadedTCPConnectedClient* client )
        {
            LOG_FUNCTION_START();
            this->m_client = client;
            if ( this->m_client != nullptr )
            {
                this->m_client->SetNotificationCallback( &ControlReceiveHandler );
                this->m_clientThreadMutex.unlock();
            }
            LOG_FUNCTION_END();
        }

    /**
     *  @brief Processes the received message from the External Controller.
     *
     *  This function processes the received message from the External Controller. Note that the received buffer is not guaranteed to hold a single message. This function processes the buffer in a while loop until all received messages are processed in order not to lose any data. The work flow is as follows:
      - Check the received size for a connection drop (gracefull or not).
      - Check the message type and process the respective data structure.
      - Repeat process until the remaining unprocessed message size is zero.
     @todo Divide the function into multiple functions, processing each message separately.
     *
     *  @param data Buffer for the received message.
     *  @param size Size of the received message.
     */
        void
        ProcessData( void* data, const size_t size );

    /**
     *  @brief Starts the decision process by sending necessary information to External Controller.
     
        This function starts the synchronous client decision process by sending the required information for each client. The information contains:
        - Message Size for easier processing.
        - Total number of synchronous clients.
        - System mode.
        - System time.
        - For each client:
           - Client unique id
           - Client terminal voltage
     */
        void
        MakeDecision( void );

    /**
     *  Stops the calling thread until a ready signal is received from the External Controller. @todo This doesn't need to be inline.
     */
        void
        WaitUntilReady( void )
        {
            LOG_FUNCTION_START();
            this->m_readyMutex.lock();
            LOG_FUNCTION_END();
        }

    /**
     *  Registers a new client to its maps for later referencing. @todo This shouldn't be inline.
     *
     *  @param clientId      Unique client id.
     *  @param clientName    Object name.
     *  @param clientManager ClientManager instance managing the client related works.
     */
        void
        RegisterClient( const TClientId clientId, const TClientName & clientName, ClientManager* clientManager )
        {
            LOG_FUNCTION_START();
            LogPrint( "Client ", clientName, " registered on Id: ", clientId );
            this->m_clientMapLock.lock();
            this->m_clientIdMap.insert( std::make_pair( clientId, clientName ) );
            this->m_clientManagerMap.insert( std::make_pair( clientId, clientManager ) );
            this->m_clientMapLock.unlock();
            LOG_FUNCTION_END();
        }

    /**
     *  Deletes a client from the maps when connection is broken.
     *
     *  @param clientId Unique client id of the broken connection.
     */
        void
        UnRegisterClient( const TClientId clientId )
        {
            LOG_FUNCTION_START();
            LogPrint( "Unregistering client ", clientId );
            this->m_clientMapLock.lock();
            if ( this->m_clientIdMap.find( clientId ) != this->m_clientIdMap.end() )
            {
                this->m_clientIdMap.erase( clientId );
            }
            if ( this->m_clientManagerMap.find( clientId ) != this->m_clientManagerMap.end() )
            {
                this->m_clientManagerMap.erase( clientId );
            }
            this->m_clientMapLock.unlock();
            LOG_FUNCTION_END();
        }

    /**
     *  Relays the price request of a client to the External Controller.
     *
     *  @param clientId Unique client id of the requesting client.
     */
        void
        ClientPriceRequest( const TClientId clientId );

    /**
     *  Relays the demand negotiation response of the client to the External Controller.
     *
     *  @param clientId           Unique client id of the responding client.
     *  @param numberOfDataPoints Number of consumption points sent.
     *  @param dataPoints         Buffer for the consumption points.
     */
        void
        ClientDemandNegotiation( const TClientId clientId,
                                 const TNumberOfDataPoints numberOfDataPoints,
                                 TDataPoint* dataPoints );

    /**
     *  Returns the name of the object. @todo Let's make this not inline for debugging.
     *
     *  @param clientId Unique client id for the requested object.
     *
     *  @return Object name of the requested client id.
     */
        TClientName
        GetClientName( const TClientId clientId )
        {
            if ( this->m_clientIdMap.find( clientId ) == this->m_clientIdMap.end() )
            {
                ErrorPrint( "Client ", clientId, " not found!" );
            }
            return ( this->m_clientIdMap[clientId] );
        }
};

#endif /* CONTROLMANAGER_H_ */
