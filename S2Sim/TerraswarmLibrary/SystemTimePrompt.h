/**
 * @file SystemTimePrompt.h
 * Defines the SystemTimePrompt class.
 *  @date Apr 29, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef SYSTEMTIMEPROMPT_H_
#define SYSTEMTIMEPROMPT_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    /**
     *  System Time Prompt message sent from the client to get the current system time.
     */
    class SystemTimePrompt
    {
    private:
        /**
         *  Message header values.
         */
        enum HeaderValues
        {
            MessageType = 0x0001,
            MessageId = 0x0006
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
            Success = ( TCheckResult )true, /**< Message is of correct type and id **/
            Fail = ( TCheckResult )false /**< Message has incorrect type or id **/
        };
       
    private:
        /**
         *  No use. Private constructor to force usage of the static creation method.
         */
        SystemTimePrompt( void );
        
    public:
        /**
         *  Deallocates the memory for the message.
         */
        ~SystemTimePrompt( void );
        
        /**
         *  Creates a new SystemTimePrompt message and allocates memory for it. @warning Deallocation is the responsibility of the user.
         *
         *  @param senderId      Id of the sender.
         *  @param receiverId    Id of the receiver.
         *
         *  @return Returns a new allocated message.
         */
        static SystemTimePrompt*
        GetNewSystemTimePrompt( const MessageHeader::TSenderId senderId,
                                const MessageHeader::TReceiverId receiverId );
        
        /**
         *  Checks whether the current memory contains a SystemTimePrompt message.
         *
         *  @return Result of the check.
         */
        TCheckResult
        CheckMessage( void ) const;
        
        /**
         *  Returns the size of the message.
         *
         *  @return Size of the current message.
         */
        TDataSize
        GetSize() const;
    };
    
    /**
     *  System Time Response message sent from the controller in response to the time prompt.
     */
    class SystemTimeResponse
    {
    private:
        /**
         *  Message header values.
         */
        enum HeaderValues
        {
            MessageType = 0x0001,
            MessageId = 0x0007
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
            Success = ( TCheckResult )true, /**< Message is of correct type and id **/
            Fail = ( TCheckResult )false /**< Message has incorrect type or id **/
        };
        
        /**
         *  Defines the type of system time, in epoch format.
         */
        typedef unsigned int TSystemTime;
        
    private:
        /**
         *  Size values for the data fields.
         */
        enum FieldSizeValues
        {
            SystemTimeSize = sizeof( TSystemTime )
        };
        
        /**
         *  Index values for the fata fields.
         */
        enum FieldIndexValues
        {
            SystemTimeIndex = MessageHeader::MessageHeaderSize
        };
        
        /**
         *  Accessor helper for the System Time field.
         */
        typedef NetworkByteAccessor<SystemTimeIndex, SystemTimeSize> TSystemTimeAccessor;
        
    private:
        /**
         *  No use. Private constructor to force usage of the static creation method.
         */
        SystemTimeResponse( void );
        
    public:
        /**
         *  Deallocates the memory for the message.
         */
        ~SystemTimeResponse( void );
        
        /**
         *  Creates a new SystemTimeResponse message and allocates memory for it. @warning Deallocation is the responsibility of the user.
         *
         *  @param senderId      Id of the sender.
         *  @param receiverId    Id of the receiver.
         *  @param systemTime    System time in epoch format.
         *
         *  @return Returns a new allocated message.
         */
        static SystemTimeResponse*
        GetNewSystemTimeResponse( const MessageHeader::TSenderId senderId,
                                  const MessageHeader::TReceiverId receiverId,
                                  const TSystemTime systemTime );
        
        /**
         *  Checks whether the current memory contains a SystemTimeResponse message.
         *
         *  @return Result of the check.
         */
        TCheckResult
        CheckMessage( void ) const;
        
        /**
         *  Reads the System Time field in the message.
         *
         *  @return Value of System Time in the message.
         */
        TSystemTime
        GetSystemTime( void ) const;
        
        /**
         *  Returns the size of the message.
         *
         *  @return Size of the current message.
         */
        TDataSize
        GetSize( void ) const;
    };
} /* namespace TerraSwarm */


#endif /* SYSTEMTIMEPROMPT_H_ */
