/*
 * ControlManager.cpp
 *
 *  Created on: Oct 27, 2013
 *      Author: Alper
 */

#include "ControlManager.h"

ControlManager&
GetControlManager( void )
{
    static ControlManager controlManager;
    return ( controlManager );
}

void
ControlNotificationHandler( ThreadedTCPConnectedClient* acceptedClient )
{
    LOG_FUNCTION_START();
    LogPrint( "External Controller connection accepted" );
    GetControlManager().SetClient( acceptedClient );
    LOG_FUNCTION_END();
}

void
ControlReceiveHandler( ThreadedTCPConnectedClient* client, void* buffer, size_t size )
{
    LOG_FUNCTION_START();
    LogPrint( "Data of size ", size, " received from External Controller" );
    GetControlManager().ProcessData( buffer, size );
    LOG_FUNCTION_END();
}

ControlManager::ControlManager( void ) : m_client( NULL ), m_readySemaphore( 0, ( char* )"ControlReadySemaphore" )
{
    LOG_FUNCTION_START();
    this->m_server.SetPort( 26997 );
    this->m_server.SetNotificationCallback( &ControlNotificationHandler );
    LOG_FUNCTION_END();
}

void
ControlManager::ProcessData( void* data, const size_t size )
{
    LOG_FUNCTION_START();
    if ( size <= 0 )
    {
        WarningPrint( "External Controller connection dropped" );
        delete this->m_client;
        this->m_client = NULL;
        LOG_FUNCTION_END();
        return;
    }
    int remainingSize = ( int )size;

    while ( remainingSize > 0 )
    {
    TMessageType messageType;
    char* currentAddress = ( char* )data;
    memcpy( &messageType, currentAddress, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );

    messageType = ntohl( messageType );
    if ( messageType == DecisionFinishedType )
    {
        remainingSize -= sizeof( TMessageType );
        LogPrint( "Decision Finished control message received. Releasing ready semaphore" );
        this->m_readySemaphore.ReleaseSemaphore();
        LogPrint( "Ready semaphore released" );
    }
    else if ( messageType == SetPriceType )
    {
        remainingSize -= sizeof( TMessageType ) + sizeof( TClientId ) + sizeof( TClientId ) + sizeof( TPrice );
        LogPrint( "Set Price control message received" );
        TClientId clientId;
        memcpy( &clientId, currentAddress, sizeof( TClientId ) );
        currentAddress += sizeof( TClientId );
        TClientId convertedClientId = ntohs( clientId );

        TClientId clientIdDummy;
        memcpy( &clientIdDummy, currentAddress, sizeof( TClientId ) );
        currentAddress += sizeof( TClientId );

        TPrice price;
        memcpy( &price, currentAddress, sizeof( TPrice ) );
        currentAddress += sizeof( TPrice );
        TPrice convertedPrice = ntohl( price );

        LogPrint( "Price of Client ", convertedClientId, " set to ", convertedPrice );
        if ( this->m_clientManagerMap.find( convertedClientId ) == this->m_clientManagerMap.end() )
        {
            ErrorPrint( "Client Id ", convertedClientId, " not found!" );
            LOG_FUNCTION_END();
            return;
        }
        this->m_clientManagerMap[convertedClientId]->SetCurrentPrice( convertedPrice,
                                                                      GetSystemManager().GetSystemTime(),
                                                                      GetSystemManager().GetSystemTime() + 1 );
    }
    else if ( messageType == SendPriceProposalType )
    {
        remainingSize -= sizeof( TMessageType ) + sizeof( TClientId ) + sizeof( TPrice );
        LogPrint( "Send Price Proposal control command received" );
        TClientId clientId;
        memcpy( &clientId, currentAddress, sizeof( TClientId ) );
        currentAddress += sizeof( TClientId );
        TClientId convertedClientId = ntohs( clientId );

        TPrice price;
        memcpy( &price, currentAddress, sizeof( TPrice ) );
        currentAddress += sizeof( TPrice );
        TPrice convertedPrice = ntohl( price );

        LogPrint( "Price of ", convertedPrice, " proposed to Client ", convertedClientId );

        this->m_clientManagerMap[convertedClientId]->PriceProposal( convertedPrice,
                                                                    GetSystemManager().GetSystemTime(),
                                                                    GetSystemManager().GetSystemTime() + 1 );
    }
    else
    {
        ErrorPrint( "Unknown Message Type received: ", messageType );
    }
    data = currentAddress;
    }
    LOG_FUNCTION_END();
}

