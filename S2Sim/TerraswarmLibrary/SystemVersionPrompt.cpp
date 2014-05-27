/**
 * @file SystemVersionPrompt.cpp
 * Implements the SystemVersionPrompt class.
 *  @date Apr 29, 2014
 *  @author: Alper Sinan Akyurek
 */

#include "SystemVersionPrompt.h"

namespace TerraSwarm
{
    SystemVersionPrompt::SystemVersionPrompt( void )
    {
    }
    
    SystemVersionPrompt::~SystemVersionPrompt( void )
    {
    }
    
    SystemVersionPrompt*
    SystemVersionPrompt::GetNewSystemVersionPrompt( const MessageHeader::TSenderId senderId,
                                                    const MessageHeader::TReceiverId receiverId )
    {
        TDataSize dataSize = 0;
        TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
        char* newMemory = new char[totalDataSize];
        ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
        ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
        return ( ( SystemVersionPrompt* )newMemory );
    }
    
    SystemVersionPrompt::TCheckResult
    SystemVersionPrompt::CheckMessage( void ) const
    {
        if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
            ( ( MessageHeader* )this )->GetMessageId() == MessageId )
        {
            return ( Success );
        }
        return ( Fail );
    }
    
    TDataSize
    SystemVersionPrompt::GetSize( void ) const
    {
        return ( MessageHeader::MessageHeaderSize + MessageEnder::EndOfMessageSize );
    }
    
    SystemVersionResponse::SystemVersionResponse( void )
    {
    }
    
    SystemVersionResponse::~SystemVersionResponse( void )
    {
    }
    
    SystemVersionResponse*
    SystemVersionResponse::GetNewSystemVersionResponse( const MessageHeader::TSenderId senderId,
                                                        const MessageHeader::TReceiverId receiverId,
                                                        const TMajorVersion majorVersion,
                                                        const TMinorVersion minorVersion )
    {
        TDataSize dataSize = MajorVersionSize + MinorVersionSize;
        TDataSize totalDataSize = MessageHeader::MessageHeaderSize + dataSize + MessageEnder::EndOfMessageSize;
        char* newMemory = new char[totalDataSize];
        ( ( MessageHeader* )newMemory )->PrepareOutgoingMessage( senderId, receiverId, MessageType, MessageId, dataSize );
        ( ( TMajorVersionAccessor* )newMemory )->Write( majorVersion );
        ( ( TMinorVersionAccessor* )newMemory )->Write( minorVersion );
        ( ( MessageEnder* )newMemory )->SetEndOfMessageField();
        return ( ( SystemVersionResponse* )newMemory );
    }
    
    SystemVersionResponse::TCheckResult
    SystemVersionResponse::CheckMessage( void ) const
    {
        if ( ( ( MessageHeader* )this )->GetMessageType() == MessageType &&
            ( ( MessageHeader* )this )->GetMessageId() == MessageId )
        {
            return ( Success );
        }
        return ( Fail );
    }
    
    SystemVersionResponse::TMajorVersion
    SystemVersionResponse::GetMajorVersion( void ) const
    {
        TMajorVersion value;
        ( ( TMajorVersionAccessor* )this )->Read( value );
        return ( value );
    }
    
    SystemVersionResponse::TMinorVersion
    SystemVersionResponse::GetMinorVersion( void ) const
    {
        TMinorVersion value;
        ( ( TMinorVersionAccessor* )this )->Read( value );
        return ( value );
    }
    
    TDataSize
    SystemVersionResponse::GetSize( void ) const
    {
        return ( MessageHeader::MessageHeaderSize + MajorVersionSize + MinorVersionSize + MessageEnder::EndOfMessageSize );
    }
} /* namespace TerraSwarm */