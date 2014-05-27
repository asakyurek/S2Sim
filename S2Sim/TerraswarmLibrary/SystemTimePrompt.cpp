/**
 * @file SystemTimePrompt.cpp
 * Implements the SystemTimePrompt class.
 *  @date Apr 29, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "SystemTimePrompt.h"

namespace TerraSwarm
{
    SystemTimePrompt::SystemTimePrompt( void )
    {
    }
    
    SystemTimePrompt::~SystemTimePrompt( void )
    {
    }
    
    SystemTimePrompt*
    SystemTimePrompt::GetNewSystemTimePrompt( const MessageHeader::TSenderId senderId,
                                              const MessageHeader::TReceiverId receiverId )
    {
        TDataSize dataSize = 0;
        TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
        char* newMemory = new char[totalDataSize];
        ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
        ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
        return ( ( SystemTimePrompt* )newMemory );
    }
    
    SystemTimePrompt::TCheckResult
    SystemTimePrompt::CheckMessage( void ) const
    {
        if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
            ( ( MessageHeader* )this )->GetMessageId() == MessageId )
        {
            return ( Success );
        }
        return ( Fail );
    }

    TDataSize
    SystemTimePrompt::GetSize( void ) const
    {
        return ( MessageHeader::MessageHeaderSize + MessageEnder::EndOfMessageSize );
    }
    
    SystemTimeResponse::SystemTimeResponse( void )
    {
    }
    
    SystemTimeResponse::~SystemTimeResponse( void )
    {
    }
    
    SystemTimeResponse*
    SystemTimeResponse::GetNewSystemTimeResponse( const MessageHeader::TSenderId senderId,
                                                  const MessageHeader::TReceiverId receiverId,
                                                  const TSystemTime systemTime )
    {
        TDataSize dataSize = SystemTimeSize;
        TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
        char* newMemory = new char[totalDataSize];
        ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
        ( ( TSystemTimeAccessor* )newMemory )->Write( systemTime );
        ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
        return ( ( SystemTimeResponse* )newMemory );
    }
    
    SystemTimeResponse::TCheckResult
    SystemTimeResponse::CheckMessage( void ) const
    {
        if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
            ( ( MessageHeader* )this )->GetMessageId() == MessageId )
        {
            return ( Success );
        }
        return ( Fail );
    }
    
    SystemTimeResponse::TSystemTime
    SystemTimeResponse::GetSystemTime( void ) const
    {
        TSystemTime value;
        ( ( TSystemTimeAccessor* )this )->Read( value );
        return ( value );
    }
    
    TDataSize
    SystemTimeResponse::GetSize( void ) const
    {
        return ( MessageHeader::MessageHeaderSize + SystemTimeSize + MessageEnder::EndOfMessageSize );
    }
} /* namespace TerraSwarm */