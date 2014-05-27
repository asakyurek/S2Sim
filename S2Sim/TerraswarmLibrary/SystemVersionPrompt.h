/**
 * @file SystemVersionPrompt.h
 * Defines the SystemVersionPrompt class.
 *  @date Apr 29, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef SYSTEMVERSIONPROMPT_H_
#define SYSTEMVERSIONPROMPT_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{ 
    /**
     *  System Version Prompt message sent from the client to get the current system version.
     */
    class SystemVersionPrompt
    {
    private:
        /**
         *  Message header values.
         */
        enum HeaderValues
        {
            MessageType = 0x0001,
            MessageId = 0x0008
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
        SystemVersionPrompt( void );
        
    public:
        /**
         *  Deallocates the memory for the message.
         */
        ~SystemVersionPrompt( void );
        
        /**
         *  Creates a new SystemVersionPrompt message and allocates memory for it. @warning Deallocation is the responsibility of the user.
         *
         *  @param senderId      Id of the sender.
         *  @param receiverId    Id of the receiver.
         *
         *  @return Returns a new allocated message.
         */
        static SystemVersionPrompt*
        GetNewSystemVersionPrompt( const MessageHeader::TSenderId senderId,
                                   const MessageHeader::TReceiverId receiverId );
        
        /**
         *  Checks whether the current memory contains a SystemVersionPrompt message.
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
        GetSize( void ) const;
    };
    
    /**
     *  System Version Response message sent from the controller in response to the time prompt.
     */
    class SystemVersionResponse
    {
    private:
        /**
         *  Message header values.
         */
        enum HeaderValues
        {
            MessageType = 0x0001,
            MessageId = 0x0009
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
         *  Defines the Major Revision value.
         */
        typedef unsigned short TMajorVersion;
        
        /**
         *  Defines the Minor Revision value.
         */
        typedef unsigned short TMinorVersion;
        
    private:
        /**
         *  Size values for the data fields.
         */
        enum FieldSizeValues
        {
            MajorVersionSize = sizeof( TMajorVersion ),
            MinorVersionSize = sizeof( TMinorVersion )
        };
        
        /**
         *  Index values for the fata fields.
         */
        enum FieldIndexValues
        {
            MajorVersionIndex = MessageHeader::MessageHeaderSize,
            MinorVersionIndex = MajorVersionIndex + MajorVersionSize
        };
        
        /**
         *  Accessor helper for the Major Revision field.
         */
        typedef NetworkByteAccessor<MajorVersionIndex, MajorVersionSize> TMajorVersionAccessor;
        
        /**
         *  Accessor helper for the Minor Revision field.
         */
        typedef NetworkByteAccessor<MinorVersionIndex, MinorVersionSize> TMinorVersionAccessor;
        
    private:
        /**
         *  No use. Private constructor to force usage of the static creation method.
         */
        SystemVersionResponse( void );
        
    public:
        /**
         *  Deallocates the memory for the message.
         */
        ~SystemVersionResponse( void );
        
        /**
         *  Creates a new SystemVersionResponse message and allocates memory for it. @warning Deallocation is the responsibility of the user.
         *
         *  @param senderId      Id of the sender.
         *  @param receiverId    Id of the receiver.
         *  @param majorRevision Major Revision of the current software.
         *  @param minorRevision Minor Revision of the current software.
         *
         *  @return Returns a new allocated message.
         */
        static SystemVersionResponse*
        GetNewSystemVersionResponse( const MessageHeader::TSenderId senderId,
                                     const MessageHeader::TReceiverId receiverId,
                                     const TMajorVersion majorRevision,
                                     const TMinorVersion minorRevision );
        
        /**
         *  Checks whether the current memory contains a SystemVersionResponse message.
         *
         *  @return Result of the check.
         */
        TCheckResult
        CheckMessage( void ) const;
        
        /**
         *  Reads the Major Revision field in the message.
         *
         *  @return Value of Major Revision in the message.
         */
        TMajorVersion
        GetMajorVersion( void ) const;
        
        /**
         *  Reads the Minor Revision field in the message.
         *
         *  @return Value of Minor Revision in the message.
         */
        TMinorVersion
        GetMinorVersion( void ) const;
        
        /**
         *  Returns the size of the message.
         *
         *  @return Size of the current message.
         */
        TDataSize
        GetSize( void ) const;
    };
} /* namespace TerraSwarm */


#endif /* SYSTEMVERSIONPROMPT_H_ */
