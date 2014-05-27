/**
 * @file SetCurrentPrice.cpp
 * Implements the SetCurrentPrice class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "SetCurrentPrice.h"

namespace TerraSwarm
{
    namespace Synchronous
    {
        SetCurrentPrice::SetCurrentPrice( void )
        {
        }

        SetCurrentPrice::~SetCurrentPrice( void )
        {
            //delete[] ( ( char* )this );
        }

        SetCurrentPrice*
        SetCurrentPrice::GetNewSetCurrentPrice( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TInterval intervalBegin,
                                                const TNumberOfPricePoints numberOfPricePoints,
                                                TPrice* pricePoints )
        {
            TDataSize dataSize = IntervalBeginSize + NumberOfPricePointsSize + numberOfPricePoints * PriceSize;
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
            ( ( TIntervalBeginAccessor* )newMemory )->Write( intervalBegin );
            ( ( TNumberOfPricePointsAccessor* )newMemory )->Write( numberOfPricePoints );
            memcpy( ( newMemory + PricePointsIndex ), pricePoints, PriceSize * numberOfPricePoints );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( SetCurrentPrice* )newMemory );
        }

        SetCurrentPrice::TCheckResult
        SetCurrentPrice::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        SetCurrentPrice::TPrice*
        SetCurrentPrice::GetPrice( void ) const
        {
            TNumberOfPricePoints numberOfPricePoints = this->GetNumberOfPricePoints();
            TPrice* values = new TPrice[numberOfPricePoints];
            memcpy( values, ( this + PricePointsIndex ), PriceSize * numberOfPricePoints );
            return ( values );
        }

        SetCurrentPrice::TInterval
        SetCurrentPrice::GetIntervalBegin( void ) const
        {
            TInterval value;
            ( ( TIntervalBeginAccessor* )this )->Read( value );
            return ( value );
        }

        SetCurrentPrice::TNumberOfPricePoints
        SetCurrentPrice::GetNumberOfPricePoints( void ) const
        {
            TNumberOfPricePoints value;
            ( ( TNumberOfPricePointsAccessor* )this )->Read( value );
            return ( value );
        }
        
        TDataSize
        SetCurrentPrice::GetSize( void ) const
        {
            return ( MessageHeader::MessageHeaderSize + IntervalBeginSize + NumberOfPricePointsSize + this->GetNumberOfPricePoints() * PriceSize + MessageEnder::EndOfMessageSize );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
