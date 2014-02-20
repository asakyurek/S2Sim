/*
 * ClientData.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef CLIENTDATA_H_
#define CLIENTDATA_H_

#include "MessageHeader.h"
#include "MessageEnder.h"
#include <string.h>

namespace TerraSwarm
{
    namespace Asynchronous
    {

        class ClientData
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0003
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

                typedef unsigned int TStartTime;
                typedef unsigned int TTimeResolution;
                typedef unsigned int TNumberOfDataPoints;
                typedef unsigned int TDataPoint;

            private:
                enum FieldSizeValues
                {
                    StartTimeSize = sizeof( TStartTime ),
                    TimeResolutionSize = sizeof( TTimeResolution ),
                    NumberOfDataPointsSize = sizeof( TNumberOfDataPoints ),
                    DataPointSize = sizeof( TDataPoint )
                };

                enum FieldIndexValues
                {
                    StartTimeIndex = MessageHeader::MessageHeaderSize,
                    TimeResolutionIndex = StartTimeIndex + StartTimeSize,
                    NumberOfDataPointsIndex = TimeResolutionIndex + TimeResolutionSize,
                    DataStartIndex = NumberOfDataPointsIndex + NumberOfDataPointsSize
                };

                typedef NetworkByteAccessor<StartTimeIndex, StartTimeSize> TStartTimeAccessor;
                typedef NetworkByteAccessor<TimeResolutionIndex, TimeResolutionSize> TTimeResolutionAccessor;
                typedef NetworkByteAccessor<NumberOfDataPointsIndex, NumberOfDataPointsSize> TNumberOfDataPointsAccessor;

            private:
                ClientData( void );

            public:
                ~ClientData( void );

                static ClientData*
                GetNewClientData( const MessageHeader::TSenderId senderId,
                                  const MessageHeader::TReceiverId receiverId,
                                  const TStartTime startTime,
                                  const TTimeResolution timeResolution,
                                  const TNumberOfDataPoints numberOfDataPoints,
                                  TDataPoint* dataPoints );

                TCheckResult
                CheckMessage( void ) const;

                TStartTime
                GetStartTime( void ) const;

                TTimeResolution
                GetTimeResolution( void ) const;

                TNumberOfDataPoints
                GetNumberOfDataPoints( void ) const;

                TDataPoint*
                GetDataPoints( void ) const;
        };

    } /* namespace Asynchronous */

    namespace Synchronous
    {

        class ClientData
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0005
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

                typedef unsigned int TDataPoint;

            private:
                enum FieldSizeValues
                {
                    DataPointSize = sizeof( TDataPoint )
                };

                enum FieldIndexValues
                {
                    DataStartIndex = MessageHeader::MessageHeaderSize
                };

                typedef NetworkByteAccessor<DataStartIndex, DataPointSize> TDataPointAccessor;

            private:
                ClientData( void );

            public:
                ~ClientData( void );

                static ClientData*
                GetNewClientData( const MessageHeader::TSenderId senderId,
                                  const MessageHeader::TReceiverId receiverId,
                                  TDataPoint dataPoint );

                TCheckResult
                CheckMessage( void ) const;

                TDataPoint
                GetDataPoint( void ) const;
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* CLIENTDATA_H_ */
