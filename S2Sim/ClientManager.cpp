/**
 * @file ClientManager.cpp
 *  Source file for the ClientManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "ClientManager.h"

ClientManager::TId ClientManager::nextClientId = 1;

ClientManager::ClientManager( void ) : m_isRegistered( IsNotRegistered ), m_client( nullptr ), m_clientId( 0 ), m_clientType( 0 )
{
    LOG_FUNCTION_START();
    LOG_FUNCTION_END();
}

ClientManager::ClientManager( ThreadedTCPConnectedClient* client ) : m_isRegistered( IsNotRegistered ),
                                                                     m_client( client ),
                                                                     m_clientId( 0 ),
                                                                     m_clientType( 0 )
{
    LOG_FUNCTION_START();
    LOG_FUNCTION_END();
}

ClientManager::ClientManager( const ClientManager& copy ) : m_isRegistered( copy.m_isRegistered ),
                                                            m_client( copy.m_client ),
                                                            m_clientId( copy.m_clientId ),
                                                            m_clientType( copy.m_clientType )
{
    LOG_FUNCTION_START();
    LOG_FUNCTION_END();
}

ClientManager::~ClientManager( void )
{
    LOG_FUNCTION_START();
    if ( this->m_client != nullptr )
    {
        delete this->m_client;
    }
    if ( this->m_isRegistered == IsRegistered )
    {
        GetControlManager().UnRegisterClient( this->m_clientId );
        this->m_isRegistered = IsNotRegistered;
    }
    LOG_FUNCTION_END();
}

ClientManager&
ClientManager::operator = ( const ClientManager & copy )
{
    LOG_FUNCTION_START();
    /* Check for self assignment */
    if ( this == &copy )
    {
        return ( *this );
    }
    this->m_isRegistered = copy.m_isRegistered;
    this->m_client = copy.m_client;
    this->m_clientId = copy.m_clientId;
    this->m_clientType = copy.m_clientType;
    LOG_FUNCTION_END();
    return ( *this );
}

void
ClientManager::ConnectionBroken( void )
{
    LOG_FUNCTION_START();
    WarningPrint( "Client Connection broken for: ", this->m_clientId );
    
    if ( this->m_isRegistered == IsRegistered )
    {
        GetControlManager().UnRegisterClient( this->m_clientId );
        this->m_isRegistered = IsNotRegistered;
        WarningPrint( "Deregistered Client, deleting..." );
    }
    
    GetConnectionManager().DeleteClient( this );
    WarningPrint( "Client deleted" );
    LOG_FUNCTION_END();
}

