/*
 * MessageHeader.h
 *
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef MESSAGEHEADER_H_
#define MESSAGEHEADER_H_

#include "NetworkByteAccessor.h"
#include <map>

namespace TerraSwarm
{
    class MessageHeader
    {
        public:
            typedef unsigned short TId;
            typedef TId TSenderId;
            typedef TId TReceiverId;
            typedef unsigned short TMessageType;
            typedef unsigned short TMessageId;

        private:
            typedef unsigned int TStartOfMessage;
            enum StartOfMessageValues
            {
                StartOfMessageDefaultValue = ( TStartOfMessage )0x12345678
            };

            typedef unsigned int TSequenceNumber;

            static TSequenceNumber
            GetNextSequenceNumber( const TSenderId senderId, const TReceiverId receiverId );

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

            typedef NetworkByteAccessor<StartOfMessageIndex, StartOfMessageSize> TStartOfMessageAccessor;
            typedef NetworkByteAccessor<SenderIdIndex, SenderIdSize> TSenderIdAccessor;
            typedef NetworkByteAccessor<ReceiverIdIndex, ReceiverIdSize> TReceiverIdAccessor;
            typedef NetworkByteAccessor<SequenceNumberIndex, SequenceNumberSize> TSequenceNumberAccessor;
            typedef NetworkByteAccessor<MessageTypeIndex, MessageTypeSize> TMessageTypeAccessor;
            typedef NetworkByteAccessor<MessageIdIndex, MessageIdSize> TMessageIdAccessor;
            typedef NetworkByteAccessor<DataSizeIndex, DataSizeSize> TDataSizeAccessor;

        public:
            enum SizeValues
            {
                MessageHeaderSize   = ( StartOfMessageSize +
                                        SenderIdSize +
                                        ReceiverIdSize +
                                        SequenceNumberSize +
                                        MessageTypeSize +
                                        MessageIdSize +
                                        DataSizeSize )
            };

        private:
            MessageHeader( void );

            template <typename Accessor>
            Accessor* Access( void )
            {
                return ( ( Accessor* )this );
            }

            template <typename Accessor>
            const Accessor* Access( void ) const
            {
                return ( ( const Accessor* )this );
            }

        public:
            static MessageHeader*
            GetNewMessageHeader( void );

            void
            PrepareOutgoingMessage( const TSenderId senderId,
                                    const TReceiverId receiverId,
                                    const TMessageType messageType,
                                    const TMessageId messageId,
                                    const TDataSize dataSize );

            TSenderId
            GetSenderId( void ) const
            {
                TSenderId value;
                this->Access<TSenderIdAccessor>()->Read( value );
                return ( value );
            }

            TReceiverId
            GetReceiverId( void ) const
            {
                TReceiverId value;
                this->Access<TReceiverIdAccessor>()->Read( value );
                return ( value );
            }

            TMessageType
            GetMessageType( void ) const
            {
                TMessageType value;
                this->Access<TMessageTypeAccessor>()->Read( value );
                return ( value );
            }

            TMessageId
            GetMessageId( void ) const
            {
                TMessageId value;
                this->Access<TMessageIdAccessor>()->Read( value );
                return ( value );
            }

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
