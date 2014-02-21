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
            delete[] ( ( char* )this );
        }

        SetCurrentPrice*
        SetCurrentPrice::GetNewSetCurrentPrice( const MessageHeader::TSenderId senderId,
                                                const MessageHeader::TReceiverId receiverId,
                                                const TPrice price,
                                                const TInterval intervalBegin,
                                                const TInterval intervalEnd )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, TotalSize );
            ( ( TPriceAccessor* )newMemory )->Write( price );
            ( ( TIntervalBeginAccessor* )newMemory )->Write( intervalBegin );
            ( ( TIntervalEndAccessor* )newMemory )->Write( intervalEnd );
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

        SetCurrentPrice::TPrice
        SetCurrentPrice::GetPrice( void ) const
        {
            TPrice value;
            ( ( TPriceAccessor* )this )->Read( value );
            return ( value );
        }

        SetCurrentPrice::TInterval
        SetCurrentPrice::GetIntervalBegin( void ) const
        {
            TInterval value;
            ( ( TIntervalBeginAccessor* )this )->Read( value );
            return ( value );
        }

        SetCurrentPrice::TInterval
        SetCurrentPrice::GetIntervalEnd( void ) const
        {
            TInterval value;
            ( ( TIntervalEndAccessor* )this )->Read( value );
            return ( value );
        }

        TDataSize
        SetCurrentPrice::GetSize( void )
        {
            return ( MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
