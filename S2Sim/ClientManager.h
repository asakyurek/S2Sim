/**
 *  @file ClientManager.h
 *  Header file for the ClientManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_

#include <fstream>
#include <thread>

#include "MessageHeader.h"
#include "ClientConnectionRequest.h"
#include "ClientConnectionResponse.h"
#include "SetCurrentPrice.h"
#include "PriceProposal.h"
#include "ClientData.h"
#include "GetPrice.h"
#include "SystemTimePrompt.h"
#include "SystemVersionPrompt.h"
#include "DemandNegotiation.h"
#include "ConnectionManager.h"
#include "MatlabManager.h"
#include "SystemManager.h"
#include "ThreadedTCPConnectedClient.h"

#include "LogPrint.h"

using namespace TerraSwarm;

/**
 * @brief Manages the connection with a client.
 *
 * This class manages the connection to a single client including message processing and message construction.
 */
class ClientManager
{
    private:
    /**
     *  Defines the client type
     */
        typedef unsigned char TClientType;
    
    /**
     *  There are two client types.
        - Asynchronous client that connects, delivers its data and disconnects.
        - Synchronous client that connects and delivers data in a time synchronous way.
     */
        enum ClientTypeValues
        {
            AsynchronousClient = ( TClientType )0x01, /**< Asynchronous client type. */
            SynchronousClient = ( TClientType )0x02 /**< Synchronous client type. */
        };
    
    /**
     *  Defines whether the client is registered or not.
     */
        typedef bool TIsRegistered;
    
    /**
     *  Defines the values for the TIsRegistered type.
     */
        enum IsRegisteredValues
        {
            IsRegistered = true,
            IsNotRegistered = false
        };

    public:
    /**
     *  Type of the Unique ID of the client in the system.
     */
        typedef MessageHeader::TId TId;
    
    /**
     *  Type used to represent a price signal.
     */
        typedef Synchronous::SetCurrentPrice::TPrice TPrice;
    
    /**
     *  Type used to represent number of price values.
     */
        typedef Synchronous::SetCurrentPrice::TNumberOfPricePoints TNumberOfPriceValues;
    
    /**
     *  Type used to represent a time interval.
     */
        typedef Synchronous::SetCurrentPrice::TInterval TInterval;
    
    /**
     *  Redefines the type for object name for rapid development.
     */
        typedef Asynchronous::ClientConnectionRequest::TClientName TClientName;

    private:
    /**
     *  Static variable that holds the next Unique ID. Incremented at each new connection. Returns to 0 if all values have been used.
     */
        static TId nextClientId;
    
    /**
     *  States whether the client is registered at the Control Manager.
     */
        TIsRegistered m_isRegistered;
    
    /**
     *  Pointer to the TCP Client connection managing class instance. This variable is used for all communication purposes.
     */
        ThreadedTCPConnectedClient* m_client;
    /**
     *  Actual assigned Unique ID of the managed client.
     */
        TId m_clientId;
    /**
     *  Type of the managed client. @see{ClientManager::ClientTypeValues}
     */
        TClientType m_clientType;
    /**
     *  Name of the managed client.
     */
        TClientName m_clientName;

    private:
    /**
     *  @brief Processes an asynchronous client connection request.
     *
     * This function processes a received asynchronous client connection request. There are multiple steps in the acceptance procedure:
     *  - Checking with OpenDSS whether the object actually exists.
     *  - If the object exists, an acceptance message is sent back.
     *  - Next client id is incremented.
     *  - The client is registered to the ControlManager.
     *
     *  @param data Received message structure in TerraSwarm::Asynchronous::ClientConnectionRequest.
     */
        void
        ProcessClientConnectionRequest( Asynchronous::ClientConnectionRequest* data );

    /**
     *  @brief Processes an synchronous client connection request.
     *
     * This function processes a received synchronous client connection request. There are multiple steps in the acceptance procedure:
     *  - Checking with OpenDSS whether the object actually exists.
     *  - If the object exists, an acceptance message is sent back.
     *  - Next client id is incremented.
     *  - The client is registered to the ControlManager.
     *
     *  @param data Received message structure in TerraSwarm::Synchronous::ClientConnectionRequest.
     */
        void
        ProcessClientConnectionRequest( Synchronous::ClientConnectionRequest* data );

    /**
     *  @brief Processes the consumption information.
     *
     *  This function processes the received consumption information of the asynchronous client. The data is registered to the SystemManager.
     *
     *  @param data Received message structure in TerraSwarm::Asynchronous::ClientData.
     */
        void
        ProcessClientData( Asynchronous::ClientData* data );

    /**
     *  @brief Processes the consumption information.
     *
     *  This function processes the received consumption information of the synchronous client. The data is registered to the SystemManager.
     *
     *  @param data Received message structure in TerraSwarm::Synchronous::ClientData.
     */
        void
        ProcessClientData( Synchronous::ClientData* data );
    
