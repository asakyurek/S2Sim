/*
 * MessageEnder.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef MESSAGEENDER_H_
#define MESSAGEENDER_H_

#include "MessageHeader.h"
#include "NetworkByteAccessor.h"

namespace TerraSwarm
{

    class MessageEnder
    {
        private:
            typedef unsigned int TEndOfMessage;
            enum EndOfMessageValues
            {
                EndOfMessageValue = ( TEndOfMessage )0xFEDCBA98
            };

        public:
            enum SizeValues
            {
                EndOfMessageSize = sizeof( TEndOfMessage )
            };

            typedef bool TCheckResult;
            enum CheckResultValues
            {
                CheckSuccess = ( TCheckResult )true,
                CheckFail = ( TCheckResult )false
            };

        private:
            typedef NetworkByteAccessor<0,EndOfMessageSize> TEndOfMessageAccessor;

        public:
            void
            SetEndOfMessageField( void );

            TCheckResult
            CheckEndOfMessageField( void ) const;
    };

} /* namespace TerraSwarm */
#endif /* MESSAGEENDER_H_ */
