/**
 * @file GetPrice.h
 * Defines the GetPrice class and message.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef GETPRICE_H_
#define GETPRICE_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Synchronous
    {

        /**
         *  Defines the GetPrice message sent from the client to the Controller to get the current price value.
         */
        class GetPrice
        {
            private:
            /**
             *  Message Header values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0001
                };

            public:
            /**
             *  Defines the message check result type.
             */
                typedef bool TCheckResult;
            
            /**
             *  Defines the values for TCheckResult.
             */
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true, /**< Message is of correct type and id. **/
                    Fail = ( TCheckResult )false /**< Message has incorrect type or id. **/
                };
            
            private:
            /**
             *  Defines the Field Size values.
             */
                enum FieldSizeValues
                {
                    TotalSize = ( 0 ) /**< No data field. */
                };

            private:
            /**
             *  Private constructor to force the usage of the static creation method.
             */
                GetPrice( void );

            public:
            /**
             *  Deallocates the memory.
             */
                ~GetPrice( void );

            /**
             *  Creates a new GetPrice message and allocates the necessary memory. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId   Id of the sender.
             *  @param receiverId Id of the receiver.
             *
             *  @return Pointer to the newly allocated message.
             */
                static GetPrice*
                GetNewGetPrice( const MessageHeader::TSenderId senderId,
                                const MessageHeader::TReceiverId receiverId );

            /**
             *  Checks whether the current memory contains the GetPrice message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Returns the size of a GetPrice message.
             *
             *  @return Size of the GetPrice message.
             */
                static TDataSize
                GetSize( void );
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* GETPRICE_H_ */
