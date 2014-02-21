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
                                       m_systemMode( SimulationMode )
{
    LOG_FUNCTION_START();
    LOG_FUNCTION_END();
}

void
SystemManager::RegisterData( const TClientId clientId,
                             const TSystemTime startTime,
                             const TSystemTime resolution,
                             const TNumberOfDataPoints numberOfDataPoints,
                             TDataPoint* dataPoints )
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
    for ( TSystemTime currentTime = realStartTime; currentTime < endTime; ++currentTime )
    {
        this->m_systemMap[currentTime][clientId] = dataPoints[(currentTime - startTime)/resolution];
    }
    delete[] dataPoints;
    LOG_FUNCTION_END();
}

void
SystemManager::RegisterData( const TClientId clientId,
                             TDataPoint dataPoint )
{
    LOG_FUNCTION_START();
    LogPrint( "Registering Synchronous Data for Client ", clientId, " as: ", dataPoint );
    this->m_systemMap[this->m_systemTime][clientId] = dataPoint;
    LOG_FUNCTION_END();
}

void
SystemManager::AdvanceTimeStep( void )
{
    LOG_FUNCTION_START();
    LogPrint( "Start preparation for next time step by deleting previous step" );
    this->m_systemMap.erase( this->m_systemTime - 1 );
    TDataMap currentDataMap = this->m_systemMap[this->m_systemTime];
    LogPrint( "Send consumption information to OpenDSS" );
    for ( TDataMap::iterator client = currentDataMap.begin();
          client != currentDataMap.end();
          ++client )
    {
        LogPrint( "Set Wattage for Next Client" );
        GetMatlabManager().SetWattage( GetControlManager().GetClientName( client->first ), client->second );
        LogPrint( "Wattage set for the client" );
    }
    LogPrint( "Advance time on OpenDSS" );
    GetMatlabManager().AdvanceTimeStep();
    ++this->m_systemTime;

    LogPrint( "Invoke External Controller for a Decision" );
    GetControlManager().MakeDecision();
    GetControlManager().WaitUntilReady();
    LogPrint( "Current time frame finished" );
}
