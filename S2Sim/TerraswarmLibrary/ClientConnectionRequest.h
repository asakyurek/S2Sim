/**
 * @file ClientConnectionRequest.h
 * Defines the ClientConnectionRequest classes.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef CLIENTCONNECTIONREQUEST_H_
#define CLIENTCONNECTIONREQUEST_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

#include <string>

/**
 *  TerraSwarm related classes are defined under this namespace.
 */
namespace TerraSwarm
{
    /**
     *  Asynchronous client messages are defined under this namespace.
     */
    namespace Asynchronous
    {
        /**
         *  This class implements the asynchronous client connection request message.
         */
        class ClientConnectionRequest
        {
            private:
            /**
             *  Defines the header values for message recognition.
             */
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0001
                };

            public:
            /**
             *  Defines the object name type.
             */
                typedef std::string TClientName;
            
            /**
             *  Defines the message check result type.
             */
                typedef bool TCheckResult;
            
            /**
             *  Defines the values for TCheckResult.
             */
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true, /**< Message is of correct type and id **/
                    Fail = ( TCheckResult )false /**< Message has incorrect type or id **/
                };

            private:
            /**
             *  Not used. Made private to force the usage of the static method for creation.
             */
                ClientConnectionRequest( void );

            public:
            /**
             *  Deletes the current memory.
             */
                ~ClientConnectionRequest( void );

            /**
             *  This function creates a new Client Connection Request message.
             *
             *  @param senderId   Id of the client.
             *  @param receiverId Id of S2Sim.
             *  @param clientName Name of the object, the client is representing.
             *
             *  @return A new allocated message. @warning The deallocation is user's duty.
             */
                static ClientConnectionRequest*
                GetNewClientConnectionRequest( const MessageHeader::TSenderId senderId,
                                               const MessageHeader::TReceiverId receiverId,
                                               const TClientName & clientName );

            /**
             *  Checks whether the current memory address contains a Client Connection Request message. Cast the received memory to the pointer of this class and call this method for checking.
             *
             *  @return Check result of the memory.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Returns the object name.
             *
             *  @return Name of the object within the message.
             */
                TClientName
                GetClientName( void ) const;
        };
    } /* namespace Asynchronous */

    /**
     *  Synchronous client message are defined under this namespace.
     */
    namespace Synchronous
    {
        /**
         *  This class implements the synchronous client connection request message.
         */
        class ClientConnectionRequest
        {
            private:
            /**
             *  Defines the header values for message recognition.
             */
                enum HeaderValues
                {
                    MessageType = 0x0001,
                    MessageId = 0x0004
                };

            public:
            /**
             *  Defines the object name type.
             */
            typedef std::string TClientName;
            
            /**
             *  Defines the message check result type.
             */
            typedef bool TCheckResult;
            
            /**
             *  Defines the values for TCheckResult.
             */
            enum CheckResultValues
            {
                Success = ( TCheckResult )true, /**< Message is of correct type and id **/
                Fail = ( TCheckResult )false /**< Message has incorrect type or id **/
            };

            private:
            /**
             *  Not used. Made private to force the usage of the static method for creation.
             */
                ClientConnectionRequest( void );

            public:
            /**
             *  Deletes the current memory.
             */
                ~ClientConnectionRequest( void );

            /**
             *  This function creates a new Client Connection Request message.
             *
             *  @param senderId   Id of the client.
             *  @param receiverId Id of S2Sim.
             *  @param clientName Name of the object, the client is representing.
             *
             *  @return A new allocated message. @warning The deallocation is user's duty.
             */
                static ClientConnectionRequest*
                GetNewClientConnectionRequest( const MessageHeader::TSenderId senderId,
                                               const MessageHeader::TReceiverId receiverId,
                                               const TClientName & clientName );

            /**
             *  Checks whether the current memory address contains a Client Connection Request message. Cast the received memory to the pointer of this class and call this method for checking.
             *
             *  @return Check result of the memory.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Returns the object name.
             *
             *  @return Name of the object within the message.
             */
                TClientName
                GetClientName( void ) const;
        };
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* CLIENTCONNECTIONREQUEST_H_ */
