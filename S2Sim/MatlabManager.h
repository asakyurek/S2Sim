/**
 * @file MatlabManager.h
 * Defines the MatlabManager class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef MATLABMANAGER_H_
#define MATLABMANAGER_H_

#include <iostream>
#include <mutex>

#include "ThreadedTCPServer.h"
#include "ThreadedTCPConnectedClient.h"
#include "SystemManager.h"

using namespace TerraSwarm;

class SystemManager;
class MatlabManager;

/**
 *  This function returns the only instance of MatlabManager.
 *
 *  @return The only instance of MatlabManager.
 */
MatlabManager &
GetMatlabManager( void );

/**
 *  Wrapper callback function that forwards the received data to MatlabManager.
 *
 *  @param client Connection information of OpenDSS-Matlab controller.
 *  @param buffer Buffer containing the received data.
 *  @param size   Size of the received data.
 */
void
MatlabReceiveHandler( ThreadedTCPConnectedClient* client,
                      ThreadedTCPConnectedClient::TBuffer buffer,
                      ThreadedTCPConnectedClient::TNumberOfBytes size );

/**
 *  @brief Manages the connection to the OpenDSS-Matlab controller.
 
    This class manages the communication to the MATLAB controller, that controls the OpenDSS related information processing through the DLL interface.
 */
class MatlabManager
{
    /**
     *  Friend method to implement the singleton for MatlabManager.
     *
     *  @return Returns the only instance of MatlabManager.
     */
    friend MatlabManager &
    GetMatlabManager( void );

    private:
    /**
     *  This type defines the message type used for message processing.
     */
        typedef unsigned int TMessageType;
    
    /**
     *  Defines the values that can be used for TMessageType.
     */
        enum MessageTypeValues
        {
            ClientCheckRequestType = ( TMessageType )0x00000001, /**< Sent to the OpenDSS controller to check whether a client exists. */
            ClientCheckResultType = ( TMessageType )0x00000002, /**< Response of the OpenDSS controller to the client existence check. */
            ClientSetWattageType = ( TMessageType )0x00000003, /**< Sent to the OpenDSS controller to set the consumption of an object. */
            ClientGetWattageType = ( TMessageType )0x00000004, /**< Sent to the OpenDSS controller to get the consumption of an object. */
            ClientWattageResultType = ( TMessageType )0x00000005, /**< Response of the OpenDSS controller to the client consumption get request. */
            AdvanceTimeStepType = ( TMessageType )0x00000006, /**< Sent to the OpenDSS controller to indicate the end of a time step. */
            ClientGetVoltageType = ( TMessageType )0x00000007, /**< Sent to the OpenDSS controller to get the terminal voltage of an object. */
            ClientVoltageResultType = ( TMessageType )0x00000008, /**< Response of the OpenDSS controller to the voltage get request. */
            ClientGetVoltageDeviationType = ( TMessageType )0x00000009, /**< Sent to the OpenDSS controller to get the terminal voltage deviation of an object. */
            ClientVoltageDeviationResultType = ( TMessageType )0x0000000A, /**< Response of the OpenDSS controller to the voltage deviation get request. */
            ClientGetVoltageDeviationAndConsumptionType = ( TMessageType )0x0000000B, /**< Sent to OpenDSS controller to get the deviation and consumption of an object. */
            ClientVoltageDeviationAndConsumptionResultType = ( TMessageType )0x0000000C, /**< Response of the OpenDSS controller to the deviation and consumption of an object. */
        };

    /**
     *  Defines the type that is received from the client existence check.
     */
        typedef unsigned int TClientCheckResult;
    
    /**
     *  Defines the values for TClientCheckResult.
     */
        enum ClientCheckResultValues
        {
            ClientExists = ( TClientCheckResult )0x00000001, /**< Indicates that the object exists. */
            ClientDoesNotExist = ( TClientCheckResult )0x00000002 /**< Indicates that the object does not exist. */
        };

    public:
    /**
     *  Defines the type for an object name.
     */
        typedef std::string TClientName;
    
    /**
     *  Defines the consumption information type.
     */
        typedef Asynchronous::ClientData::TDataPoint TWattage;
    
    /**
     *  Defines the voltage information type.
     */
        typedef Asynchronous::ClientData::TDataPoint TVoltage;

    private:
    /**
     *  Implements the TCP server managing the connection to OpenDSS controller.
     */
        ThreadedTCPServer m_server;
    
