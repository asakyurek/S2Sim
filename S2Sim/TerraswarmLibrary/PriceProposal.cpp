/**
 * @file PriceProposal.cpp
 * Implements the PriceProposal class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "PriceProposal.h"

namespace TerraSwarm
{
    namespace Synchronous
    {
        PriceProposal::PriceProposal( void )
        {
        }

        PriceProposal::~PriceProposal( void )
        {
            //delete[] ( ( char* )this );
        }

        PriceProposal*
        PriceProposal::GetNewPriceProposal( const MessageHeader::TSenderId senderId,
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
            return ( ( PriceProposal* )newMemory );
        }

        PriceProposal::TCheckResult
        PriceProposal::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        PriceProposal::TPrice
        PriceProposal::GetPrice( void ) const
        {
            TPrice value;
            ( ( TPriceAccessor* )this )->Read( value );
            return ( value );
        }

        PriceProposal::TInterval
        PriceProposal::GetIntervalBegin( void ) const
        {
            TInterval value;
            ( ( TIntervalBeginAccessor* )this )->Read( value );
            return ( value );
        }

        PriceProposal::TInterval
        PriceProposal::GetIntervalEnd( void ) const
        {
            TInterval value;
            ( ( TIntervalEndAccessor* )this )->Read( value );
            return ( value );
        }

        TDataSize
        PriceProposal::GetSize( void )
        {
            return ( MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
