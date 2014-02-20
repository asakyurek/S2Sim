/*
 * PriceProposal.h
 *
 *  Created on: Oct 31, 2013
 *      Author: Alper
 */

#ifndef PRICEPROPOSAL_H_
#define PRICEPROPOSAL_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Synchronous
    {

        class PriceProposal
        {
            private:
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0003
                };

            public:
                typedef bool TCheckResult;
                enum CheckResultValues
                {
                    Success = ( TCheckResult )true,
                    Fail = ( TCheckResult )false
                };

                typedef unsigned int TPrice;
                typedef unsigned int TInterval;

            private:
                enum FieldSizeValues
                {
                    PriceSize = sizeof( TPrice ),
                    IntervalBeginSize = sizeof( TInterval ),
                    IntervalEndSize = sizeof( TInterval ),
                    TotalSize = ( PriceSize + IntervalBeginSize + IntervalEndSize )
                };

                enum FieldIndexValues
                {
                    PriceIndex = MessageHeader::MessageHeaderSize,
                    IntervalBeginIndex = PriceIndex + PriceSize,
                    IntervalEndIndex = IntervalBeginIndex + IntervalBeginSize
                };

                typedef NetworkByteAccessor<PriceIndex, PriceSize> TPriceAccessor;
                typedef NetworkByteAccessor<IntervalBeginIndex, IntervalBeginSize> TIntervalBeginAccessor;
                typedef NetworkByteAccessor<IntervalEndIndex, IntervalEndSize> TIntervalEndAccessor;

            private:
                PriceProposal( void );

            public:
                ~PriceProposal( void );

                static PriceProposal*
                GetNewPriceProposal( const MessageHeader::TSenderId senderId,
                                     const MessageHeader::TReceiverId receiverId,
                                     const TPrice price,
                                     const TInterval intervalBegin,
                                     const TInterval intervalEnd );

                TCheckResult
                CheckMessage( void ) const;

                TPrice
                GetPrice( void ) const;

                TInterval
                GetIntervalBegin( void ) const;

                TInterval
                GetIntervalEnd( void ) const;

                static TDataSize
                GetSize( void );
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* PRICEPROPOSAL_H_ */