    /**
     *  Pointer to the connection management with the OpenDSS controller.
     */
        ThreadedTCPConnectedClient* m_client;

    /**
     *  Mutex used to signal that the presence check result is received.
     */
        std::mutex m_clientPresenceMutex;
    
    /**
     *  Mutex used to signal that the wattage get result is received.
     */
        std::mutex m_clientWattageMutex;
    
    /**
     *  Mutex used to signal that the voltage get result is received.
     */
        std::mutex m_clientVoltageMutex;
    
    /**
     *  Mutex used to signal that the voltage deviation get result is received.
     */
        std::mutex m_clientVoltageDeviationMutex;
    
    /**
     *  Mutex used to signal that the voltage deviation and consumption get result is received.
     */
        std::mutex m_clientVoltageDeviationAndConsumptionMutex;
    
    /**
     *  Mutex used to protect the TCP thread instance.
     */
        std::mutex m_clientThreadMutex;

    /**
     *  Temporary storage for the client presence information.
     */
        bool m_clientPresentInformation;
    
    /**
     *  Temporary storage for the client wattage information.
     */
        TWattage m_clientWattageInformation;
    
    /**
     *  Temporary storage for the client voltage information.
     */
        TVoltage m_clientVoltageInformation;
    
    /**
     *  Temporary storage for the client voltage deviation information.
     */
        TVoltage m_clientVoltageDeviationInformation;

    private:
    /**
     *  Private constructor to implement the singleton.
     */
        MatlabManager( void );
    
    /**
     *  Deletes the TCP thread safely.
     */
        void
        DeleteClientThread( void );

    public:
    /**
     *  Sets the OpenDSS controller connection information.
     *
     *  @param client TCP connection handler.
     */
        void
        SetClient( ThreadedTCPConnectedClient* client )
        {
            this->m_client = client;
            if ( this->m_client != nullptr )
            {
                this->m_client->SetNotificationCallback( &MatlabReceiveHandler );
                this->m_clientThreadMutex.unlock();
            }
        }

    /**
     *  Checks for the presence of a client by communicating with OpenDSS controller. It may block the function call.
     *
     *  @param clientName Name of the object to be checked.
     *
     *  @return Indicates the presence of the client.
     */
        bool
        IsClientPresent( const TClientName & clientName );

    /**
     *  Sets the consumption of a client by communicating with OpenDSS controller.
     *
     *  @param clientName Name of the object.
     *  @param wattage    Consumption of the selected object.
     */
        void
        SetWattage( const TClientName & clientName, const TWattage wattage );

    /**
     *  Gets the consumption of an object by communicating with OpenDSS controller. It may block the function call.
     *
     *  @param clientName Name of the object.
     *
     *  @return Consumption of the selected object.
     */
        TWattage
        GetWattage( const TClientName & clientName );

    /**
     *  Gets the terminal voltage of an object by communicating with OpenDSS controller. It may block the function call.
     *
     *  @param clientName Name of the object.
     *
     *  @return Terminal voltage of the client.
     */
        TVoltage
        GetVoltage( const TClientName & clientName );
    
    /**
     *  Gets the terminal voltage deviation of an object by communicating with OpenDSS controller. It may block the function call.
     *
     *  @param clientName Name of the object.
     *
     *  @return Terminal voltage deviation of the client.
     */
        TVoltage
        GetVoltageDeviation( const TClientName & clientName );
    
    /**
     *  Gets the terminal voltage deviation and consumption of an object by communicating with OpenDSS controller. It may block the function call.
     *
     *  @param clientName Name of the object.
     *
     *  @return Terminal voltage deviation and consumption of the client.
     */
        std::pair<TVoltage, TWattage>
        GetVoltageDeviationAndConsumption( const TClientName & clientName );

    /**
     *  Sends a signal to OpenDSS controller to indicate the end of a time step.
     */
        void
        AdvanceTimeStep( void );

    /**
     *  @brief Processes the received message from OpenDSS controller.
     
        This function processes all received messages from the OpenDSS controller. Note that, it is not guaranteed that the received buffer contains only a single message. This function uses a while loop to process all the messages to avoid data loss.
     *
     *  @param buffer Buffer containing the received data.
     *  @param size   Size of the received data.
     */
        void
        ProcessData( void* buffer, size_t size );
};

#endif /* MATLABMANAGER_H_ */
