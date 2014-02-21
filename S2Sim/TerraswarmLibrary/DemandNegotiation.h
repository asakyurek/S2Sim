/**
 * @file DemandNegotiation.h
 * Defines the DemandNegotiation class and message.
 *  @date Oct 31, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef DEMANDNEGOTIATION_H_
#define DEMANDNEGOTIATION_H_

#include "MessageHeader.h"
#include "MessageEnder.h"
#include <string.h>

namespace TerraSwarm
{
    namespace Synchronous
    {

        /**
         *  Defines the DemandNegotiation message sent from the client to the Controller as a response to the price proposal.
         */
        class DemandNegotiation
        {
            private:
            /**
             *  Message Header values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0004
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
                    Success = ( TCheckResult )true, /**< Message is of correct type and id. **/
                    Fail = ( TCheckResult )false /**< Message has incorrect type or id. **/
                };

            /**
             *  Defines the type for number of data points.
             */
                typedef unsigned int TNumberOfDataPoints;
            
            /**
             *  Defines a general data point, used for consumption in this message.
             */
                typedef unsigned int TDataPoint;

            private:
            /**
             *  Size of the fields in the message.
             */
                enum FieldSizeValues
                {
                    NumberOfDataPointsSize = sizeof( TNumberOfDataPoints ),
                    DataPointSize = sizeof( TDataPoint )
                };

            /**
             *  Index of the fields in the message.
             */
                enum FieldIndexValues
                {
                    NumberOfDataPointsIndex = MessageHeader::MessageHeaderSize,
                    DataStartIndex = NumberOfDataPointsIndex + NumberOfDataPointsSize
                };

            /**
             *  Accessor Helper for NumberOfDataPoints field.
             */
                typedef NetworkByteAccessor<NumberOfDataPointsIndex, NumberOfDataPointsSize> TNumberOfDataPointsAccessor;

            private:
            /**
             *  Private constructor to force the usage of the static creation method.
             */
                DemandNegotiation( void );

            public:
            /**
             *  Deallocates the memory.
             */
                ~DemandNegotiation( void );

            /**
             *  Creates a new DemandNegotiation message and allocates the memory. @warning Dellocation is the responsibility of the user.
             *
             *  @param senderId           Id of the sender.
             *  @param receiverId         Id of the receiver.
             *  @param numberOfDataPoints Number of data points in the message.
             *  @param dataPoints         Pointer to the buffer holding the data points.
             *
             *  @return Pointer to the newly allocated message.
             */
                static DemandNegotiation*
                GetNewDemandNegotiation( const MessageHeader::TSenderId senderId,
                                         const MessageHeader::TReceiverId receiverId,
                                         const TNumberOfDataPoints numberOfDataPoints,
                                         TDataPoint* dataPoints );

            /**
             *  Checks whether the current memory contains a DemandNegotiation message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the number of data points in the message.
             *
             *  @return Number of data points.
             */
                TNumberOfDataPoints
                GetNumberOfDataPoints( void ) const;

            /**
             *  Gets the pointer to the data points field.
             *
             *  @return Pointer to the start of the data points field.
             */
                TDataPoint*
                GetDataPoints( void ) const;
        };

    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* DEMANDNEGOTIATION_H_ */
