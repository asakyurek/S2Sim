/**
 * @file SetCurrentPrice.h
 * Defines the SetCurrentPrice class.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef SETCURRENTPRICE_H_
#define SETCURRENTPRICE_H_

#include "MessageHeader.h"
#include "MessageEnder.h"

namespace TerraSwarm
{
    namespace Synchronous
    {

        /**
         *  Set Current Price message sent for the Controller to the clients to set the current price and advance the time frame.
         */
        class SetCurrentPrice
        {
            private:
            /**
             *  Message header values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0002
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
            
            /**
             *  Type for number of price points.
             */
                typedef unsigned int TNumberOfPricePoints;

            private:
            /**
             *  Size values for the data fields.
             */
                enum FieldSizeValues
                {
                    IntervalBeginSize = sizeof( TInterval ),
                    NumberOfPricePointsSize = sizeof( TNumberOfPricePoints ),
                    PriceSize = sizeof( TPrice )
                };

            /**
             *  Index values for the fata fields.
             */
                enum FieldIndexValues
                {
                    IntervalBeginIndex = MessageHeader::MessageHeaderSize,
                    NumberOfPricePointsIndex = IntervalBeginIndex + IntervalBeginSize,
                    PricePointsIndex = NumberOfPricePointsIndex + NumberOfPricePointsSize,
                };

            /**
             *  Accessor helper for the Number of Price Points field.
             */
                typedef NetworkByteAccessor<NumberOfPricePointsIndex, NumberOfPricePointsSize> TNumberOfPricePointsAccessor;
            
            /**
             *  Accessor helper for the IntervalBegin field.
             */
                typedef NetworkByteAccessor<IntervalBeginIndex, IntervalBeginSize> TIntervalBeginAccessor;

            private:
            /**
             *  No use. Private constructor to force usage of the static creation method.
             */
                SetCurrentPrice( void );

            public:
            /**
             *  Deallocates the memory for the message.
             */
                ~SetCurrentPrice( void );

            /**
             *  Creates a new SetCurrentPrice message and allocates memory for it. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId      Id of the sender.
             *  @param receiverId    Id of the receiver.
             *  @param price         Price signal to be set.
             *  @param intervalBegin Beginning of the price interval.
             *  @param numberOfPricePoints Number of price points sent within the message.
             *  @param pricePoints   Price points sent to the client.
             *
             *  @return Returns a new allocated message.
             */
                static SetCurrentPrice*
                GetNewSetCurrentPrice( const MessageHeader::TSenderId senderId,
                                       const MessageHeader::TReceiverId receiverId,
                                       const TInterval intervalBegin,
                                       const TNumberOfPricePoints numberOfPricePoints,
                                       TPrice* pricePoints );

            /**
             *  Checks whether the current memory contains a SetCurrentPrice message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the price form the current message.
             *
             *  @return Price value in the message.
             */
                TPrice*
                GetPrice( void ) const;
            
            /**
             *  Reads the IntervalBegin field in the message.
             *
             *  @return Value of IntervalBegin in the message.
             */
                TInterval
                GetIntervalBegin( void ) const;

            /**
             *  Reads the NumberOfPricePoints field in the message.
             *
             *  @return Value of NumberOfPricePoints in the message.
             */
                TInterval
                GetNumberOfPricePoints( void ) const;
            
            /**
             *  Returns the size of the message.
             *  
             *  @return Size of the current message.
             */
                TDataSize
                GetSize() const;
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* SETCURRENTPRICE_H_ */