void
ControlManager::MakeDecision( void )
{
    LOG_FUNCTION_START();
    LogPrint( "Staring Decision process" );
    TNumberOfClients numberOfClients = 0;

    for ( TClientManagerMap::iterator i = this->m_clientManagerMap.begin(); i != this->m_clientManagerMap.end(); i++ )
    {
        if ( i->second->IsSynchronous() )
        {
            ++numberOfClients;
        }
    }

    LogPrint( "Total number of synchronous clients: ", numberOfClients );

    TDataSize dataSize = sizeof( TDataSize ) +
                         sizeof( TMessageType ) +
                         sizeof( TNumberOfClients ) +
                         sizeof( SystemManager::TSystemMode ) +
                         sizeof( SystemManager::TSystemTime ) +
                         numberOfClients * sizeof( TVoltage ) +
                         numberOfClients * sizeof( TClientId ) +
                         numberOfClients * sizeof( TClientId );

    char* buffer = new char[dataSize];
    char* currentPointer = buffer;

    TDataSize convertedSize = htonl( dataSize );
    memcpy( currentPointer, &convertedSize, sizeof( TDataSize ) );
    currentPointer += sizeof( TDataSize );

    TMessageType messageType = htonl( MakeDecisionType );
    memcpy( currentPointer, &messageType, sizeof( TMessageType ) );
    currentPointer += sizeof( TMessageType );

    TNumberOfClients convertedNumberOfClients = htons( numberOfClients );
    memcpy( currentPointer, &convertedNumberOfClients, sizeof( TNumberOfClients ) );
    currentPointer += sizeof( TNumberOfClients );

    SystemManager::TSystemMode convertedSystemMode = htons( GetSystemManager().GetSystemMode() );
    memcpy( currentPointer, &convertedSystemMode, sizeof( SystemManager::TSystemMode ) );
    currentPointer += sizeof( SystemManager::TSystemMode );

    SystemManager::TSystemTime convertedSystemTime = htonl( GetSystemManager().GetSystemTime() );
    memcpy( currentPointer, &convertedSystemTime, sizeof( SystemManager::TSystemTime ) );
    currentPointer += sizeof( SystemManager::TSystemTime );
    for ( TClientManagerMap::iterator i = this->m_clientManagerMap.begin(); i != this->m_clientManagerMap.end(); i++ )
    {
        if ( i->second->IsSynchronous() )
        {
            TClientId clientId = htons( i->first );
            TVoltage voltage = htonl( GetMatlabManager().GetVoltage( this->m_clientIdMap[i->first] ) );

            memcpy( currentPointer, &clientId, sizeof( TClientId ) );
            currentPointer += sizeof( TClientId );

            memcpy( currentPointer, &clientId, sizeof( TClientId ) );
            currentPointer += sizeof( TClientId );

            memcpy( currentPointer, &voltage, sizeof( TVoltage ) );
            currentPointer += sizeof( TVoltage );
        }
    }
    LogPrint( "Send Voltage Information of synchronous clients to External Controller" );
    this->m_client->SendData( buffer, dataSize );
    delete[] buffer;
    LOG_FUNCTION_END();
}

void
ControlManager::ClientPriceRequest( const TClientId clientId )
{
    LOG_FUNCTION_START();
    LogPrint( "Forward the Price Request of Client ", clientId, " to External Controller" );
    TDataSize dataSize = sizeof( TDataSize ) +
                         sizeof( TMessageType ) +
                         sizeof( TClientId );
    char* buffer = new char[dataSize];
    char* currentPointer = buffer;

    TDataSize convertedSize = htonl( dataSize );
    memcpy( currentPointer, &convertedSize, sizeof( TDataSize ) );
    currentPointer += sizeof( TDataSize );

    TMessageType messageType = htonl( PriceRequestType );
    memcpy( currentPointer, &messageType, sizeof( TMessageType ) );
    currentPointer += sizeof( TMessageType );

    TClientId convertedClientId = htonl( clientId );
    memcpy( currentPointer, &convertedClientId, sizeof( TClientId ) );
    currentPointer += sizeof( TClientId );

    this->m_client->SendData( buffer, dataSize );
    delete[] buffer;
    LOG_FUNCTION_END();
}

void
ControlManager::ClientDemandNegotiation( const TClientId clientId,
                                         const TNumberOfDataPoints numberOfDataPoints,
                                         TDataPoint* dataPoints )
{
    LOG_FUNCTION_START();
    LogPrint( "Forward Demand Negotiation response of Client ", clientId, " with ", numberOfDataPoints, " data points to  the External Controller" );
    TDataSize dataSize = sizeof( TDataSize ) +
                         sizeof( TMessageType ) +
                         sizeof( TClientId ) +
                         sizeof( TNumberOfDataPoints ) +
                         numberOfDataPoints * sizeof( TDataPoint );

    char* buffer = new char[dataSize];
    char* currentPointer = buffer;

    TDataSize convertedSize = htonl( dataSize );
    memcpy( currentPointer, &convertedSize, sizeof( TDataSize ) );
    currentPointer += sizeof( TDataSize );

    TMessageType messageType = htonl( DemandNegotiationType );
    memcpy( currentPointer, &messageType, sizeof( TMessageType ) );
    currentPointer += sizeof( TMessageType );

    TClientId convertedClientId = htonl( clientId );
    memcpy( currentPointer, &convertedClientId, sizeof( TClientId ) );
    currentPointer += sizeof( TClientId );

    TNumberOfDataPoints convertedNumberOfDataPoints = htonl( numberOfDataPoints );
    memcpy( currentPointer, &convertedNumberOfDataPoints, sizeof( TNumberOfDataPoints ) );
    currentPointer += sizeof( TNumberOfDataPoints );

    for ( TNumberOfDataPoints index = 0; index < numberOfDataPoints; ++index )
    {
        TDataPoint convertedDataPoint = htonl( dataPoints[index] );
        memcpy( currentPointer, &convertedDataPoint, sizeof( TDataPoint ) );
        currentPointer += sizeof( TDataPoint );
    }

    this->m_client->SendData( buffer, dataSize );
    delete[] buffer;
    LOG_FUNCTION_END();
}
