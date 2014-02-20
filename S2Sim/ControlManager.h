/*
 * ControlManager.h
 *
 *  Created on: Oct 27, 2013
 *      Author: Alper
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

using namespace TerraSwarm;

class ConnectionManager;
class SystemManager;
class MatlabManager;
class ControlManager;
class ClientManager;

ControlManager&
GetControlManager( void );

void
ControlNotificationHandler( TCPConnectedClient* acceptedClient );

void
ControlReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size );

class ControlManager
{
    friend ControlManager&
    GetControlManager( void );

    public:
        typedef Asynchronous::ClientData::TDataPoint TVoltage;
        typedef Asynchronous::ClientData::TDataPoint TWattage;
        typedef MessageHeader::TId TNumberOfClients;
        typedef MessageHeader::TId TClientId;
        typedef Asynchronous::ClientConnectionRequest::TClientName TClientName;
        typedef Asynchronous::ClientData::TDataPoint TPrice;

        typedef Asynchronous::ClientData::TDataPoint TDataPoint;
        typedef Asynchronous::ClientData::TNumberOfDataPoints TNumberOfDataPoints;


    private:
        typedef unsigned int TDataSize;
        typedef unsigned int TMessageType;

        enum MessageTypeValues
        {
            MakeDecisionType = 0x00000001,
            DecisionFinishedType = 0x00000002,
            SetPriceType = 0x00000003,
            SendPriceProposalType = 0x00000004,
            PriceRequestType = 0x00000005,
            DemandNegotiationType = 0x00000006
        };

        typedef std::map<TClientId, TClientName> TClientIdMap;
        typedef std::map<TClientId,ClientManager*> TClientManagerMap;

    private:
        ThreadedTCPServer m_server;
        ThreadedTCPConnectedClient* m_client;
        Semaphore m_readySemaphore;
        TClientIdMap m_clientIdMap;
        TClientManagerMap m_clientManagerMap;

    private:
        ControlManager( void );

    public:
        void
        SetClient( ThreadedTCPConnectedClient* client )
        {
            LOG_FUNCTION_START();
            this->m_client = client;
            this->m_client->SetNotificationCallback( &ControlReceiveHandler );
            LOG_FUNCTION_END();
        }

        void
        ProcessData( void* data, const size_t size );

        void
        MakeDecision( void );

        void
        WaitUntilReady( void )
        {
            LOG_FUNCTION_START();
            this->m_readySemaphore.TakeSemaphore();
            LOG_FUNCTION_END();
        }

        void
        RegisterClient( const TClientId clientId, const TClientName & clientName, ClientManager* clientManager )
        {
            LOG_FUNCTION_START();
            LogPrint( "Client ", clientName, " registered on Id: ", clientId );
            this->m_clientIdMap.insert( std::make_pair( clientId, clientName ) );
            this->m_clientManagerMap.insert( std::make_pair( clientId, clientManager ) );
            LOG_FUNCTION_END();
        }

        void
        UnRegisterClient( const TClientId clientId )
        {
            LOG_FUNCTION_START();
            LogPrint( "Unregistering client ", clientId );
            this->m_clientIdMap.erase( clientId );
            this->m_clientManagerMap.erase( clientId );
            LOG_FUNCTION_END();
        }

        void
        ClientPriceRequest( const TClientId clientId );

        void
        ClientDemandNegotiation( const TClientId clientId,
                                 const TNumberOfDataPoints numberOfDataPoints,
                                 TDataPoint* dataPoints );

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
