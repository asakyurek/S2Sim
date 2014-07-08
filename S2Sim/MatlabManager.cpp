/**
 * @file MatlabManager.cpp
 * Implements the MatlabManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "MatlabManager.h"

using namespace TerraSwarm;

MatlabManager &
GetMatlabManager( void )
{
    static MatlabManager matlabManager;
    return ( matlabManager );
}

void
MatlabNotificationHandler( ThreadedTCPConnectedClient* acceptedClient )
{
    LOG_FUNCTION_START();
    LogPrint( "OpenDSS MATLAB Controller connected as Client" );
    GetMatlabManager().SetClient( acceptedClient );
    LOG_FUNCTION_END();
}

void
MatlabReceiveHandler( ThreadedTCPConnectedClient* client,
                      ThreadedTCPConnectedClient::TBuffer buffer,
                      ThreadedTCPConnectedClient::TNumberOfBytes  size )
{
    LOG_FUNCTION_START();
    LogPrint( "OpenDSS MATLAB Controller data received. Data size: ", size );
    GetMatlabManager().ProcessData( buffer, size );
    LOG_FUNCTION_END();
}

MatlabManager::MatlabManager( void ) : m_client( NULL ),
                                       m_clientPresentInformation( false ),
                                       m_clientWattageInformation( 0 ),
                                       m_clientVoltageInformation( 0 ),
                                       m_clientVoltageDeviationInformation( 0 )
{
    LOG_FUNCTION_START();
    LogPrint( "OpenDSS MATLAB Manager alive" );
    this->m_clientVoltageMutex.lock();
    this->m_clientWattageMutex.lock();
    this->m_clientPresenceMutex.lock();
    this->m_clientVoltageDeviationMutex.lock();
    this->m_clientVoltageDeviationAndConsumptionMutex.lock();
    this->m_clientThreadMutex.lock();
    this->m_server.SetPort( 26998 );
    this->m_server.SetNotificationCallback( &MatlabNotificationHandler );
    LOG_FUNCTION_END();
}

void
MatlabManager::ProcessData( void* buffer, size_t size )
{
    LOG_FUNCTION_START();
    LogPrint( "Processing data of size: " , size );
    
    int remainingSize = ( int )size;

    if ( remainingSize <= 0 )
    {
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return;
    }
    
    while ( remainingSize > 0 )
    {
        TMessageType messageType;
        char* currentAddress = ( char* )buffer;
        memcpy( &messageType, currentAddress, sizeof( TMessageType ) );
        currentAddress += sizeof( TMessageType );

        messageType = ntohl( messageType );

        if ( messageType == ClientCheckResultType )
        {
            remainingSize -= sizeof( TMessageType ) + sizeof( TClientCheckResult );
            LogPrint( "Message Type: Client Presence Result" );
            TClientCheckResult checkResult;
            memcpy( &checkResult, currentAddress, sizeof( TClientCheckResult ) );
            currentAddress += sizeof( TClientCheckResult );

            checkResult = ntohl( checkResult );
            if ( checkResult == ClientExists )
            {
                LogPrint( "OpenDSS says: Client Exists" );
                this->m_clientPresentInformation = true;
            }
            else
            {
                WarningPrint( "OpenDSS says: Client does not Exist" );
                this->m_clientPresentInformation = false;
            }
            LogPrint( "Releasing Client Presence Semaphore" );
            this->m_clientPresenceMutex.unlock();
        }
        else if ( messageType == ClientWattageResultType )
        {
            remainingSize -= sizeof( TMessageType ) + sizeof( TWattage );
            LogPrint( "Message Type: Client Wattage Result" );
            TWattage wattageResult;
            memcpy( &wattageResult, currentAddress, sizeof( TWattage ) );
            currentAddress += sizeof( TWattage );

            wattageResult = ntohl( wattageResult );

            LogPrint( "OpenDSS says: Client Wattage = ", wattageResult );
            this->m_clientWattageInformation = wattageResult;

            LogPrint( "Releasing Client Wattage Semaphore" );
            this->m_clientWattageMutex.unlock();
        }
        else if ( messageType == ClientVoltageResultType )
        {
            remainingSize -= sizeof( TMessageType ) + sizeof( TVoltage );
            LogPrint( "Message Type: Client Voltage Result" );
            TVoltage voltageResult;
            memcpy( &voltageResult, currentAddress, sizeof( TVoltage ) );
            currentAddress += sizeof( TVoltage );

            voltageResult = ntohl( voltageResult );
            this->m_clientVoltageInformation = voltageResult;
            LogPrint( "OpenDSS says: Client Voltage = ", this->m_clientVoltageInformation );

            LogPrint( "Releasing Client Voltage Semaphore" );
            this->m_clientVoltageMutex.unlock();
        }
        else if ( messageType == ClientVoltageDeviationResultType )
        {
            remainingSize -= sizeof( TMessageType ) + sizeof( TVoltage );
            LogPrint( "Message Type: Client Voltage Deviation Result" );
            TVoltage voltageResult;
            memcpy( &voltageResult, currentAddress, sizeof( TVoltage ) );
            currentAddress += sizeof( TVoltage );
            
            voltageResult = ntohl( voltageResult );
            this->m_clientVoltageDeviationInformation = voltageResult;
            LogPrint( "OpenDSS says: Client Voltage Deviation = ", this->m_clientVoltageDeviationInformation );
            
            LogPrint( "Releasing Client Voltage Deviation Semaphore" );
            this->m_clientVoltageDeviationMutex.unlock();
        }
        else if ( messageType == ClientVoltageDeviationAndConsumptionResultType )
        {
            remainingSize -= sizeof( TMessageType ) + sizeof( TVoltage ) + sizeof( TWattage );
            LogPrint( "Message Type: Client Voltage Deviation and Consumption Result" );
            TVoltage voltageResult;
            memcpy( &voltageResult, currentAddress, sizeof( TVoltage ) );
            currentAddress += sizeof( TVoltage );
            
            voltageResult = ntohl( voltageResult );
            this->m_clientVoltageDeviationInformation = voltageResult;
            
            TWattage wattageResult;
            memcpy( &wattageResult, currentAddress, sizeof( TWattage ) );
            currentAddress += sizeof( TWattage );
            
            wattageResult = ntohl( wattageResult );
            this->m_clientWattageInformation = wattageResult;
            
            
            LogPrint( "OpenDSS says: Client Voltage Deviation = ", this->m_clientVoltageDeviationInformation );
            LogPrint( "OpenDSS says: Client Consumption = ", this->m_clientWattageInformation );
            
            LogPrint( "Releasing Client Voltage Deviation and Consumption Semaphore" );
            this->m_clientVoltageDeviationAndConsumptionMutex.unlock();
        }
        else
        {
            ErrorPrint( "Unknwon Message Type from OPENDSS" );
        }
        buffer = currentAddress;
    }
    LOG_FUNCTION_END();
}

bool
MatlabManager::IsClientPresent( const std::string & clientName )
{
    LOG_FUNCTION_START();
    LogPrint( "Checking with OpenDSS for client presence of: \"", clientName, "\"" );

    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return ( -1 );
    }

    this->m_clientPresentInformation = false;

    size_t bufferSize = sizeof(int) + sizeof(TMessageType) + clientName.length();
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );

    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );

    TMessageType messageType = ClientCheckRequestType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );

    memcpy( currentAddress, clientName.c_str(), clientName.length() );
    currentAddress += clientName.length();
    LogPrint( "Sending Client Presence Message to OpenDSS" );
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return ( this->m_clientPresentInformation );
    }
    delete[] buffer;
    this->m_clientThreadMutex.unlock();

    LogPrint( "Waiting for Client Presence Mutex" );
    this->m_clientPresenceMutex.lock();
    LogPrint( "Client Presence Mutex taken" );
    LOG_FUNCTION_END();
    return ( this->m_clientPresentInformation );
}

void
MatlabManager::SetWattage( const TClientName & clientName, const TWattage wattage )
{
    LOG_FUNCTION_START();
    LogPrint( "Setting the Wattage value of \"", clientName, "\" to ", wattage );
    WarningPrint("Lock");
    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return;
    }
    size_t bufferSize = sizeof( int ) + sizeof( TMessageType ) + sizeof( TWattage ) + clientName.length();
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );

    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );

    TMessageType messageType = ClientSetWattageType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );

    TWattage networkWattage = htonl( wattage );
    memcpy( currentAddress, &networkWattage, sizeof( TWattage ) );
    currentAddress += sizeof( TWattage );

    memcpy( currentAddress, clientName.c_str(), clientName.length() );
    currentAddress += clientName.length();

    LogPrint( "Sending Client Set Wattage Message to OpenDSS" );
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        WarningPrint("UNLock");
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return;
    }
    delete[] buffer;
    WarningPrint("UNLock");
    this->m_clientThreadMutex.unlock();
    LOG_FUNCTION_END();
}

MatlabManager::TWattage
MatlabManager::GetWattage( const TClientName & clientName )
{
    LOG_FUNCTION_START();
    LogPrint( "Getting the Wattage value of \"", clientName, "\"" );

    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return ( -1 );
    }

    size_t bufferSize = sizeof( int ) + sizeof( TMessageType ) + clientName.length();
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );

    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );

    TMessageType messageType = ClientGetWattageType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );

    memcpy( currentAddress, clientName.c_str(), clientName.length() );
    currentAddress += clientName.length();

    LogPrint( "Sending Client Get Wattage Message to OpenDSS" );
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return ( this->m_clientWattageInformation );;
    }
    delete[] buffer;
    this->m_clientThreadMutex.unlock();

    LogPrint( "Waiting for Client Wattage Mutex" );
    this->m_clientWattageMutex.lock();
    LogPrint( "Client Wattage Mutex taken" );

    LOG_FUNCTION_END();
    return ( this->m_clientWattageInformation );
}

MatlabManager::TVoltage
MatlabManager::GetVoltage( const TClientName & clientName )
{
    LOG_FUNCTION_START();
    LogPrint( "Getting the Voltage value of \"", clientName, "\"" );

    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return ( -1 );
    }

    size_t bufferSize = sizeof( int ) + sizeof( TMessageType ) + clientName.length();
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );

    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );

    TMessageType messageType = ClientGetVoltageType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );

    memcpy( currentAddress, clientName.c_str(), clientName.length() );
    currentAddress += clientName.length();

    LogPrint( "Sending Client Get Voltage Message to OpenDSS" );
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return ( this->m_clientVoltageInformation );
    }
    delete[] buffer;
    this->m_clientThreadMutex.unlock();

    LogPrint( "Waiting for Client Voltage Mutex" );
    this->m_clientVoltageMutex.lock();
    LogPrint( "Client Voltage Mutex taken" );

    LOG_FUNCTION_END();
    return ( this->m_clientVoltageInformation );
}

MatlabManager::TVoltage
MatlabManager::GetVoltageDeviation( const TClientName & clientName )
{
    LOG_FUNCTION_START();
    LogPrint( "Getting the Voltage Deviation value of \"", clientName, "\"" );
    
    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return ( -1 );
    }
    
    size_t bufferSize = sizeof( int ) + sizeof( TMessageType ) + clientName.length();
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );
    
    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );
    
    TMessageType messageType = ClientGetVoltageDeviationType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );
    
    memcpy( currentAddress, clientName.c_str(), clientName.length() );
    currentAddress += clientName.length();
    
    LogPrint( "Sending Client Get Voltage Deviation Message to OpenDSS" );
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return ( this->m_clientVoltageDeviationInformation );
    }
    delete[] buffer;
    this->m_clientThreadMutex.unlock();
    
    LogPrint( "Waiting for Client Voltage Deviation Mutex" );
    this->m_clientVoltageMutex.lock();
    LogPrint( "Client Voltage Deviation Mutex taken" );
    
    LOG_FUNCTION_END();
    return ( this->m_clientVoltageDeviationInformation );
}

std::pair<MatlabManager::TVoltage, MatlabManager::TWattage>
MatlabManager::GetVoltageDeviationAndConsumption( const TClientName & clientName )
{
    LOG_FUNCTION_START();
    LogPrint( "Getting the Voltage Deviation and consumption value of \"", clientName, "\"" );
    
    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return ( std::make_pair( -1, -1 ) );
    }
    
    size_t bufferSize = sizeof( int ) + sizeof( TMessageType ) + clientName.length();
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );
    
    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );
    
    TMessageType messageType = ClientGetVoltageDeviationAndConsumptionType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );
    
    memcpy( currentAddress, clientName.c_str(), clientName.length() );
    currentAddress += clientName.length();
    
    LogPrint( "Sending Client Get Voltage Deviation and Consumption Message to OpenDSS" );
    
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
        this->m_clientThreadMutex.unlock();
        this->DeleteClientThread();
        LOG_FUNCTION_END();
        return ( std::make_pair( this->m_clientVoltageDeviationInformation, this->m_clientWattageInformation ) );
    }
    this->m_clientThreadMutex.unlock();
    delete[] buffer;
    
    LogPrint( "Waiting for Client Voltage Deviation and Consumption Mutex" );
    this->m_clientVoltageDeviationAndConsumptionMutex.lock();
    LogPrint( "Client Voltage Deviation and Consumption Mutex taken" );
    
    LOG_FUNCTION_END();
    return ( std::make_pair( this->m_clientVoltageDeviationInformation, this->m_clientWattageInformation ) );
}

void
MatlabManager::AdvanceTimeStep( void )
{
    LOG_FUNCTION_START();
    LogPrint( "Advancing Time Step in OpenDSS" );
    this->m_clientThreadMutex.lock();
    if ( this->m_client == nullptr )
    {
        ErrorPrint( "No OpenDSS MATLAB connection" );
        this->m_clientThreadMutex.unlock();
        LOG_FUNCTION_END();
        return;
    }

    size_t bufferSize = sizeof( int ) + sizeof( TMessageType );
    char* buffer = new char[bufferSize];
    char* currentAddress = buffer;
    int messageLength = htonl( bufferSize );

    memcpy( currentAddress, &messageLength, sizeof( int ) );
    currentAddress += sizeof( int );

    TMessageType messageType = AdvanceTimeStepType;
    messageType = htonl( messageType );
    memcpy( currentAddress, &messageType, sizeof( TMessageType ) );
    currentAddress += sizeof( TMessageType );

    LogPrint( "Sending Time Step Advancement Message to OpenDSS" );
    if ( this->m_client->SendData( buffer, bufferSize ) <= 0 )
    {
        ErrorPrint( "Send failed! OpenDSS MATLAB Connection is broken" );
        delete[] buffer;
        WarningPrint( "OpenDSS MATLAB Manager disconnected" );
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
MatlabManager::DeleteClientThread( void )
{
    LOG_FUNCTION_START();
    this->m_clientThreadMutex.lock();
    auto clientPointer = this->m_client;
    this->m_client = nullptr;
    delete clientPointer;
    LOG_FUNCTION_END();
}
