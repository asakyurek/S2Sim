/**
 * @file MessageEnder.cpp
 * Implements a MessageEnder class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "MessageEnder.h"

namespace TerraSwarm
{

    void
    MessageEnder::SetEndOfMessageField( void )
    {
        MessageHeader* header = ( ( MessageHeader* )this );
        TByteIndex endOfMessageIndex = MessageHeader::MessageHeaderSize + header->GetDataSize();
        void* endOfMessageAddress = ( void* )( ( ( char* )this ) + endOfMessageIndex );
        ( ( TEndOfMessageAccessor* )endOfMessageAddress )->Write( ( TEndOfMessage )EndOfMessageValue );
    }

    MessageEnder::TCheckResult
    MessageEnder::CheckEndOfMessageField( void ) const
    {
        MessageHeader* header = ( ( MessageHeader* )this );
        TByteIndex endOfMessageIndex = MessageHeader::MessageHeaderSize + header->GetDataSize();
        void* endOfMessageAddress = ( void* )( ( ( char* )this ) + endOfMessageIndex );
        TEndOfMessage value;
        ( ( TEndOfMessageAccessor* )endOfMessageAddress )->Read( value );
        if ( value == EndOfMessageValue )
        {
            return ( CheckSuccess );
        }
        return ( CheckFail );
    }

} /* namespace TerraSwarm */
