/*
 * ClientManager.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_

#include "MessageHeader.h"
#include "ClientConnectionRequest.h"
#include "ClientConnectionResponse.h"
#include "SetCurrentPrice.h"
#include "PriceProposal.h"
#include "ClientData.h"
#include "GetPrice.h"
#include "DemandNegotiation.h"
#include "ConnectionManager.h"
#include "MatlabManager.h"
#include "SystemManager.h"
#include "ThreadedTCPConnectedClient.h"

#include "LogPrint.h"

using namespace TerraSwarm;


class ClientManager
{
    private:
        typedef unsigned char TClientType;
        enum ClientTypeValues
        {
            AsynchronousClient = ( TClientType )0x01,
            SynchronousClient = ( TClientType )0x02
        };

        typedef MessageHeader::TId TId;

    public:
        typedef Synchronous::SetCurrentPrice::TPrice TPrice;
        typedef Synchronous::SetCurrentPrice::TInterval TInterval;

    private:
        static TId nextClientId;
        ThreadedTCPConnectedClient* m_client;
        TId m_clientId;
        TClientType m_clientType;

    private:
        void
        ProcessClientConnectionRequest( Asynchronous::ClientConnectionRequest* data );

        void
        ProcessClientConnectionRequest( Synchronous::ClientConnectionRequest* data );

        void
        ProcessClientData( Asynchronous::ClientData* data );

        void
        ProcessClientData( Synchronous::ClientData* data );

        void
        ProcessGetPrice( Synchronous::GetPrice* data );

        void
        ProcessDemandNegotiation( Synchronous::DemandNegotiation* data );

    public:
        ClientManager( void );
        ClientManager( ThreadedTCPConnectedClient* client );
        ClientManager( const ClientManager& copy );

        ~ClientManager( void );

        ClientManager&
        operator = ( const ClientManager & copy );

        void
        ConnectionBroken( void );

        void
        MessageReceived( void* data, const size_t dataSize );

        void
        SetCurrentPrice( const TPrice price, const TInterval beginInterval, const TInterval endInterval );

        void
        PriceProposal( const TPrice price, const TInterval beginInterval, const TInterval endInterval );

        bool
        IsSynchronous( void ) const
        {
            return ( this->m_clientType == SynchronousClient );
        }

        bool
        IsAsynchronous( void ) const
        {
            return ( this->m_clientType == AsynchronousClient );
        }
};

#endif /* CLIENTMANAGER_H_ */
