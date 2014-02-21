/**
 * @file ClientConnectionRequest.cpp
 * Implements the ClientConnectionRequest class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "ClientConnectionRequest.h"

namespace TerraSwarm
{
    namespace Asynchronous
    {

        ClientConnectionRequest::ClientConnectionRequest( void )
        {
        }

        ClientConnectionRequest::~ClientConnectionRequest( void )
        {
            delete[] ( ( char* )this );
        }

        ClientConnectionRequest*
        ClientConnectionRequest::GetNewClientConnectionRequest( const MessageHeader::TSenderId senderId,
                                                                const MessageHeader::TReceiverId receiverId,
                                                                const TClientName & clientName )
        {
            TDataSize nameSize = ( TDataSize )clientName.size();
            TDataSize paddingSize = ( nameSize % 4 == 0 ) ? 0 : 4 - nameSize;
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + nameSize + paddingSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, nameSize + paddingSize );
            memcpy( newMemory + MessageHeader::MessageHeaderSize, clientName.c_str(), nameSize );
            memset( newMemory + MessageHeader::MessageHeaderSize + nameSize, 0x00, paddingSize );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( ClientConnectionRequest* )newMemory );
        }

        ClientConnectionRequest::TCheckResult
        ClientConnectionRequest::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientConnectionRequest::TClientName
        ClientConnectionRequest::GetClientName( void ) const
        {
            TClientName clientName = ( ( char* )this ) + MessageHeader::MessageHeaderSize;
            return ( clientName );
        }

    } /* namespace Asynchronous */

    namespace Synchronous
    {
        ClientConnectionRequest::ClientConnectionRequest( void )
        {
        }

        ClientConnectionRequest::~ClientConnectionRequest( void )
        {
            delete[] ( ( char* )this );
        }

        ClientConnectionRequest*
        ClientConnectionRequest::GetNewClientConnectionRequest( const MessageHeader::TSenderId senderId,
                                                                const MessageHeader::TReceiverId receiverId,
                                                                const TClientName & clientName )
        {
            TDataSize nameSize = ( TDataSize )clientName.size();
            TDataSize paddingSize = ( nameSize % 4 == 0 ) ? 0 : 4 - nameSize;
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + nameSize + paddingSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, nameSize + paddingSize );
            memcpy( newMemory + MessageHeader::MessageHeaderSize, clientName.c_str(), nameSize );
            memset( newMemory + MessageHeader::MessageHeaderSize + nameSize, 0x00, paddingSize );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( ClientConnectionRequest* )newMemory );
        }

        ClientConnectionRequest::TCheckResult
        ClientConnectionRequest::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientConnectionRequest::TClientName
        ClientConnectionRequest::GetClientName( void ) const
        {
            TClientName clientName = ( ( char* )this ) + MessageHeader::MessageHeaderSize;
            return ( clientName );
        }
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
