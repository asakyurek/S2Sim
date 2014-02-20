/*
 * ClientConnectionRequest.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef CLIENTCONNECTIONREQUEST_H_
#define CLIENTCONNECTIONREQUEST_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

#include <string>

namespace TerraSwarm
{
    namespace Asynchronous
    {
        class ClientConnectionRequest
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0001
                };

            public:
                typedef std::string TClientName;
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

            private:
                ClientConnectionRequest( void );

            public:
                ~ClientConnectionRequest( void );

                static ClientConnectionRequest*
                GetNewClientConnectionRequest( const MessageHeader::TSenderId senderId,
                                               const MessageHeader::TReceiverId receiverId,
                                               const TClientName & clientName );

                TCheckResult
                CheckMessage( void ) const;

                TClientName
                GetClientName( void ) const;
        };
    } /* namespace Asynchronous */

    namespace Synchronous
    {
        class ClientConnectionRequest
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0004
                };

            public:
                typedef std::string TClientName;
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

            private:
                ClientConnectionRequest( void );

            public:
                ~ClientConnectionRequest( void );

                static ClientConnectionRequest*
                GetNewClientConnectionRequest( const MessageHeader::TSenderId senderId,
                                               const MessageHeader::TReceiverId receiverId,
                                               const TClientName & clientName );

                TCheckResult
                CheckMessage( void ) const;

                TClientName
                GetClientName( void ) const;
        };
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* CLIENTCONNECTIONREQUEST_H_ */
