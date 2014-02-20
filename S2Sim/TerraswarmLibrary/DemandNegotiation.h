/*
 * DemandNegotiation.h
 *
 *  Created on: Oct 31, 2013
 *      Author: Alper
 */

#ifndef DEMANDNEGOTIATION_H_
#define DEMANDNEGOTIATION_H_

#include "MessageHeader.h"
#include "MessageEnder.h"
#include <string.h>

namespace TerraSwarm
{
    namespace Synchronous
    {

        class DemandNegotiation
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0004
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

                typedef unsigned int TNumberOfDataPoints;
                typedef unsigned int TDataPoint;

            private:
                enum FieldSizeValues
                {
                    NumberOfDataPointsSize = sizeof( TNumberOfDataPoints ),
                    DataPointSize = sizeof( TDataPoint )
                };

                enum FieldIndexValues
                {
                    NumberOfDataPointsIndex = MessageHeader::MessageHeaderSize,
                    DataStartIndex = NumberOfDataPointsIndex + NumberOfDataPointsSize
                };

                typedef NetworkByteAccessor<NumberOfDataPointsIndex, NumberOfDataPointsSize> TNumberOfDataPointsAccessor;

            private:
                DemandNegotiation( void );

            public:
                ~DemandNegotiation( void );

                static DemandNegotiation*
                GetNewDemandNegotiation( const MessageHeader::TSenderId senderId,
                                         const MessageHeader::TReceiverId receiverId,
                                         const TNumberOfDataPoints numberOfDataPoints,
                                         TDataPoint* dataPoints );

                TCheckResult
                CheckMessage( void ) const;

                TNumberOfDataPoints
                GetNumberOfDataPoints( void ) const;

                TDataPoint*
                GetDataPoints( void ) const;
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* DEMANDNEGOTIATION_H_ */