void
ClientManager::MessageReceived( void* data, const size_t dataSize )
{
    LOG_FUNCTION_START();
    LogPrint( "Client Message Received of size: ", dataSize );
    if ( ( ( Asynchronous::ClientConnectionRequest* )data )->CheckMessage() )
    {
        LogPrint( "Asynchronous Client Connection Request Received. Processing..." );
        this->ProcessClientConnectionRequest( ( Asynchronous::ClientConnectionRequest* )data );
    }
    else if ( ( ( Asynchronous::ClientData* )data )->CheckMessage() )
    {
        LogPrint( "Asynchronous Client Data Received. Processing..." );
        this->ProcessClientData( ( Asynchronous::ClientData* )data );
    }
    else if ( ( ( Synchronous::ClientConnectionRequest* )data )->CheckMessage() )
    {
        LogPrint( "Synchronous Client Connection Request Received. Processing..." );
        this->ProcessClientConnectionRequest( ( Synchronous::ClientConnectionRequest* )data );
    }
    else if ( ( ( Synchronous::ClientData* )data )->CheckMessage() )
    {
        LogPrint( "Synchronous Client Data Received. Processing..." );
        this->ProcessClientData( ( Synchronous::ClientData* )data );
    }
    else if ( ( ( Synchronous::GetPrice* )data )->CheckMessage() )
    {
        LogPrint( "Get Price Received. Processing..." );
        this->ProcessGetPrice( ( Synchronous::GetPrice* )data );
    }
    else if ( ( ( Synchronous::DemandNegotiation* )data )->CheckMessage() )
    {
        LogPrint( "Demand Negotiation Received. Processing..." );
        this->ProcessDemandNegotiation( ( Synchronous::DemandNegotiation* )data );
    }
    else if ( ( ( Synchronous::ClientExtendedData* )data )->CheckMessage() )
    {
        LogPrint( "Synchronous Client Extended Data Received. Processing..." );
    }
    else if ( ( ( SystemTimePrompt* )data )->CheckMessage() )
    {
        LogPrint( "System Time Prompt Received. Processing..." );
        this->ProcessSystemTimePrompt( ( SystemTimePrompt* )data );
    }
    else if ( ( ( SystemVersionPrompt* )data )->CheckMessage() )
    {
        LogPrint( "System Version Prompt Received. Processing..." );
        this->ProcessSystemVersionPrompt( ( SystemVersionPrompt* )data );
    }
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessClientConnectionRequest( Asynchronous::ClientConnectionRequest* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Asynchronous Client Connection Request received" );
    Asynchronous::ClientConnectionResponse::TRequestResult requestResult;
    if ( GetMatlabManager().IsClientPresent( data->GetClientName() ) )
    {
        LogPrint( "Client is present, accepting connection request" );
        requestResult = Asynchronous::ClientConnectionResponse::RequestAccepted;
        this->m_clientType = AsynchronousClient;
        this->m_clientId = this->nextClientId;
        this->m_clientName = data->GetClientName();
        LogPrint( "Registering client to the control manager" );
        GetControlManager().RegisterClient( this->m_clientId, data->GetClientName(), this );
        this->m_isRegistered = IsRegistered;
        ++this->nextClientId;
    }
    else
    {
        WarningPrint( "Client is not present, rejecting connection request" );
        requestResult = Asynchronous::ClientConnectionResponse::RequestObjectIdNotFound;
    }
    SystemManager::TSystemMode systemMode = GetSystemManager().GetSystemMode();
    SystemManager::TSystemTime systemTime = GetSystemManager().GetSystemTime();
    ConnectionManager::TNumberOfClients numberOfClients = GetConnectionManager().GetNumberOfClients();

    LogPrint( "Sending connection response message to client" );
    Asynchronous::ClientConnectionResponse* responseData = Asynchronous::ClientConnectionResponse::GetNewClientConnectionResponse( 0x0000,
                                                                                                                                   this->m_clientId,
                                                                                                                                   requestResult,
                                                                                                                                   systemTime,
                                                                                                                                   numberOfClients,
                                                                                                                                   systemMode );
    this->m_client->SendData( responseData, ( int )Asynchronous::ClientConnectionResponse::GetSize() );
    delete[] ( ( char* )responseData );
    if ( requestResult == Asynchronous::ClientConnectionResponse::RequestObjectIdNotFound )
    {
        WarningPrint( "Deleting  temporarily constructed client information for rejected request" );
        GetConnectionManager().DeleteClient( this );
    }
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessClientData( Asynchronous::ClientData* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Processing asynchronous client data" );
    Asynchronous::ClientData::TStartTime startTime = data->GetStartTime();
    Asynchronous::ClientData::TTimeResolution timeResolution = data->GetTimeResolution();
    Asynchronous::ClientData::TNumberOfDataPoints numberOfDataPoints = data->GetNumberOfDataPoints();
    LogPrint( "Client ", this->m_clientId, " sent ", numberOfDataPoints, " starting at time: ", startTime, " with resolution: ", timeResolution );
    GetSystemManager().RegisterData( this->m_clientId, startTime, timeResolution, numberOfDataPoints, data->GetDataPoints() );
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessSystemTimePrompt( SystemTimePrompt *data )
{
    LOG_FUNCTION_START();
    LogPrint( "Processing System Time Prompt" );
    SystemTimeResponse::TSystemTime systemTime = GetSystemManager().GetSystemTime();
    SystemTimeResponse* responseData = SystemTimeResponse::GetNewSystemTimeResponse( 0x0000, 0xFFFF, systemTime );
    this->m_client->SendData( responseData, responseData->GetSize() );
    delete[] ( ( char* )responseData );
    LogPrint( "Deleting this temporary time asking client");
    GetConnectionManager().DeleteClient( this );
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessSystemVersionPrompt( SystemVersionPrompt *data )
{
    LOG_FUNCTION_START();
    LogPrint( "Processing System Version Prompt" );
    SystemVersionResponse* responseData = SystemVersionResponse::GetNewSystemVersionResponse( 0x0000, 0xFFFF, 1, 3 );
    this->m_client->SendData( responseData, responseData->GetSize() );
    delete[] ( ( char* )responseData );
    LogPrint( "Deleting this temporary version asking client");
    GetConnectionManager().DeleteClient( this );
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessClientConnectionRequest( Synchronous::ClientConnectionRequest* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Synchronous Client Connection Request received" );
    Synchronous::ClientConnectionResponse::TRequestResult requestResult;
    if ( GetMatlabManager().IsClientPresent( data->GetClientName() ) )
    {
        LogPrint( "Client is present, accepting request" );
        requestResult = Synchronous::ClientConnectionResponse::RequestAccepted;
        this->m_clientType = SynchronousClient;
        this->m_clientId = this->nextClientId;
        this->m_clientName = data->GetClientName();
        LogPrint( "Registering client to the control manager" );
        GetControlManager().RegisterClient( this->m_clientId, data->GetClientName(), this );
        this->m_isRegistered = IsRegistered;
        ++this->nextClientId;
    }
    else
    {
        WarningPrint( "Client is not present, rejecting connection request" );
        requestResult = Synchronous::ClientConnectionResponse::RequestObjectIdNotFound;
    }
    SystemManager::TSystemMode systemMode = GetSystemManager().GetSystemMode();
    SystemManager::TSystemTime systemTime = GetSystemManager().GetSystemTime();
    SystemManager::TSystemTimeStep systemTimeStep = GetSystemManager().GetSystemTimeStep();
    ConnectionManager::TNumberOfClients numberOfClients = GetConnectionManager().GetNumberOfClients();

    Synchronous::ClientConnectionResponse* responseData = Synchronous::ClientConnectionResponse::GetNewClientConnectionResponse( 0x0000,
                                                                                                                                 this->m_clientId,
                                                                                                                                 requestResult,
                                                                                                                                 systemTime,
                                                                                                                                 numberOfClients,
                                                                                                                                 systemMode,
                                                                                                                                 systemTimeStep );
    this->m_client->SendData( responseData, ( int )Synchronous::ClientConnectionResponse::GetSize() );
    delete[] ( ( char* )responseData );
    if ( requestResult == Synchronous::ClientConnectionResponse::RequestObjectIdNotFound )
    {
        WarningPrint( "Deleting  temporarily constructed client information for rejected request" );
        GetConnectionManager().DeleteClient( this );
    }
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessClientData( Synchronous::ClientData* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Synchronous Client Data received from: ", this->m_clientId );
    Synchronous::ClientData::TDataPoint dataPoint = data->GetDataPoint();
    GetSystemManager().RegisterData( this->m_clientId, dataPoint );
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessClientExtendedData( Synchronous::ClientExtendedData* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Synchronous Client Data received from: ", this->m_clientId );
    Synchronous::ClientExtendedData::TNumberOfDataPoints numberOfDataPoints = data->GetNumberOfDataPoints();
    std::shared_ptr<Synchronous::ClientExtendedData::TDataPoint> dataPoints = data->GetDataPoints();
    GetSystemManager().RegisterData( this->m_clientId, numberOfDataPoints, dataPoints );
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessGetPrice( Synchronous::GetPrice* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Client ", this->m_clientId, " requests the price. Sending request to External Control." );
    GetControlManager().ClientPriceRequest( this->m_clientId );
    LOG_FUNCTION_END();
}

void
ClientManager::ProcessDemandNegotiation( Synchronous::DemandNegotiation* data )
{
    LOG_FUNCTION_START();
    LogPrint( "Client ", this->m_clientId, " sends demand negotiation result. Sending result to External Control." );
    GetControlManager().ClientDemandNegotiation( this->m_clientId,
                                                 data->GetNumberOfDataPoints(),
                                                 data->GetDataPoints() );
    LOG_FUNCTION_END();
}

void
ClientManager::SetCurrentPrice( const TInterval beginInterval, const TNumberOfPriceValues numberOfPriceValues, TPrice* priceValues )
{
    LOG_FUNCTION_START();
    LogPrint( "Price for client ", this->m_clientId, " is set to: ", ntohl( priceValues[0] ), " from time: ", beginInterval, ". Number of: ", numberOfPriceValues );
    Synchronous::SetCurrentPrice* message = Synchronous::SetCurrentPrice::GetNewSetCurrentPrice( 0x0000,
                                                                                                 this->m_clientId,
                                                                                                 beginInterval,
                                                                                                 numberOfPriceValues,
                                                                                                 priceValues );

    this->m_client->SendData( message, message->GetSize() );
    delete[] ( ( char* )message );
    LOG_FUNCTION_END();
}

void
ClientManager::PriceProposal( const TPrice price, const TInterval beginInterval, const TInterval endInterval )
{
    LOG_FUNCTION_START();
    LogPrint( "Price proposal for client ", this->m_clientId, " is negotiated on price: ", price, " from: ", beginInterval, " until: ", endInterval );
    Synchronous::PriceProposal* message = Synchronous::PriceProposal::GetNewPriceProposal( 0x0000,
                                                                                           this->m_clientId,
                                                                                           price,
                                                                                           beginInterval,
                                                                                           endInterval );

    this->m_client->SendData( message, Synchronous::PriceProposal::GetSize() );
    delete[] ( ( char* )message );
    LOG_FUNCTION_END();
}

void
ClientManager::PrintClientInformation( std::ofstream & outputFile ) const
{
    std::string clientTypeString = "Asynchronous";
    if ( this->m_clientType == SynchronousClient )
    {
        clientTypeString = "Synchronous";
    }
    outputFile << this->m_clientId << " " << this->m_clientName << " " << clientTypeString << std::endl;
}

void
ClientManager::StopClientAndClean( void )
{
    this->m_client->StopThread();
    delete this;
}
