/**
 * @file ClientData.cpp
 * Implements the Async and Synchronous ClientData classes and messages.
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#include "ClientData.h"

namespace TerraSwarm
{
    namespace Asynchronous
    {

        ClientData::ClientData( void )
        {
        }

        ClientData::~ClientData( void )
        {
            delete[] ( ( char* )this );
        }

        ClientData*
        ClientData::GetNewClientData( const MessageHeader::TSenderId senderId,
                                      const MessageHeader::TReceiverId receiverId,
                                      const TStartTime startTime,
                                      const TTimeResolution timeResolution,
                                      const TNumberOfDataPoints numberOfDataPoints,
                                      TDataPoint* dataPoints )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
                                      StartTimeSize +
                                      TimeResolutionSize +
                                      NumberOfDataPointsSize +
                                      DataPointSize * numberOfDataPoints +
                                      MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TStartTimeAccessor* )newMemory )->Write( startTime );
            ( ( TTimeResolutionAccessor* )newMemory )->Write( timeResolution );
            ( ( TNumberOfDataPointsAccessor* )newMemory )->Write( numberOfDataPoints );
            memcpy( ( newMemory + DataStartIndex ), dataPoints, DataPointSize * numberOfDataPoints );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( ClientData* )newMemory );
        }

        ClientData::TCheckResult
        ClientData::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientData::TStartTime
        ClientData::GetStartTime( void ) const
        {
            TStartTime value;
            ( ( TStartTimeAccessor* )this )->Read( value );
            return ( value );
        }

        ClientData::TTimeResolution
        ClientData::GetTimeResolution( void ) const
        {
            TTimeResolution value;
            ( ( TTimeResolutionAccessor* )this )->Read( value );
            return ( value );
        }

        ClientData::TNumberOfDataPoints
        ClientData::GetNumberOfDataPoints( void ) const
        {
            TNumberOfDataPoints value;
            ( ( TNumberOfDataPointsAccessor* )this )->Read( value );
            return ( value );
        }

        ClientData::TDataPoint*
        ClientData::GetDataPoints( void ) const
        {
            TNumberOfDataPoints numberOfDataPoints = this->GetNumberOfDataPoints();
            TDataPoint* dataPoints = new TDataPoint[numberOfDataPoints];
            memcpy( dataPoints, ( ( char* )this ) + DataStartIndex, numberOfDataPoints * DataPointSize );
            return ( dataPoints );
        }

    } /* namespace Asynchronous */

    namespace Synchronous
    {

        ClientData::ClientData( void )
        {
        }

        ClientData::~ClientData( void )
        {
            delete[] ( ( char* )this );
        }

        ClientData*
        ClientData::GetNewClientData( const MessageHeader::TSenderId senderId,
                                      const MessageHeader::TReceiverId receiverId,
                                      TDataPoint dataPoint )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize +
                                      DataPointSize +
                                      MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, totalDataSize );
            ( ( TDataPointAccessor* )newMemory )->Write( dataPoint );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( ClientData* )newMemory );
        }

        ClientData::TCheckResult
        ClientData::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientData::TDataPoint
        ClientData::GetDataPoint( void ) const
        {
            TDataPoint value;
            ( ( TDataPointAccessor* )this )->Read( value );
            return ( value );
        }

    } /* namespace Synchronous */

} /* namespace TerraSwarm */
