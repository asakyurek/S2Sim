/*
 * GetPrice.h
 *
 *  Created on: Oct 31, 2013
 *      Author: Alper
 */

#ifndef GETPRICE_H_
#define GETPRICE_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Synchronous
    {

        class GetPrice
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0001
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

            private:
                enum FieldSizeValues
                {
                    TotalSize = ( 0 )
                };

            private:
                GetPrice( void );

            public:
                ~GetPrice( void );

                static GetPrice*
                GetNewGetPrice( const MessageHeader::TSenderId senderId,
                                const MessageHeader::TReceiverId receiverId );

                TCheckResult
                CheckMessage( void ) const;

                static TDataSize
                GetSize( void );
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* GETPRICE_H_ */
