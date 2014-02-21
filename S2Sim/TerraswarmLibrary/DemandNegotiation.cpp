/**
 * @file DemandNegotiation.cpp
 * Implements the DemandNegotiation class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "DemandNegotiation.h"

namespace TerraSwarm
{
    namespace Synchronous
    {
        DemandNegotiation::DemandNegotiation( void )
        {
        }

        DemandNegotiation::~DemandNegotiation( void )
        {
            delete[] ( ( char* )this );
        }

        DemandNegotiation*
        DemandNegotiation::GetNewDemandNegotiation( const MessageHeader::TSenderId senderId,
                                                    const MessageHeader::TReceiverId receiverId,
                                                    const TNumberOfDataPoints numberOfDataPoints,
                                                    TDataPoint* dataPoints )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
                                      NumberOfDataPointsSize +
                                      DataPointSize * numberOfDataPoints +
                                      MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TNumberOfDataPointsAccessor* )newMemory )->Write( numberOfDataPoints );
            memcpy( ( newMemory + DataStartIndex ), dataPoints, DataPointSize * numberOfDataPoints );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( DemandNegotiation* )newMemory );
        }

        DemandNegotiation::TCheckResult
        DemandNegotiation::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        DemandNegotiation::TNumberOfDataPoints
        DemandNegotiation::GetNumberOfDataPoints( void ) const
        {
            TNumberOfDataPoints value;
            ( ( TNumberOfDataPointsAccessor* )this )->Read( value );
            return ( value );
        }

        DemandNegotiation::TDataPoint*
        DemandNegotiation::GetDataPoints( void ) const
        {
            TNumberOfDataPoints numberOfDataPoints = this->GetNumberOfDataPoints();
            TDataPoint* dataPoints = new TDataPoint[numberOfDataPoints];
            memcpy( dataPoints, ( ( char* )this ) + DataStartIndex, numberOfDataPoints * DataPointSize );
            return ( dataPoints );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
