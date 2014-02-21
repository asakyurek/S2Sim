/**
 * @file ClientConnectionResponse.cpp
 * Implements the Async and Synchronous ClientConnectionResponse message and classes.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "ClientConnectionResponse.h"

namespace TerraSwarm
{
    namespace Asynchronous
    {

        ClientConnectionResponse::ClientConnectionResponse( void )
        {
        }

        ClientConnectionResponse::~ClientConnectionResponse( void )
        {
            delete[] ( ( char* )this );
        }

        ClientConnectionResponse*
        ClientConnectionResponse::GetNewClientConnectionResponse( const MessageHeader::TSenderId senderId,
                                                                  const MessageHeader::TReceiverId receiverId,
                                                                  const TRequestResult requestResult,
                                                                  const TSystemTime systemTime,
                                                                  const TNumberOfClients numberOfClients,
                                                                  const TSystemMode systemMode )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, TotalSize );
            ( ( TRequestResultAccessor* )newMemory )->Write( requestResult );
            ( ( TSystemTimeAccessor* )newMemory )->Write( systemTime );
            ( ( TNumberOfClientsAccessor* )newMemory )->Write( numberOfClients );
            ( ( TSystemModeAccessor* )newMemory )->Write( systemMode );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( ClientConnectionResponse* )newMemory );
        }

        ClientConnectionResponse::TCheckResult
        ClientConnectionResponse::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientConnectionResponse::TRequestResult
        ClientConnectionResponse::GetRequestResult( void ) const
        {
            TRequestResult value;
            ( ( TRequestResultAccessor* )this )->Read( value );
            return ( value );
        }

        ClientConnectionResponse::TSystemTime
        ClientConnectionResponse::GetSystemTime( void ) const
        {
            TSystemTime value;
            ( ( TSystemTimeAccessor* )this )->Read( value );
            return ( value );
        }

        ClientConnectionResponse::TNumberOfClients
        ClientConnectionResponse::GetNumberOfClients( void ) const
        {
            TNumberOfClients value;
            ( ( TNumberOfClientsAccessor* )this )->Read( value );
            return ( value );
        }

        ClientConnectionResponse::TSystemMode
        ClientConnectionResponse::GetSystemMode( void ) const
        {
            TSystemMode value;
            ( ( TSystemModeAccessor* )this )->Read( value );
            return ( value );
        }

        TDataSize
        ClientConnectionResponse::GetSize( void )
        {
            return ( MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize );
        }

    } /* namespace Asynchronous */

    namespace Synchronous
    {

        ClientConnectionResponse::ClientConnectionResponse( void )
        {
        }

        ClientConnectionResponse::~ClientConnectionResponse( void )
        {
            delete[] ( ( char* )this );
        }

        ClientConnectionResponse*
        ClientConnectionResponse::GetNewClientConnectionResponse( const MessageHeader::TSenderId senderId,
                                                                  const MessageHeader::TReceiverId receiverId,
                                                                  const TRequestResult requestResult,
                                                                  const TSystemTime systemTime,
                                                                  const TNumberOfClients numberOfClients,
                                                                  const TSystemMode systemMode )
        {
            TDataSize totalDataSize = MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize;
            char* newMemory = new char[totalDataSize];
            ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, TotalSize );
            ( ( TRequestResultAccessor* )newMemory )->Write( requestResult );
            ( ( TSystemTimeAccessor* )newMemory )->Write( systemTime );
            ( ( TNumberOfClientsAccessor* )newMemory )->Write( numberOfClients );
            ( ( TSystemModeAccessor* )newMemory )->Write( systemMode );
            ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
            return ( ( ClientConnectionResponse* )newMemory );
        }

        ClientConnectionResponse::TCheckResult
        ClientConnectionResponse::CheckMessage( void ) const
        {
            if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
                 ( ( MessageHeader* )this )->GetMessageId() == MessageId )
            {
                return ( Success );
            }
            return ( Fail );
        }

        ClientConnectionResponse::TRequestResult
        ClientConnectionResponse::GetRequestResult( void ) const
        {
            TRequestResult value;
            ( ( TRequestResultAccessor* )this )->Read( value );
            return ( value );
        }

        ClientConnectionResponse::TSystemTime
        ClientConnectionResponse::GetSystemTime( void ) const
        {
            TSystemTime value;
            ( ( TSystemTimeAccessor* )this )->Read( value );
            return ( value );
        }

        ClientConnectionResponse::TNumberOfClients
        ClientConnectionResponse::GetNumberOfClients( void ) const
        {
            TNumberOfClients value;
            ( ( TNumberOfClientsAccessor* )this )->Read( value );
            return ( value );
        }

        ClientConnectionResponse::TSystemMode
        ClientConnectionResponse::GetSystemMode( void ) const
        {
            TSystemMode value;
            ( ( TSystemModeAccessor* )this )->Read( value );
            return ( value );
        }

        TDataSize
        ClientConnectionResponse::GetSize( void )
        {
            return ( MessageHeader::MessageHeaderSize + TotalSize + MessageEnder::EndOfMessageSize );
        }

    } /* namespace Synchronous */

} /* namespace TerraSwarm */
