/*
 * ClientConnectionResponse.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef CLIENTCONNECTIONRESPONSE_H_
#define CLIENTCONNECTIONRESPONSE_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Asynchronous
    {

        class ClientConnectionResponse
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0002
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

                typedef unsigned int TRequestResult;
                enum RequestResultValues
                {
                    RequestAccepted = ( TRequestResult )0x00000000,
                    RequestObjectIdNotFound = ( TRequestResult )0x00000001
                };

                typedef unsigned int TSystemTime;
                typedef unsigned short TNumberOfClients;
                typedef unsigned short TSystemMode;
                enum SystemModeValues
                {
                    SimulationMode = ( TSystemMode )0x0001,
                    RealTimeMode = ( TSystemMode )0x0002
                };

            private:
                enum FieldSizeValues
                {
                    RequestResultSize = sizeof( TRequestResult ),
                    SystemTimeSize = sizeof( TSystemTime ),
                    NumberOfClientsSize = sizeof( TNumberOfClients ),
                    SystemModeSize = sizeof( TSystemMode ),
                    TotalSize = ( RequestResultSize + SystemTimeSize + NumberOfClientsSize + SystemModeSize )
                };

                enum FieldIndexValues
                {
                    RequestResultIndex = MessageHeader::MessageHeaderSize,
                    SystemTimeIndex = RequestResultIndex + RequestResultSize,
                    NumberOfClientsIndex = SystemTimeIndex + SystemTimeSize,
                    SystemModeIndex = NumberOfClientsIndex + NumberOfClientsSize
                };

                typedef NetworkByteAccessor<RequestResultIndex, RequestResultSize> TRequestResultAccessor;
                typedef NetworkByteAccessor<SystemTimeIndex, SystemTimeSize> TSystemTimeAccessor;
                typedef NetworkByteAccessor<NumberOfClientsIndex, NumberOfClientsSize> TNumberOfClientsAccessor;
                typedef NetworkByteAccessor<SystemModeIndex, SystemModeSize> TSystemModeAccessor;

            private:
                ClientConnectionResponse( void );

            public:
                ~ClientConnectionResponse( void );

                static ClientConnectionResponse*
                GetNewClientConnectionResponse( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TRequestResult requestResult,
                                                const TSystemTime systemTime,
                                                const TNumberOfClients numberOfClients,
                                                const TSystemMode systemMode );

                TCheckResult
                CheckMessage( void ) const;

                TRequestResult
                GetRequestResult( void ) const;

                TSystemTime
                GetSystemTime( void ) const;

                TNumberOfClients
                GetNumberOfClients( void ) const;

                TSystemMode
                GetSystemMode( void ) const;

                static TDataSize
                GetSize( void );
        };

    } /* namespace Asynchronous */

    namespace Synchronous
    {

        class ClientConnectionResponse
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0005
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

                typedef unsigned int TRequestResult;
                enum RequestResultValues
                {
                    RequestAccepted = ( TRequestResult )0x00000000,
                    RequestObjectIdNotFound = ( TRequestResult )0x00000001
                };

                typedef unsigned int TSystemTime;
                typedef unsigned short TNumberOfClients;
                typedef unsigned short TSystemMode;
                enum SystemModeValues
                {
                    SimulationMode = ( TSystemMode )0x0001,
                    RealTimeMode = ( TSystemMode )0x0002
                };

            private:
                enum FieldSizeValues
                {
                    RequestResultSize = sizeof( TRequestResult ),
                    SystemTimeSize = sizeof( TSystemTime ),
                    NumberOfClientsSize = sizeof( TNumberOfClients ),
                    SystemModeSize = sizeof( TSystemMode ),
                    TotalSize = ( RequestResultSize + SystemTimeSize + NumberOfClientsSize + SystemModeSize )
                };

                enum FieldIndexValues
                {
                    RequestResultIndex = MessageHeader::MessageHeaderSize,
                    SystemTimeIndex = RequestResultIndex + RequestResultSize,
                    NumberOfClientsIndex = SystemTimeIndex + SystemTimeSize,
                    SystemModeIndex = NumberOfClientsIndex + NumberOfClientsSize
                };

                typedef NetworkByteAccessor<RequestResultIndex, RequestResultSize> TRequestResultAccessor;
                typedef NetworkByteAccessor<SystemTimeIndex, SystemTimeSize> TSystemTimeAccessor;
                typedef NetworkByteAccessor<NumberOfClientsIndex, NumberOfClientsSize> TNumberOfClientsAccessor;
                typedef NetworkByteAccessor<SystemModeIndex, SystemModeSize> TSystemModeAccessor;

            private:
                ClientConnectionResponse( void );

            public:
                ~ClientConnectionResponse( void );

                static ClientConnectionResponse*
                GetNewClientConnectionResponse( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TRequestResult requestResult,
                                                const TSystemTime systemTime,
                                                const TNumberOfClients numberOfClients,
                                                const TSystemMode systemMode );

                TCheckResult
                CheckMessage( void ) const;

                TRequestResult
                GetRequestResult( void ) const;

                TSystemTime
                GetSystemTime( void ) const;

                TNumberOfClients
                GetNumberOfClients( void ) const;

                TSystemMode
                GetSystemMode( void ) const;

                static TDataSize
                GetSize( void );
        };

    } /* namespace Synchronous */

} /* namespace TerraSwarm */
#endif /* CLIENTCONNECTIONRESPONSE_H_ */
