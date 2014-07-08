/**
 * @file ControlManager.cpp
 * This file implements the ControlManager class.
 * @date Oct 27, 2013
 * @author: Alper Sinan Akyurek
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

ControlManager::ControlManager( void ) : m_client( NULL )
{
    LOG_FUNCTION_START();
    this->m_readyMutex.lock();
    this->m_server.SetPort( 26997 );
    this->m_server.SetNotificationCallback( &ControlNotificationHandler );
    LOG_FUNCTION_END();
}

void
ControlManager::ProcessData( void* data, const size_t size )
{
    LOG_FUNCTION_START();
    int remainingSize = ( int )size;
    
    if ( remainingSize <= 0 )
    {
        WarningPrint( "External Controller connection dropped" );
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return;
    }

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
            this->m_readyMutex.unlock();
            LogPrint( "Ready semaphore released" );
        }
        else if ( messageType == SetPriceType )
        {
            remainingSize -= sizeof( TMessageType ) + sizeof( TClientId ) + sizeof( TClientId ) + sizeof( TNumberOfPricePoints );
            LogPrint( "Set Price control message received" );
            TClientId clientId;
            memcpy( &clientId, currentAddress, sizeof( TClientId ) );
            currentAddress += sizeof( TClientId );
            TClientId convertedClientId = ntohs( clientId );
            currentAddress += sizeof( TClientId );
            
            TNumberOfPricePoints numberOfPricePoints;
            memcpy( &numberOfPricePoints, currentAddress, sizeof( TNumberOfPricePoints ) );
            currentAddress += sizeof( TNumberOfPricePoints );
            TNumberOfPricePoints convertedNumberOfPricePoints = ntohl( numberOfPricePoints );

            TPrice* priceData = new TPrice[convertedNumberOfPricePoints];
            memcpy( priceData, currentAddress, sizeof( TPrice ) * convertedNumberOfPricePoints );
            currentAddress += sizeof( TPrice ) * convertedNumberOfPricePoints;
            remainingSize -= sizeof( TPrice ) * convertedNumberOfPricePoints;
            
            LogPrint( convertedNumberOfPricePoints, " number of prices for Client ", convertedClientId, " set starting with ", ntohl( priceData[0] ) );
            if ( this->m_clientManagerMap.find( convertedClientId ) == this->m_clientManagerMap.end() )
            {
                ErrorPrint( "Client Id ", convertedClientId, " not found!" );
                LOG_FUNCTION_END();
                return;
            }
            this->m_clientManagerMap[convertedClientId]->SetCurrentPrice( GetSystemManager().GetSystemTime(),
                                                                          convertedNumberOfPricePoints,
                                                                          priceData );
            delete[] priceData;
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
                         numberOfClients * sizeof( TClientId ) +
                         numberOfClients * sizeof( TClientId ) +
                         numberOfClients * sizeof( TNumberOfDataPoints );

    this->m_clientMapLock.lock();
    for ( TClientManagerMap::iterator i = this->m_clientManagerMap.begin(); i != this->m_clientManagerMap.end(); i++ )
    {
        if ( i->second->IsSynchronous() )
        {
            dataSize += GetSystemManager().GetNumberOfConsumptions( i->first ) * ( sizeof( TDataPoint ) + sizeof( TVoltage ) );
        }
    }
    this->m_clientMapLock.unlock();
    
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
    
    TVoltage** voltageValues = nullptr;
    TDataPoint** consumptionValues = nullptr;
    TNumberOfDataPoints* numberOfDataPointValues = nullptr;
    
    if ( numberOfClients > 0 )
    {
        voltageValues = new TVoltage*[numberOfClients];
        consumptionValues = new TDataPoint*[numberOfClients];
        numberOfDataPointValues = new TNumberOfDataPoints[numberOfClients];
    }
    
    TNumberOfDataPoints maximumNumberOfPoints = 0;
    TNumberOfDataPoints clientIndex = 0;
    
    this->m_clientMapLock.lock();
    for ( TClientManagerMap::iterator i = this->m_clientManagerMap.begin(); i != this->m_clientManagerMap.end(); i++ )
    {
        if ( i->second->IsSynchronous() )
        {
            TNumberOfDataPoints numberOfDataPoints = GetSystemManager().GetNumberOfConsumptions( i->first );
            
            LogPrint( "Number of data points for ", i->first, " is ", numberOfDataPoints );
            
            if ( numberOfDataPoints > 0 )
            {
                voltageValues[clientIndex] = new TVoltage[numberOfDataPoints];
                consumptionValues[clientIndex] = new TDataPoint[numberOfDataPoints];
            }
            numberOfDataPointValues[clientIndex] = numberOfDataPoints;
            ++clientIndex;
            
            if ( maximumNumberOfPoints < numberOfDataPoints )
            {
                maximumNumberOfPoints = numberOfDataPoints;
            }
        }
    }
    
    for ( TNumberOfDataPoints dataIndex = 0; dataIndex < maximumNumberOfPoints; ++dataIndex )
    {
        LogPrint( "Predicting ", dataIndex, " time steps from now" );
        GetSystemManager().SetConsumptionsToPredictionTime( GetSystemManager().GetSystemTime() + dataIndex );
        
        clientIndex = 0;
        for ( TClientManagerMap::iterator i = this->m_clientManagerMap.begin(); i != this->m_clientManagerMap.end(); i++ )
        {
            if ( i->second->IsSynchronous() )
            {
                if ( numberOfDataPointValues[clientIndex] > dataIndex )
                {
                    std::pair<TVoltage, TDataPoint> result = GetMatlabManager().GetVoltageDeviationAndConsumption( this->m_clientIdMap[i->first] );
                
                    voltageValues[clientIndex][dataIndex] = result.first;
                    consumptionValues[clientIndex][dataIndex] = result.second;
                    
                    LogPrint( "Client ", i->first, "'s predicted deviation: ", voltageValues[clientIndex][dataIndex], ", consumption: ", consumptionValues[clientIndex][dataIndex] );
                }
                ++clientIndex;
            }
        }
    }
    
    clientIndex = 0;
    for ( TClientManagerMap::iterator i = this->m_clientManagerMap.begin(); i != this->m_clientManagerMap.end(); i++ )
    {
        if ( i->second->IsSynchronous() )
        {
            TClientId clientId =  i->first;
            TClientId convertedClientId = htons( clientId );
            
            TNumberOfDataPoints numberOfDataPoints = numberOfDataPointValues[clientIndex];
            TNumberOfDataPoints convertedNumberOfDataPoints = htonl( numberOfDataPoints );
            memcpy( currentPointer, &convertedNumberOfDataPoints, sizeof( TNumberOfDataPoints ) );
            currentPointer += sizeof( TNumberOfDataPoints );
            
            for ( TNumberOfDataPoints dataIndex = 0; dataIndex < numberOfDataPoints; ++dataIndex )
            {
                TDataPoint currentConsumption = consumptionValues[clientIndex][dataIndex];
                TDataPoint convertedCurrentConsumption = htonl( currentConsumption );
                memcpy( currentPointer, &convertedCurrentConsumption, sizeof( TDataPoint ) );
                currentPointer += sizeof( TDataPoint );
                
                TVoltage voltage = voltageValues[clientIndex][dataIndex];
                TVoltage convertedVoltage = htonl( voltage );
                memcpy( currentPointer, &convertedVoltage, sizeof( TVoltage ) );
                currentPointer += sizeof( TVoltage );
            }
            
            memcpy( currentPointer, &convertedClientId, sizeof( TClientId ) );
            currentPointer += sizeof( TClientId );
            
            memcpy( currentPointer, &convertedClientId, sizeof( TClientId ) );
            currentPointer += sizeof( TClientId );
            
            if ( numberOfDataPointValues[clientIndex] > 0 )
            {
                delete [] consumptionValues[clientIndex];
                delete [] voltageValues[clientIndex];
            }
            ++clientIndex;
        }
    }
    
    if ( numberOfClients > 0 )
    {
        delete [] consumptionValues;
        delete [] voltageValues;
        delete [] numberOfDataPointValues;
    }
    
    GetSystemManager().SetConsumptionsToPredictionTime( GetSystemManager().GetSystemTime() );
    
    this->m_clientMapLock.unlock();
    LogPrint( "Send Voltage and Consumption Information of synchronous clients to External Controller" );
    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        WarningPrint( "Connection to External Controller is lost." );
        delete[] buffer;
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return;
    }
    else if ( this->m_client->SendData( buffer, dataSize ) <= 0 )
    {
        WarningPrint( "Connection to External Controller is lost." );
        delete[] buffer;
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return;
    }
    delete[] buffer;
    this->m_clientThreadMutex.unlock();
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

    this->m_clientThreadMutex.lock();
    
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

    if ( this->m_client == nullptr )
    {
        WarningPrint( "Connection to External Controller is lost." );
        delete[] buffer;
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return;
    }
    else if ( this->m_client->SendData( buffer, dataSize ) <= 0 )
    {
        WarningPrint( "Connection to External Controller is lost." );
        delete[] buffer;
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
    }
    delete[] buffer;
    LOG_FUNCTION_END();
}

void
ControlManager::DeleteClientThread( void )
{
    LOG_FUNCTION_START();
    this->m_clientThreadMutex.lock();
    auto clientPointer = this->m_client;
    this->m_client = nullptr;
    delete clientPointer;
    LOG_FUNCTION_END();
}
