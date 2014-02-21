/**
 * @file PriceProposal.h
 * Defines the PriceProposal class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef PRICEPROPOSAL_H_
#define PRICEPROPOSAL_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Synchronous
    {

        /**
         *  Price Proposal message sent by the controller to the clients to propose a price.
         */
        class PriceProposal
        {
            private:
            /**
             *  Message Header values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0003
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
             *  Type for price signal.
             */
                typedef unsigned int TPrice;
            
            /**
             *  Type for time interval.
             */
                typedef unsigned int TInterval;

            private:
            /**
             *  Size of the message fields.
             */
                enum FieldSizeValues
                {
                    PriceSize = sizeof( TPrice ),
                    IntervalBeginSize = sizeof( TInterval ),
                    IntervalEndSize = sizeof( TInterval ),
                    TotalSize = ( PriceSize + IntervalBeginSize + IntervalEndSize )
                };

            /**
             *  Index values for the message fields.
             */
                enum FieldIndexValues
                {
                    PriceIndex = MessageHeader::MessageHeaderSize,
                    IntervalBeginIndex = PriceIndex + PriceSize,
                    IntervalEndIndex = IntervalBeginIndex + IntervalBeginSize
                };

            /**
             *  Accessor helper for Price field.
             */
                typedef NetworkByteAccessor<PriceIndex, PriceSize> TPriceAccessor;
            
            /**
             *  Accessor helper for IntervalBegin field.
             */
                typedef NetworkByteAccessor<IntervalBeginIndex, IntervalBeginSize> TIntervalBeginAccessor;
            
            /**
             *  Accessor helper for IntervalEnd field.
             */
                typedef NetworkByteAccessor<IntervalEndIndex, IntervalEndSize> TIntervalEndAccessor;

            private:
            /**
             *  Empty private function to force the usage of the static method.
             */
                PriceProposal( void );

            public:
            /**
             *  Deletes the allocated message.
             */
                ~PriceProposal( void );

            /**
             *  Creates a new PriceProposal message and allocates the necessary memory for it. @warning Deallocation is the job of the user.
             *
             *  @param senderId      Id of the sender.
             *  @param receiverId    Id of the receiver.
             *  @param price         Proposed price.
             *  @param intervalBegin Beginning of the price interval.
             *  @param intervalEnd   End of the price interval.
             *
             *  @return Pointer to the allocated message.
             */
                static PriceProposal*
                GetNewPriceProposal( const MessageHeader::TSenderId senderId,
                                     const MessageHeader::TReceiverId receiverId,
                                     const TPrice price,
                                     const TInterval intervalBegin,
                                     const TInterval intervalEnd );

            /**
             *  Checks whether the current memory contains the PriceProposal message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the price field.
             *
             *  @return Price value in the message.
             */
                TPrice
                GetPrice( void ) const;

            /**
             *  Reads the IntervalBegin field in the message.
             *
             *  @return IntervalBegin value in the message.
             */
                TInterval
                GetIntervalBegin( void ) const;

            /**
             *  Reads the IntervalEnd field in the message.
             *
             *  @return IntervalEnd value in the message.
             */
                TInterval
                GetIntervalEnd( void ) const;

            /**
             *  Returns the size of this message.
             *
             *  @return Size of a PriceProposal message.
             */
                static TDataSize
                GetSize( void );
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* PRICEPROPOSAL_H_ */
