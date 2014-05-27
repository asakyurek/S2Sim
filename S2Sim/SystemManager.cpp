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
    }
    this->m_systemDataLock.unlock();
    LOG_FUNCTION_END();
}

void
SystemManager::RegisterData( const TClientId clientId,
                             TDataPoint dataPoint )
{
    LOG_FUNCTION_START();
    LogPrint( "Registering Synchronous Data for Client ", clientId, " as: ", dataPoint );
    this->m_systemDataLock.lock();
    this->m_systemMap[this->m_systemTime][clientId].realConsumption = dataPoint;
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
    this->m_systemMap[this->m_systemTime][clientId].predictedConsumption = dataPoints;
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
    this->m_clientTimedMutex.try_lock_for( std::chrono::seconds( this->m_clientTimeout ) );
    
    LogPrint( "Start preparation for next time step by deleting previous step" );
    this->m_systemDataLock.lock();
    if ( this->m_systemMap.find( this->m_systemTime - 1 ) != this->m_systemMap.end() )
    {
        this->m_systemMap.erase( this->m_systemTime - 1 );
    }
    TDataMap currentDataMap = this->m_systemMap[this->m_systemTime];
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
    ++this->m_systemTime;

    LogPrint( "Invoke External Controller for a Decision" );
    GetControlManager().MakeDecision();
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