    /**
     *  @brief Processes the consumption information.
     *
     *  This function processes the received consumption information and possible predicted consumption informations from the synchronous client. The data is registered to the SystemManager.
     *
     *  @param data Received message structure in TerraSwarm::Synchronous::ClientExtendedData.
     */
        void
        ProcessClientExtendedData( Synchronous::ClientExtendedData* data );

    /**
     *  @brief Processes the price signal request.
     *
     *  The price signal request by the synchronous client is sent to ControlManager.
     *  @param data Received message structure in TerraSwarm::Synchronous::GetPrice.
     */
        void
        ProcessGetPrice( Synchronous::GetPrice* data );

    /**
     *  @brief Processes the demand negotiation response.
     *
     *  The client sends a demand negotiation response to the simulator. This response is sent to ControlManager.
     *
     *  @param data Received message structure in TerraSwarm::Synchronous::DemandNegotiation.
     */
        void
        ProcessDemandNegotiation( Synchronous::DemandNegotiation* data );
    
    /**
     *  @brief Processes the system version prompt.
     *
     *  The client sends a system version prompt to get the current system version.
     *
     *  @param data Received message structure in TerraSwarm::SystemVersionPrompt.
     */
        void
        ProcessSystemVersionPrompt( SystemVersionPrompt* data );
    
    /**
     *  @brief Processes the system time prompt.
     *
     *  The client sends a system time prompt to get the current system time.
     *
     *  @param data Received message structure in TerraSwarm::SystemTimePrompt.
     */
        void
        ProcessSystemTimePrompt( SystemTimePrompt* data );

    public:
    /**
     *  @brief Default constructor.
     *
     *  Initializes the variables.
     */
        ClientManager( void );
    
    /**
     *  @brief Constructs the client manager with a connection.
     *
     *  This constructor initializes the class and sets the connection handle to the accepted connection.
     *  @param client Pointer to the accepted TCP connection.
     */
        ClientManager( ThreadedTCPConnectedClient* client );
    
    /**
     *  @brief Copy constructor.
     *
     *  Copies the contents of another client manager. Written to enable usage within stdlib containers.
     *  @param copy Client manager to be copied.
     */
        ClientManager( const ClientManager& copy );

    /**
     *  @brief Destructor, deletes the connection.
     *
     *  The destructor deletes the accepted connection since it is not managed anymore. @todo This should be replaced by a reference counting memory manager.
     */
        ~ClientManager( void );

    /**
     *  @brief Copies the contents of another client manager.
     *
     *  Overloads the equal operator and copies the content of another client manager.
     *
     *  @param copy Instance to be copied.
     *
     *  @return Returns a reference to itself.
     */
        ClientManager&
        operator = ( const ClientManager & copy );

    /**
     *  @brief Handles the broken connection case.
     *
     *  If the connection with the client is broken, the stored information should be deleted. This function is called by ClientManager::m_client.
     */
        void
        ConnectionBroken( void );

    /**
     *  @brief Called when a message is received.
     *
     *  This function is called by m_client when a message is received. It checks for all possible message types and calls the necessary processing method.
     *
     *  @param data     Pointer to the received data.
     *  @param dataSize Size of the received data.
     */
        void
        MessageReceived( void* data, const size_t dataSize );

    /**
     *  Sends a price signal to the client.
     *
     *  @param priceValues   Indicates the price values for the intervals.
     *  @param beginInterval Indicates the beginning time for the price.
     *  @param numberOfPriceValues   Indicates the number of price values.
     */
        void
        SetCurrentPrice( const TInterval beginInterval, const TNumberOfPriceValues numberOfPriceValues, TPrice* priceValues );

    /**
     *  Sends a price proposal to the client.
     *
     *  @param price         Proposed price.
     *  @param beginInterval Indicates the beginning time for the proposed price.
     *  @param endInterval   Indicates the ending time for the proposed price.
     */
        void
        PriceProposal( const TPrice price, const TInterval beginInterval, const TInterval endInterval );

    /**
     *  Method to check for Synchronous Client type. @see m_clientType
     *
     *  @return Returns whether the client is synchronous.
     */
        bool
        IsSynchronous( void ) const
        {
            return ( this->m_clientType == SynchronousClient );
        }

    /**
     *  Method to check for Asynchronous Client type. @see m_clientType
     *
     *  @return Returns whether the client is asynchronous.
     */
        bool
        IsAsynchronous( void ) const
        {
            return ( this->m_clientType == AsynchronousClient );
        }
    
    /**
     *  Prints the client information for the website.
     *  @param outputFile Stream to be written to.
     */
        void
        PrintClientInformation( std::ofstream & outputFile ) const;
    
    /**
     *  Returns the client id being managed.
     *
     *  @return Id of the client being managed.
     */
        TId
        GetClientId( void ) const
        {
            return ( this->m_clientId );
        }
    
    /**
     *  Stops the thread and deletes itself. This has to be called from a separate thread.
     */
        void
        StopClientAndClean( void );
};

#endif /* CLIENTMANAGER_H_ */
