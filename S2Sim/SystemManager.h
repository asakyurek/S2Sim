/**
 * @file SystemManager.h
 * Defines the SystemManager class.
 *  @date Oct 25, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include <map>
#include "ClientManager.h"
#include "MatlabManager.h"
#include "ControlManager.h"
#include "LogPrint.h"

using namespace TerraSwarm;

class ClientManager;
class ControlManager;
class SystemManager;
class MatlabManager;

/**
 *  Singleton function that returns the only instance of SystemManager.
 *
 *  @return The only instance of SystemManager.
 */
SystemManager&
GetSystemManager( void );

/**
 *  @brief Manages the various components of the system and timing.
 
    This class coordinates the other components within S2Sim and manages the timing of consumption information from both async and synchronous clients.
 */
class SystemManager
{
    /**
     *  Friend function to implement the singleton.
     *
     *  @return Returns the only instance of SystemManager.
     */
    friend SystemManager& GetSystemManager( void );
    
    public:
    /**
     *  Defines the type for System Time in epoch format.
     */
        typedef unsigned int TSystemTime;
    
    /**
     *  Defines the working mode of the system.
     */
        typedef unsigned short TSystemMode;
    
    /**
     *  Defines the values for the TSystemMode type.
     */
        enum SystemModeValues
        {
            SimulationMode = ( TSystemMode )0x0001, /**< Indicates that the system expects an external signal to start. */
            RealTimeMode = ( TSystemMode )0x0002 /**< Indicates that the system is working in real-time, even without external signaling. Not implemented. */
        };

    /**
     *  Redefines the unique client id type for rapid development.
     */
        typedef MessageHeader::TId TClientId;
    
    /**
     *  Redefines the object name for rapid development.
     */
        typedef Asynchronous::ClientConnectionRequest::TClientName TClientName;
    
    /**
     *  Redefines the data point type for rapid development.
     */
        typedef Asynchronous::ClientData::TDataPoint TDataPoint;
    
    /**
     *  Redefines the number of data points type for rapid development.
     */
        typedef Asynchronous::ClientData::TNumberOfDataPoints TNumberOfDataPoints;

    /**
     *  Defines the voltage information type.
     */
        typedef TDataPoint TVoltage;
    
    /**
     *  Defines the wattage consumption type.
     */
        typedef TDataPoint TWattage;

    private:
    /**
     *  Defines the mapping from ClientId->Consumption.
     */
        typedef std::map<TClientId, TDataPoint> TDataMap;
    
    /**
     *  Defines the mappting from Time->(ClientId->Consumption).
     */
        typedef std::map<TSystemTime, TDataMap> TSystemMap;

    private:
    /**
     *  The current system time, incremented at each time step.
     */
        TSystemTime m_systemTime;
    
    /**
     *  The current working mode of the system.
     */
        TSystemMode m_systemMode;

    /**
     *  This variable contains the consumption information for all clients for the future. It is a mapping from time->Client/Data. This allows us to get the consumption of any client at any time. This simplifies the asynchronous client consumption drastically.
     */
        TSystemMap m_systemMap;

    private:
    /**
     *  Private constructor for singleton implementation.
     */
        SystemManager( void );

    public:
    /**
     *  Returns the current system time.
     *
     *  @return Current system time.
     */
        TSystemTime
        GetSystemTime( void ) const
        {
            return ( this->m_systemTime );
        }

    /**
     *  Returns the current system working mode.
     *
     *  @return Current system working mode.
     */
        TSystemMode
        GetSystemMode( void ) const
        {
            return ( this->m_systemMode );
        }

    /**
     *  @brief Used to register multiple consumption information.
     
        This method is mostly used for asynchronous consumption registration. Multiple consumption data points are fed into the SystemManager::m_systemMap.
     *
     *  @param clientId           Unique client id for the consumer.
     *  @param startTime          Starting time of the consumption map.
     *  @param resolution         Time resolution between consecutive consumptions.
     *  @param numberOfDataPoints Number of consumption data points.
     *  @param dataPoints         Buffer containing consumption data points.
     */
        void
        RegisterData( const TClientId clientId,
                      const TSystemTime startTime,
                      const TSystemTime resolution,
                      const TNumberOfDataPoints numberOfDataPoints,
                      TDataPoint* dataPoints );

    /**
     *  @brief Used to register a single consumption information for the next time step.
     
        The method is mostly used for synchronous consumption registration. The consumption for the next time interval is registered.
     *
     *  @param clientId  Unique client id of the consumer.
     *  @param dataPoint Consumption for the next time interval.
     */
        void
        RegisterData( const TClientId clientId,
                      TDataPoint dataPoint );

    /**
     *  @brief Main time iteration of the system.
     
        This method is the main time iteration of the whole system. The workflow is as follows:
        - Delete the previous time step information.
        - Get the current time consumption information.
        - Set the consumption information in OpenDSS.
        - Advance the time in OpenDSS.
        - Invoke the External Controller for a decision.
        - Wait for the External Controller to finish its decision.
     */
        void
        AdvanceTimeStep( void );

};

#endif /* SYSTEMMANAGER_H_ */
