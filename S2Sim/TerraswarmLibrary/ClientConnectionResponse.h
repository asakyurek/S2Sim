/**
 * @file ClientConnectionResponse.h
 * Defines the Asnyc and Synchronous ClientConnectionResponse messages and classes.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef CLIENTCONNECTIONRESPONSE_H_
#define CLIENTCONNECTIONRESPONSE_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Asynchronous
    {

        /**
         *  This class implements the Response message of the controller to the clients connection request.
         */
        class ClientConnectionResponse
        {
            private:
            /**
             *  Message Header Values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0002
                };

            public:
            /**
             *  Defines the message check result type.
             */
                typedef bool TCheckResult;
            
            /**
             *  Defines the values for TCheckResult.
             */
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true, /**< Message is of correct type and id. **/
                    Fail = ( TCheckResult )false /**< Message has incorrect type or id. **/
                };

            /**
             *  Defines the result of the request type.
             */
                typedef unsigned int TRequestResult;
            
            /**
             *  Defines values for the TRequestResult type.
             */
                enum RequestResultValues
                {
                    RequestAccepted = ( TRequestResult )0x00000000, /**< The request is accepted. */
                    RequestObjectIdNotFound = ( TRequestResult )0x00000001 /**< The requested object name is not found and rejected. */
                };

            /**
             *  Defines the time of the system in epoch format.
             */
                typedef unsigned int TSystemTime;
            
            /**
             *  Defines the number of clients type.
             */
                typedef unsigned short TNumberOfClients;
            
            /**
             *  Defines the current system working mode.
             */
                typedef unsigned short TSystemMode;
            
            /**
             *  Defines the values for the TSystemMode type.
             */
                enum SystemModeValues
                {
                    SimulationMode = ( TSystemMode )0x0001, /**< Simulation working mode where the system is started artificially. */
                    RealTimeMode = ( TSystemMode )0x0002 /**< Real time working mode where the system is working in real time. Not implemented yet. */
                };

            private:
            /**
             *  Size of the fields within the message.
             */
                enum FieldSizeValues
                {
                    RequestResultSize = sizeof( TRequestResult ),
                    SystemTimeSize = sizeof( TSystemTime ),
                    NumberOfClientsSize = sizeof( TNumberOfClients ),
                    SystemModeSize = sizeof( TSystemMode ),
                    TotalSize = ( RequestResultSize + SystemTimeSize + NumberOfClientsSize + SystemModeSize )
                };

            /**
             *  Index of the fields within the message.
             */
                enum FieldIndexValues
                {
                    RequestResultIndex = MessageHeader::MessageHeaderSize,
                    SystemTimeIndex = RequestResultIndex + RequestResultSize,
                    NumberOfClientsIndex = SystemTimeIndex + SystemTimeSize,
                    SystemModeIndex = NumberOfClientsIndex + NumberOfClientsSize
                };

            /**
             *  Accessor helper for the RequestResult field.
             */
                typedef NetworkByteAccessor<RequestResultIndex, RequestResultSize> TRequestResultAccessor;
            
            /**
             *  Accessor helper for the SystemTime field.
             */
                typedef NetworkByteAccessor<SystemTimeIndex, SystemTimeSize> TSystemTimeAccessor;
            
            /**
             *  Accessor helper for the NumberOfClients field.
             */
                typedef NetworkByteAccessor<NumberOfClientsIndex, NumberOfClientsSize> TNumberOfClientsAccessor;
            
            /**
             *  Accessor helper for the SystemMode field.
             */
                typedef NetworkByteAccessor<SystemModeIndex, SystemModeSize> TSystemModeAccessor;

            private:
            /**
             *  Private constructor to force the usage of the static creation method.
             */
                ClientConnectionResponse( void );

            public:
            /**
             *  Deallocates the memory.
             */
                ~ClientConnectionResponse( void );

            /**
             *  Creates a new ClientConnectionResponse message and allocates memory for it. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId        Id of the sender.
             *  @param receiverId      Id of the receiver.
             *  @param requestResult   Result of the connection request.
             *  @param systemTime      Current system time in epoch format.
             *  @param numberOfClients Number of Clients in the system.
             *  @param systemMode      System working mode.
             *
             *  @return Newly allocated message pointer.
             */
                static ClientConnectionResponse*
                GetNewClientConnectionResponse( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TRequestResult requestResult,
                                                const TSystemTime systemTime,
                                                const TNumberOfClients numberOfClients,
                                                const TSystemMode systemMode );

            /**
             *  Checks whether the current memory is a ClientConnectionResponse message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the request result field.
             *
             *  @return Request result value.
             */
                TRequestResult
                GetRequestResult( void ) const;

            /**
             *  Reads the system time field.
             *
             *  @return System time value.
             */
                TSystemTime
                GetSystemTime( void ) const;

            /**
             *  Reads the number of clients field.
             *
             *  @return Number of clients in the system.
             */
                TNumberOfClients
                GetNumberOfClients( void ) const;

            /**
             *  Reads the system mode field.
             *
             *  @return Current system working mode.
             */
                TSystemMode
                GetSystemMode( void ) const;

            /**
             *  Returns the size of a ClientConnectionResponse message.
             *
             *  @return Size of the ClientConnectionResponse message.
             */
                static TDataSize
                GetSize( void );
        };

    } /* namespace Asynchronous */

    namespace Synchronous
    {

        /**
         *  This class implements the Response message of the controller to the clients connection request.
         */
        class ClientConnectionResponse
        {
            private:
            /**
             *  Message Header values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0005
                };

            public:
            /**
             *  Defines the message check result type.
             */
                typedef bool TCheckResult;
            
            /**
             *  Defines the values for TCheckResult.
             */
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true, /**< Message is of correct type and id. **/
                    Fail = ( TCheckResult )false /**< Message has incorrect type or id. **/
                };
            
            /**
             *  Defines the result of the request type.
             */
                typedef unsigned int TRequestResult;
            
            /**
             *  Defines values for the TRequestResult type.
             */
                enum RequestResultValues
                {
                    RequestAccepted = ( TRequestResult )0x00000000, /**< The request is accepted. */
                    RequestObjectIdNotFound = ( TRequestResult )0x00000001 /**< The requested object name is not found and rejected. */
                };
            
            /**
             *  Defines the time of the system in epoch format.
             */
                typedef unsigned int TSystemTime;
            
            /**
             *  Defines the number of clients type.
             */
                typedef unsigned short TNumberOfClients;
            
            /**
             *  Defines the current system working mode.
             */
                typedef unsigned short TSystemMode;
            
            /**
             *  Defines the simulation time steps in seconds.
             */
                typedef unsigned int TSystemTimeStep;
            
            /**
             *  Defines the values for the TSystemMode type.
             */
                enum SystemModeValues
                {
                    SimulationMode = ( TSystemMode )0x0001, /**< Simulation working mode where the system is started artificially. */
                    RealTimeMode = ( TSystemMode )0x0002 /**< Real time working mode where the system is working in real time. Not implemented yet. */
                };
            
            private:
            /**
             *  Size of the message fields.
             */
                enum FieldSizeValues
                {
                    RequestResultSize = sizeof( TRequestResult ),
                    SystemTimeSize = sizeof( TSystemTime ),
                    NumberOfClientsSize = sizeof( TNumberOfClients ),
                    SystemModeSize = sizeof( TSystemMode ),
                    SystemTimeStepSize = sizeof( TSystemTimeStep ),
                    TotalSize = ( RequestResultSize + SystemTimeSize + NumberOfClientsSize + SystemModeSize + SystemTimeStepSize )
                };

            /**
             *  Index of the message fields.
             */
                enum FieldIndexValues
                {
                    RequestResultIndex = MessageHeader::MessageHeaderSize,
                    SystemTimeIndex = RequestResultIndex + RequestResultSize,
                    NumberOfClientsIndex = SystemTimeIndex + SystemTimeSize,
                    SystemModeIndex = NumberOfClientsIndex + NumberOfClientsSize,
                    SystemTimeStepIndex = SystemModeIndex + SystemModeSize
                };

            /**
             *  Accessor helper for the RequestResult field.
             */
                typedef NetworkByteAccessor<RequestResultIndex, RequestResultSize> TRequestResultAccessor;
            
            /**
             *  Accessor helper for the SystemTime field.
             */
                typedef NetworkByteAccessor<SystemTimeIndex, SystemTimeSize> TSystemTimeAccessor;
            
            /**
             *  Accessor helper for the NumberOfClients field.
             */
                typedef NetworkByteAccessor<NumberOfClientsIndex, NumberOfClientsSize> TNumberOfClientsAccessor;
            
            /**
             *  Accessor helper for the SystemMode field.
             */
                typedef NetworkByteAccessor<SystemModeIndex, SystemModeSize> TSystemModeAccessor;
            
            /**
             *  Accessor helper for the SystemTimeStep field.
             */
                typedef NetworkByteAccessor<SystemTimeStepIndex, SystemTimeStepSize> TSystemTimeStepAccessor;

            private:
            /**
             *  Private constructor to force the usage of the static construction method.
             */
                ClientConnectionResponse( void );

            public:
            /**
             *  Deallocates the memory.
             */
                ~ClientConnectionResponse( void );

            /**
             *  Creates a new ClientConnectionResponse message and allocates memory for it. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId        Id of the sender.
             *  @param receiverId      Id of the receiver.
             *  @param requestResult   Result of the connection request.
             *  @param systemTime      Current system time.
             *  @param numberOfClients Current number of clients.
             *  @param systemMode      Current system working mode.
             *  @param systemTimeStep  Current system time step size.
             *
             *  @return Newly allocated message pointer.
             */
                static ClientConnectionResponse*
                GetNewClientConnectionResponse( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TRequestResult requestResult,
                                                const TSystemTime systemTime,
                                                const TNumberOfClients numberOfClients,
                                                const TSystemMode systemMode,
                                                const TSystemTimeStep systemTimeStep );

            /**
             *  Checks whether the current memory holds a ClientConnectionResponse message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the RequestResult field.
             *
             *  @return Result of the connection request.
             */
                TRequestResult
                GetRequestResult( void ) const;

            /**
             *  Reads the SystemTime field.
             *
             *  @return Current system time.
             */
                TSystemTime
                GetSystemTime( void ) const;

            /**
             *  Reads the NumberOfClients field.
             *
             *  @return Number of clients in the system.
             */
                TNumberOfClients
                GetNumberOfClients( void ) const;

            /**
             *  Reads the SystemMode field.
             *
             *  @return The current system working mode.
             */
                TSystemMode
                GetSystemMode( void ) const;
            
            /**
             *  Reads the SystemTimeStep field.
             *
             * @return The current system time step size.
             */
                TSystemTimeStep
                GetSystemTimeStep( void ) const;

            /**
             *  Returns the size of a ClientConnectionResponse message.
             *
             *  @return Size of the ClientConnectionResponse message.
             */
                static TDataSize
                GetSize( void );
        };

    } /* namespace Synchronous */

} /* namespace TerraSwarm */
#endif /* CLIENTCONNECTIONRESPONSE_H_ */
