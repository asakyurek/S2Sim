/*
 * MatlabManager.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef MATLABMANAGER_H_
#define MATLABMANAGER_H_

#include <iostream>
#include "Semaphore.h"

#include "ThreadedTCPServer.h"
#include "ThreadedTCPConnectedClient.h"
#include "SystemManager.h"

using namespace TerraSwarm;

class SystemManager;
class MatlabManager;

MatlabManager &
GetMatlabManager( void );

void
MatlabReceiveHandler( ThreadedTCPConnectedClient* client,
                      ThreadedTCPConnectedClient::TBuffer buffer,
                      ThreadedTCPConnectedClient::TNumberOfBytes size );

class MatlabManager
{
    friend MatlabManager &
    GetMatlabManager( void );

    private:
        typedef unsigned int TMessageType;
        enum MessageTypeValues
        {
            ClientCheckRequestType = ( TMessageType )0x00000001,
            ClientCheckResultType = ( TMessageType )0x00000002,
            ClientSetWattageType = ( TMessageType )0x00000003,
            ClientGetWattageType = ( TMessageType )0x00000004,
            ClientWattageResultType = ( TMessageType )0x00000005,
            AdvanceTimeStepType = ( TMessageType )0x00000006,
            ClientGetVoltageType = ( TMessageType )0x00000007,
            ClientVoltageResultType = ( TMessageType )0x00000008
        };

        typedef unsigned int TClientCheckResult;
        enum ClientCheckResultValues
        {
            ClientExists = ( TClientCheckResult )0x00000001,
            ClientDoesNotExist = ( TClientCheckResult )0x00000002
        };

    public:
        typedef std::string TClientName;
        typedef Asynchronous::ClientData::TDataPoint TWattage;
        typedef unsigned int TVoltage;

    private:
        ThreadedTCPServer m_server;
        ThreadedTCPConnectedClient* m_client;

        Semaphore m_clientPresenceSemaphore;
        Semaphore m_clientWattageSemaphore;
        Semaphore m_clientVoltageSemaphore;

        bool m_clientPresentInformation;
        TWattage m_clientWattageInformation;
        TVoltage m_clientVoltageInformation;

    private:
        MatlabManager( void );

    public:
        void
        SetClient( ThreadedTCPConnectedClient* client )
        {
            this->m_client = client;
            this->m_client->SetNotificationCallback( &MatlabReceiveHandler );
        }

        bool
        IsClientPresent( const TClientName & clientName );

        void
        SetWattage( const TClientName & clientName, const TWattage wattage );

        TWattage
        GetWattage( const TClientName & clientName );

        TVoltage
        GetVoltage( const TClientName & clientName );

        void
        AdvanceTimeStep( void );

        void
        ProcessData( void* buffer, size_t size );
};

#endif /* MATLABMANAGER_H_ */
