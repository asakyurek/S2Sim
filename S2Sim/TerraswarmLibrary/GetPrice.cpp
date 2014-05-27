/**
 * @file GetPrice.cpp
 * Implements the GetPrice class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "GetPrice.h"

namespace TerraSwarm
{
    namespace Synchronous
    {
        GetPrice::GetPrice( void )
        {
        }

        GetPrice::~GetPrice( void )
        {
            //delete[] ( ( char* )this );
        }

        GetPrice*
        GetPrice::GetNewGetPrice( const MessageHeader::TSenderId senderId,
                                  const MessageHeader::TReceiverId receiverId )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, TotalSize );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( GetPrice* )newMemory );
        }

        GetPrice::TCheckResult
        GetPrice::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        TDataSize
        GetPrice::GetSize( void )
        {
            return ( MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
