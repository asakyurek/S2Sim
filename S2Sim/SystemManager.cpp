/**
 * @file SystemManager.cpp
 * Implements the SystemManager class.
 *  @date Oct 25, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "SystemManager.h"

using namespace TerraSwarm;

SystemManager&
GetSystemManager( void )
{
    static SystemManager systemManager;
    return ( systemManager );
}

SystemManager::SystemManager( void ) : m_systemTime( 0 ),
                                       m_systemTimeStep( 1 )
{
    LOG_FUNCTION_START();
    this->SetSystemMode( SimulationMode );
    this->m_clientTimedMutex.lock();
    LOG_FUNCTION_END();
}

void
SystemManager::RegisterData( const TClientId clientId,
                             const TSystemTime startTime,
                             const TSystemTime resolution,
                             const TNumberOfDataPoints numberOfDataPoints,
                             std::shared_ptr<TDataPoint> dataPoints )
{
    LOG_FUNCTION_START();
    LogPrint( "Registering Client ", clientId, "'s ", numberOfDataPoints, " data points. Staring at ", startTime, " with resolution: ", resolution );
    TSystemTime endTime = startTime + resolution * numberOfDataPoints;
    if ( endTime < this->m_systemTime )
    {
        WarningPrint( "Outdated data received. Current Time: ", this->m_systemTime, " Data End Time: ", endTime );
        return;
    }

    TSystemTime realStartTime = std::max( startTime, this->m_systemTime );
    LogPrint( "Registering from start time: ", realStartTime );
    this->m_systemDataLock.lock();
    TDataPoint* dataPointer = dataPoints.get();
    for ( TSystemTime currentTime = realStartTime; currentTime < endTime; ++currentTime )
    {
        this->m_systemMap[currentTime][clientId].realConsumption = dataPointer[(currentTime - startTime)/resolution];
        this->m_systemMap[currentTime][clientId].numberOfDataPoints = 1;
    }
    this->m_systemDataLock.unlock();
    LOG_FUNCTION_END();
}

void
SystemManager::RegisterData( const TClientId clientId,
                             TDataPoint dataPoint )
{
    LOG_FUNCTION_START();
    LogPrint( "Registering Synchronous Data for Client ", clientId, " as: ", dataPoint, " time: ", this->m_systemTime );
    this->m_systemDataLock.lock();
    this->m_systemMap[this->m_systemTime][clientId].realConsumption = dataPoint;
    this->m_systemMap[this->m_systemTime][clientId].numberOfDataPoints = 1;
    this->m_systemDataLock.unlock();
    
    if ( this->m_systemMap[this->m_systemTime].size() == GetConnectionManager().GetNumberOfSynchronousClients() )
    {
        this->m_clientTimedMutex.unlock();
    }
    
    LOG_FUNCTION_END();
}

void
SystemManager::RegisterData( const TClientId clientId,
                             const TNumberOfDataPoints numberOfDataPoints,
                             std::shared_ptr<TDataPoint> dataPoints )
{
    LOG_FUNCTION_START();
    LogPrint( "Registering Synchronous Extended Data for Client ", clientId, " N: ", numberOfDataPoints );
    this->m_systemDataLock.lock();
    TDataPoint* dataPointer = dataPoints.get();
    this->m_systemMap[this->m_systemTime][clientId].realConsumption = dataPointer[0];
    this->m_systemMap[this->m_systemTime][clientId].numberOfDataPoints = numberOfDataPoints;
    
    /* Set the predictions and the receeding horizons. */
    TSystemTime selectedTime = this->m_systemTime + 1;
    for ( TDataPoint timeIndex = 1; timeIndex < numberOfDataPoints; ++timeIndex )
    {
        this->m_systemMap[selectedTime][clientId].realConsumption = dataPointer[0];
        this->m_systemMap[selectedTime][clientId].predictedConsumption = dataPointer[timeIndex];
        this->m_systemMap[selectedTime][clientId].numberOfDataPoints = numberOfDataPoints - timeIndex;
        ++selectedTime;
    }
    this->m_systemDataLock.unlock();
    
    if ( this->m_systemMap[this->m_systemTime].size() == GetConnectionManager().GetNumberOfSynchronousClients() )
    {
        this->m_clientTimedMutex.unlock();
    }
    
    LOG_FUNCTION_END();
}

