/**
 * @file MessageEnder.h
 * Defines a MessageEnder class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef MESSAGEENDER_H_
#define MESSAGEENDER_H_

#include "MessageHeader.h"
#include "NetworkByteAccessor.h"

namespace TerraSwarm
{

    /**
     *  Class to send the end of message field.
     */
    class MessageEnder
    {
        private:
        /**
         *  End of message field type.
         */
            typedef unsigned int TEndOfMessage;
        
        /**
         *  Values for the EOM field.
         */
            enum EndOfMessageValues
            {
                EndOfMessageValue = ( TEndOfMessage )0xFEDCBA98 /**< The only defined EOM value. */
            };

        public:
        /**
         *  Defines the size values of the class.
         */
            enum SizeValues
            {
                EndOfMessageSize = sizeof( TEndOfMessage ) /**< Size of the EOM field. */
            };

        /**
         *  Defines the message check result type.
         */
            typedef bool TCheckResult;
        
        /**
         *  Defines the values for TCheckResult.
         */
            enum CheckResultValues
            {
                CheckSuccess = ( TCheckResult )true, /**< Message has correct EOM. **/
                CheckFail = ( TCheckResult )false /**< Message has incorrect EOM. **/
            };

        private:
        /**
         *  Accessor helper for the EOM field.
         */
            typedef NetworkByteAccessor<0,EndOfMessageSize> TEndOfMessageAccessor;

        public:
        /**
         *  Sets the EOM field to its value.
         */
            void
            SetEndOfMessageField( void );

        /**
         *  Checks the EOM field for the correct value.
         *
         *  @return Result of the check.
         */
            TCheckResult
            CheckEndOfMessageField( void ) const;
    };

} /* namespace TerraSwarm */
#endif /* MESSAGEENDER_H_ */
