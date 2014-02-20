/*
 * SystemManager.h
 *
 *  Created on: Oct 25, 2013
 *      Author: Alper
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

SystemManager&
GetSystemManager( void );

class SystemManager
{
    friend SystemManager& GetSystemManager( void );
    public:
        typedef unsigned int TSystemTime;
        typedef unsigned short TSystemMode;
        enum SystemModeValues
        {
            SimulationMode = ( TSystemMode )0x0001,
            RealTimeMode = ( TSystemMode )0x0002
        };

        typedef MessageHeader::TId TClientId;
        typedef Asynchronous::ClientConnectionRequest::TClientName TClientName;
        typedef Asynchronous::ClientData::TDataPoint TDataPoint;
        typedef Asynchronous::ClientData::TNumberOfDataPoints TNumberOfDataPoints;

        typedef unsigned int TVoltage;
        typedef TDataPoint TWattage;

    private:
        typedef std::map<TClientId, TDataPoint> TDataMap;
        typedef std::map<TSystemTime, TDataMap> TSystemMap;

    private:
        TSystemTime m_systemTime;
        TSystemMode m_systemMode;

        TSystemMap m_systemMap;

    private:
        SystemManager( void );

    public:
        TSystemTime
        GetSystemTime( void ) const
        {
            return ( this->m_systemTime );
        }

        TSystemMode
        GetSystemMode( void ) const
        {
            return ( this->m_systemMode );
        }

        void
        RegisterData( const TClientId clientId,
                      const TSystemTime startTime,
                      const TSystemTime resolution,
                      const TNumberOfDataPoints numberOfDataPoints,
                      TDataPoint* dataPoints );

        void
        RegisterData( const TClientId clientId,
                      TDataPoint dataPoint );

        void
        AdvanceTimeStep( void );

};

#endif /* SYSTEMMANAGER_H_ */
