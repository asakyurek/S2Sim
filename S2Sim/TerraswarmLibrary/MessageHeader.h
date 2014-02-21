/**
 * @file MessageHeader.h
 * Defines the MessageHeader class.
 *  @date Oct 13, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef MESSAGEHEADER_H_
#define MESSAGEHEADER_H_

#include "NetworkByteAccessor.h"
#include <map>

namespace TerraSwarm
{
    /**
     *  This class defines the common message header for all messages.
     */
    class MessageHeader
    {
        public:
        /**
         *  Unique client id type.
         */
            typedef unsigned short TId;
        
        /**
         *  Unique Id of the sender.
         */
            typedef TId TSenderId;
        
        /**
         *  Unique Id of the receiver.
         */
            typedef TId TReceiverId;
        
        /**
         *  Type of the message for message parsing.
         */
            typedef unsigned short TMessageType;
        
        /**
         *  Id of the message for message parsing.
         */
            typedef unsigned short TMessageId;

        private:
        /**
         *  Indicates the start of the message.
         */
            typedef unsigned int TStartOfMessage;
        
        /**
         *  Defines the value for TStartOfMessage.
         */
            enum StartOfMessageValues
            {
                StartOfMessageDefaultValue = ( TStartOfMessage )0x12345678 /**< Only possible value for SOM */
            };

        /**
         *  Defines the message sequence number.
         */
            typedef unsigned int TSequenceNumber;

        /**
         *  Static function to get the next sequence number as the sequence number is sender unique, not message unique.
         *
         *  @param senderId   Id of the sender.
         *  @param receiverId Id of the intended receiver.
         *
         *  @return Next available sequence number.
         */
            static TSequenceNumber
            GetNextSequenceNumber( const TSenderId senderId, const TReceiverId receiverId );

        /**
         *  Size of the header fields.
         */
            enum HeaderFieldSizeValues
            {
                StartOfMessageSize  = ( TDataSize )sizeof( TStartOfMessage ),
                IdSize              = ( TDataSize )sizeof( TId ),
                SenderIdSize        = IdSize,
                ReceiverIdSize      = IdSize,
                SequenceNumberSize  = ( TDataSize )sizeof( TSequenceNumber ),
                MessageTypeSize     = ( TDataSize )sizeof( TMessageType ),
                MessageIdSize       = ( TDataSize )sizeof( TMessageId ),
                DataSizeSize        = ( TDataSize )sizeof( TDataSize )
            };

        /**
         *  Byte index values for the header fields.
         */
            enum HeaderFieldIndexValues
            {
                StartOfMessageIndex = ( TByteIndex )0,
                SenderIdIndex       = ( TByteIndex )( StartOfMessageIndex + StartOfMessageSize ),
                ReceiverIdIndex     = ( TByteIndex )( SenderIdIndex + SenderIdSize ),
                SequenceNumberIndex = ( TByteIndex )( ReceiverIdIndex + ReceiverIdSize ),
                MessageTypeIndex    = ( TByteIndex )( SequenceNumberIndex + SequenceNumberSize ),
                MessageIdIndex      = ( TByteIndex )( MessageTypeIndex + MessageTypeSize ),
                DataSizeIndex       = ( TByteIndex )( MessageIdIndex + MessageIdSize ),
                DataIndex           = ( TByteIndex )( DataSizeIndex + DataSizeSize )
            };

        /**
         *  Manages the accessing of the Start of Message field.
         */
            typedef NetworkByteAccessor<StartOfMessageIndex, StartOfMessageSize> TStartOfMessageAccessor;
        
        /**
         *  Manages the accessing of the Sender Id field.
         */
            typedef NetworkByteAccessor<SenderIdIndex, SenderIdSize> TSenderIdAccessor;
        
        /**
         *  Manages the accessing of the Receiver Id field.
         */
            typedef NetworkByteAccessor<ReceiverIdIndex, ReceiverIdSize> TReceiverIdAccessor;
        
        /**
         *  Manages the accessing of the Sequence Number field.
         */
            typedef NetworkByteAccessor<SequenceNumberIndex, SequenceNumberSize> TSequenceNumberAccessor;
        
        /**
         *  Manages the accessing of the Message Type field.
         */
            typedef NetworkByteAccessor<MessageTypeIndex, MessageTypeSize> TMessageTypeAccessor;
        
        /**
         *  Manages the accessing of the Message Id field.
         */
            typedef NetworkByteAccessor<MessageIdIndex, MessageIdSize> TMessageIdAccessor;
        
        /**
         *  Manages the accessing of the Data Size field.
         */
            typedef NetworkByteAccessor<DataSizeIndex, DataSizeSize> TDataSizeAccessor;

        public:
        /**
         *  Defines various size values for the Message Header.
         */
            enum SizeValues
            {
                MessageHeaderSize   = ( StartOfMessageSize +
                                        SenderIdSize +
                                        ReceiverIdSize +
                                        SequenceNumberSize +
                                        MessageTypeSize +
                                        MessageIdSize +
                                        DataSizeSize ) /**< Size of the message header. */
            };

        private:
        /**
         *  Not used.
         */
            MessageHeader( void );

        /**
         *  Template function to use the accessor of any field to access that field.
         *
            @tparam Accessor The accessor type for the desired field.
         *  @return Returns the accessor for the desired field.
         */
            template <typename Accessor>
            Accessor* Access( void )
            {
                return ( ( Accessor* )this );
            }

        /**
         *  Template function to use the accessor of any field to access that field in a constant fashion.
         *
            @tparam Accessor The accessor type for the desired field.
         *  @return Returns the accessor for the desired field.
         */

            template <typename Accessor>
            const Accessor* Access( void ) const
            {
                return ( ( const Accessor* )this );
            }

        public:
        /**
         *  Static function that creates a buffer for a new message header.
         *
         *  @return Returns the address of the allocated buffer.
         */
            static MessageHeader*
            GetNewMessageHeader( void );

        /**
         *  Sets all the fields of the message header and prepares it.
         *
         *  @param senderId    Id of the sender.
         *  @param receiverId  Id of the receiver.
         *  @param messageType Message type field.
         *  @param messageId   Message Id field.
         *  @param dataSize    Data Size field.
         */
            void
            PrepareOutgoingMessage( const TSenderId senderId,
                                    const TReceiverId receiverId,
                                    const TMessageType messageType,
                                    const TMessageId messageId,
                                    const TDataSize dataSize );

        /**
         *  Gets the Sender Id.
         *
         *  @return Id of the Sender.
         */
            TSenderId
            GetSenderId( void ) const
            {
                TSenderId value;
                this->Access<TSenderIdAccessor>()->Read( value );
                return ( value );
            }

        /**
         *  Gets the Receiver Id.
         *
         *  @return Id of the Receiver.
         */
            TReceiverId
            GetReceiverId( void ) const
            {
                TReceiverId value;
                this->Access<TReceiverIdAccessor>()->Read( value );
                return ( value );
            }

        /**
         *  Gets the message type.
         *
         *  @return Message Type field value.
         */
            TMessageType
            GetMessageType( void ) const
            {
                TMessageType value;
                this->Access<TMessageTypeAccessor>()->Read( value );
                return ( value );
            }

        /**
         *  Gets the Message Id.
         *
         *  @return Message Id field value.
         */
            TMessageId
            GetMessageId( void ) const
            {
                TMessageId value;
                this->Access<TMessageIdAccessor>()->Read( value );
                return ( value );
            }

        /**
         *  Gets the Data Size.
         *
         *  @return Size of the user data.
         */
            TDataSize
            GetDataSize( void ) const
            {
                TDataSize value;
                this->Access<TDataSizeAccessor>()->Read( value );
                return ( value );
            }
    };
} /* namespace TerraSwarm */
#endif /* MESSAGEHEADER_H_ */