void
SystemManager::AdvanceTimeStep( void )
{
    LOG_FUNCTION_START();
    
    LogPrint( "Waiting for clients for: ", this->m_clientTimeout );
    bool lockResult = this->m_clientTimedMutex.try_lock_for( std::chrono::seconds( this->m_clientTimeout ) );
    GetConnectionManager().PrintClientList();
    LogPrint( "Start preparation for time: ",this->m_systemTime , " by deleting previous step" );
    this->m_systemDataLock.lock();
    if ( this->m_systemMap.find( this->m_systemTime - 1 ) != this->m_systemMap.end() )
    {
        this->m_systemMap.erase( this->m_systemTime - 1 );
    }
    TDataMap currentDataMap = this->m_systemMap[this->m_systemTime];
    
    std::vector<TClientId> clientList;
    if ( !lockResult )
    {
        for ( auto clientIterator = currentDataMap.begin(); clientIterator != currentDataMap.end(); ++clientIterator )
        {
            LogPrint( clientIterator->first, " is alive" );
            clientList.push_back( clientIterator->first );
        }
        
        GetConnectionManager().UpdateAliveClients( clientList );
    }
    
    this->m_systemDataLock.unlock();
    
    LogPrint( "Send consumption information to OpenDSS" );
    for ( TDataMap::iterator client = currentDataMap.begin();
          client != currentDataMap.end();
          ++client )
    {
        LogPrint( "Set Wattage for Next Client" );
        GetMatlabManager().SetWattage( GetControlManager().GetClientName( client->first ), client->second.realConsumption );
        LogPrint( "Wattage set for the client" );
    }
    LogPrint( "Advance time on OpenDSS" );
    GetMatlabManager().AdvanceTimeStep();

    LogPrint( "Invoke External Controller for a Decision" );
    GetControlManager().MakeDecision();
    ++this->m_systemTime;
    
    GetControlManager().WaitUntilReady();
    LogPrint( "Current time frame finished" );
    
    LOG_FUNCTION_END();
}

void
SystemManager::SetSystemMode( const TSystemMode systemMode )
{
    this->m_systemMode = systemMode;
    if ( this->m_systemMode == RealTimeMode )
    {
        this->m_clientTimeout = 1;
    }
    else if ( this->m_systemMode == SimulationMode )
    {
        this->m_clientTimeout = 10;
    }
    else
    {
        ErrorPrint( "Unknown System Mode: ", systemMode );
    }
}

void
SystemManager::SetConsumptionsToPredictionTime( const TSystemTime predictionTime )
{
    this->m_systemDataLock.lock();
    TDataMap currentDataMap = this->m_systemMap[predictionTime];
    this->m_systemDataLock.unlock();
    
    LogPrint( "Send prediction information to OpenDSS for time ", predictionTime );
    
    if ( predictionTime == this->m_systemTime )
    {
        for ( TDataMap::iterator client = currentDataMap.begin();
             client != currentDataMap.end();
             ++client )
        {
            GetMatlabManager().SetWattage( GetControlManager().GetClientName( client->first ), client->second.realConsumption );
        }
    }
    else
    {
        for ( TDataMap::iterator client = currentDataMap.begin();
             client != currentDataMap.end();
             ++client )
        {
            GetMatlabManager().SetWattage( GetControlManager().GetClientName( client->first ), client->second.predictedConsumption );
        }
    }
    LogPrint( "Advance time on OpenDSS" );
    GetMatlabManager().AdvanceTimeStep();
    
}

SystemManager::TDataPoint
SystemManager::GetCurrentConsumption( const TClientId clientId )
{
    return ( this->m_systemMap[this->m_systemTime][clientId].realConsumption );
}

SystemManager::TDataPoint
SystemManager::GetPredictionConsumption( const TClientId clientId, const TSystemTime interval )
{
    if ( this->m_systemMap.find( this->m_systemTime ) != this->m_systemMap.end() )
    {
        if ( this->m_systemMap[this->m_systemTime].find( clientId ) != this->m_systemMap[this->m_systemTime].end() )
        {
            return ( this->m_systemMap[this->m_systemTime + interval][clientId].predictedConsumption );
        }
    }
    return ( -1 );
}

SystemManager::TNumberOfDataPoints
SystemManager::GetNumberOfConsumptions( const TClientId clientId )
{
    if ( this->m_systemMap.find( this->m_systemTime ) != this->m_systemMap.end() )
    {
        if ( this->m_systemMap[this->m_systemTime].find( clientId ) != this->m_systemMap[this->m_systemTime].end() )
        {
            return ( this->m_systemMap[this->m_systemTime][clientId].numberOfDataPoints );
        }
    }
    return ( 0 );
}

void
SystemManager::CheckSystemKeepAlive( void )
{
    
}


