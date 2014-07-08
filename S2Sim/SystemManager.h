/**
 * @file SystemManager.h
 * Defines the SystemManager class.
 *  @date Oct 25, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include <map>
#include <mutex>
#include <memory>
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
     *  Defines the system time step size in seconds.
     */
        typedef unsigned int TSystemTimeStep;
    
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
     *  Structure holding the client information for each interval and client.
     */
        struct ClientInformation
        {
            /**
             *  Real consumption of the client within the interval.
             */
            TWattage realConsumption;
            
            /**
             *  Predicted consumption for the client for the next intervals.
             */
            TWattage predictedConsumption;
            
            /**
             *  Number of data intervals including and after this interval.
             */
            TNumberOfDataPoints numberOfDataPoints;
            
            /**
             *  Default constructor for std::map compatibility.
             */
            ClientInformation( void ){}
            
            /**
             *  Copy constructor for std::map compatibility.
             *
             *  @param copy Copied instance.
             */
            ClientInformation( const ClientInformation & copy ) : realConsumption( copy.realConsumption ),
                                                                  predictedConsumption( copy.predictedConsumption ),
                                                                  numberOfDataPoints( copy.numberOfDataPoints )
            {}
        };
    
    /**
     *  Defines the mapping from ClientId->Consumption.
     */
        typedef std::map<TClientId, ClientInformation> TDataMap;
    
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
     *  The current system time step size.
     */
        TSystemTimeStep m_systemTimeStep;

    /**
     *  This variable contains the consumption information for all clients for the future. It is a mapping from time->Client/Data. This allows us to get the consumption of any client at any time. This simplifies the asynchronous client consumption drastically.
     */
        TSystemMap m_systemMap;
    
    /**
     *  Mutex protecting the data map.
     */
        std::mutex m_systemDataLock;
    
    /**
     *  Timed mutex waiting for the consumption information of all clients.
     */
        std::timed_mutex m_clientTimedMutex;
    
    /**
     *  Defines the time to wait for the data of clients.
     */
        TSystemTime m_clientTimeout;

    private:
    /**
     *  Private constructor for singleton implementation.
     */
        SystemManager( void );
    
    /**
     *  Checks the client keep alive situations and disconnects if necessary.
     */
        void
        CheckSystemKeepAlive( void );

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
     *  Returns the current system time step size.
     *
     *  @return Current system time step size.
     */
        TSystemTimeStep
        GetSystemTimeStep( void ) const
        {
            return ( this->m_systemTimeStep );
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
                      std::shared_ptr<TDataPoint> dataPoints );

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
     *  @brief Used to register a single consumption information for the next time step.
     
     The method is mostly used for synchronous consumption registration. The consumption for the next time interval is registered.
     *
     *  @param clientId  Unique client id of the consumer.
     *  @param numberOfDataPoints Number of consumption points.
     *  @param dataPoints Consumption for the next time interval and the predicted consumptions.
     */
        void
        RegisterData( const TClientId clientId,
                      const TNumberOfDataPoints numberOfDataPoints,
                      std::shared_ptr<TDataPoint> dataPoints );


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
    
    /**
     *  @brief Sets the system mode and changes the client timeout value.
     *
     *  @param systemMode New value of the system mode.
     */
        void
        SetSystemMode( const TSystemMode systemMode );
    
    /**
     *  @brief Returns the consumption of the client in the current interval.
     *
     *  @param clientId Id of the requested client.
     *  @return Consumption in the current interval.
     */
        TDataPoint
        GetCurrentConsumption( const TClientId clientId );
    
    /**
     *  @brief Returns the predicted consumption of the client in the desired interval.
     *
     *  @param clientId Id of the requested client.
     *  @param interval Time in reference to the current time.
     *  @return Consumption in the current interval.
     */
        TDataPoint
        GetPredictionConsumption( const TClientId clientId, const TSystemTime interval );

    
    /**
     *  @brief Returns the prediction horizon of the client.
     *
     *  @param clientId Id of the requested client.
     *  @return Number of data points including the current interval.
     */
        TNumberOfDataPoints
        GetNumberOfConsumptions( const TClientId clientId );

    /**
     *  @brief Sets the consumption to the prediction values on Matlab Manager to get the deviations.
     *
     *  @param predictionTime System time to be predicted.
     */
        void
        SetConsumptionsToPredictionTime( const TSystemTime predictionTime );
    
};

#endif /* SYSTEMMANAGER_H_ */
