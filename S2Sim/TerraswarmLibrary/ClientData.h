/**
 * @file ClientData.h
 * Defines the Aysnchronous and Synchronous ClientData class and messages.
 *  Created on: Oct 13, 2013
 *      Author: Alper
 */

#ifndef CLIENTDATA_H_
#define CLIENTDATA_H_

#include "MessageHeader.h"
#include "MessageEnder.h"
#include <string.h>
#include <memory>

namespace TerraSwarm
{
    namespace Asynchronous
    {

        /**
         *  Asynchronous Client Data message from the client to indicate its consumption for a time interval.
         */
        class ClientData
        {
            private:
            /**
             *  Message Header values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0001,
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
                    Success = ( TCheckResult )true, /**< Message is of correct type and id. **/
                    Fail = ( TCheckResult )false /**< Message has incorrect type or id. **/
                };

            /**
             *  Starting time of the sent data points.
             */
                typedef unsigned int TStartTime;
            
            /**
             *  Time interval between consecutive data points.
             */
                typedef unsigned int TTimeResolution;
            
            /**
             *  Number of data points in the message.
             */
                typedef unsigned int TNumberOfDataPoints;
            
            /**
             *  General type for a data point representing consumption for this case.
             */
                typedef unsigned int TDataPoint;

            private:
            /**
             *  Size of the message fields.
             */
                enum FieldSizeValues
                {
                    StartTimeSize = sizeof( TStartTime ),
                    TimeResolutionSize = sizeof( TTimeResolution ),
                    NumberOfDataPointsSize = sizeof( TNumberOfDataPoints ),
                    DataPointSize = sizeof( TDataPoint )
                };

            /**
             *  Index of the message fields.
             */
                enum FieldIndexValues
                {
                    StartTimeIndex = MessageHeader::MessageHeaderSize,
                    TimeResolutionIndex = StartTimeIndex + StartTimeSize,
                    NumberOfDataPointsIndex = TimeResolutionIndex + TimeResolutionSize,
                    DataStartIndex = NumberOfDataPointsIndex + NumberOfDataPointsSize
                };

            /**
             *  Accessor helper for StartTime field.
             */
                typedef NetworkByteAccessor<StartTimeIndex, StartTimeSize> TStartTimeAccessor;
            
            /**
             *  Accessor helper for TimeResolution field.
             */
                typedef NetworkByteAccessor<TimeResolutionIndex, TimeResolutionSize> TTimeResolutionAccessor;
            
            /**
             *  Accessor helper for NumberOfDataPoints field.
             */
                typedef NetworkByteAccessor<NumberOfDataPointsIndex, NumberOfDataPointsSize> TNumberOfDataPointsAccessor;

            private:
            /**
             *  Private constructor to force the usage of the static construction method.
             */
                ClientData( void );

            public:
            /**
             *  Deallocates the memory.
             */
                ~ClientData( void );

            /**
             *  Creates a new Asnychronous ClientData message and allocates memory for it. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId           Id of the Sender.
             *  @param receiverId         Id of the Receiver.
             *  @param startTime          Starting time for the first consumption data.
             *  @param timeResolution     Time interval between consecutive data points.
             *  @param numberOfDataPoints Number of data points in the message.
             *  @param dataPoints         Pointer to the buffer holding the data points.
             *
             *  @return Newly allocated ClientData message.
             */
                static ClientData*
                GetNewClientData( const MessageHeader::TSenderId senderId,
                                  const MessageHeader::TReceiverId receiverId,
                                  const TStartTime startTime,
                                  const TTimeResolution timeResolution,
                                  const TNumberOfDataPoints numberOfDataPoints,
                                  TDataPoint* dataPoints );

            /**
             *  Checks whether the current memory contains a ClientData message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the StartTime field.
             *
             *  @return StartTime value.
             */
                TStartTime
                GetStartTime( void ) const;

            /**
             *  Reads the TimeResolution field.
             *
             *  @return TimeResolution value.
             */
                TTimeResolution
                GetTimeResolution( void ) const;

            /**
             *  Reads the NumberOfDataPoints field.
             *
             *  @return Number of data points in the message.
             */
                TNumberOfDataPoints
                GetNumberOfDataPoints( void ) const;

            /**
             *  Gets the pointer to the data points in the message.
             *
             *  @return Pointer to the beginning of data points.
             */
                std::shared_ptr<TDataPoint>
                GetDataPoints( void ) const;
        };

    } /* namespace Asynchronous */

    namespace Synchronous
    {
        /**
         *  Synchronous Client Data message from the client to indicate its consumption for a time interval.
         */
        class ClientData
        {
            private:
            /**
             *  Message Header Values.
             */
                enum HeaderValues
                {
                    MessageType = 0x0003,
                    MessageId = 0x0005
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
             *  Defines the type for a general data point, consumption in this case.
             */
                typedef unsigned int TDataPoint;

            private:
            /**
             *  Size of the fields within the message.
             */
                enum FieldSizeValues
                {
                    DataPointSize = sizeof( TDataPoint )
                };

            /**
             *  Index of the fields within the message.
             */
                enum FieldIndexValues
                {
                    DataStartIndex = MessageHeader::MessageHeaderSize
                };

            /**
             *  Accessor helper for the DataPoint field.
             */
                typedef NetworkByteAccessor<DataStartIndex, DataPointSize> TDataPointAccessor;

            private:
            /**
             *  Private constructor to force the usage of the static construction method.
             */
                ClientData( void );

            public:
            /**
             *  Deallocates the memory.
             */
                ~ClientData( void );

            /**
             *  Constructs a new ClientData message and allocates the necessary memory for it. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId   Id of the sender.
             *  @param receiverId Id of the receiver.
             *  @param dataPoint  Consumption of the user for the next time interval.
             *
             *  @return Newly allocated ClientData message.
             */
                static ClientData*
                GetNewClientData( const MessageHeader::TSenderId senderId,
                                  const MessageHeader::TReceiverId receiverId,
                                  TDataPoint dataPoint );

            /**
             *  Checks whether the current memory contains a ClientData message.
             *
             *  @return Result of the check.
             */
                TCheckResult
                CheckMessage( void ) const;

            /**
             *  Reads the data point in the message.
             *
             *  @return Data point value.
             */
                TDataPoint
                GetDataPoint( void ) const;
        };

        /**
         *  Synchronous Client Data message from the client to indicate its consumption for a time interval.
         */
        class ClientExtendedData
        {
            private:
            /**
             *  Message Header Values.
             */
            enum HeaderValues
            {
                MessageType = 0x0003,
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
                Success = ( TCheckResult )true, /**< Message is of correct type and id. **/
                Fail = ( TCheckResult )false /**< Message has incorrect type or id. **/
            };
            
            /**
             *  Defines the type for a general data point, consumption in this case.
             */
            typedef unsigned int TDataPoint;
            
            /**
             *  Defines the number of data points within the message.
             */
            typedef unsigned int TNumberOfDataPoints;
            
            private:
            /**
             *  Size of the fields within the message.
             */
            enum FieldSizeValues
            {
                DataPointSize = sizeof( TDataPoint ),
                NumberOfDataPointsSize = sizeof( TNumberOfDataPoints )
            };
            
            /**
             *  Index of the fields within the message.
             */
            enum FieldIndexValues
            {
                NumberOfDataPointsIndex = MessageHeader::MessageHeaderSize,
                DataStartIndex = NumberOfDataPointsIndex + NumberOfDataPointsSize
            };
            
            /**
             *  Accessor helper for the NumberOfDataPoints field.
             */
            typedef NetworkByteAccessor<NumberOfDataPointsIndex, NumberOfDataPointsSize> TNumberOfDataPointsAccessor;
            
        private:
            /**
             *  Private constructor to force the usage of the static construction method.
             */
            ClientExtendedData( void );
            
        public:
            /**
             *  Deallocates the memory.
             */
            ~ClientExtendedData( void );
            
            /**
             *  Constructs a new ClientData message and allocates the necessary memory for it. @warning Deallocation is the responsibility of the user.
             *
             *  @param senderId   Id of the sender.
             *  @param receiverId Id of the receiver.
             *  @param numberOfDataPoints Number of data points in the message.
             *  @param dataPoints  Consumption of the user for the next time interval and, if exists, prediction for the later intervals.
             *
             *  @return Newly allocated ClientData message.
             */
            static ClientExtendedData*
            GetNewClientData( const MessageHeader::TSenderId senderId,
                             const MessageHeader::TReceiverId receiverId,
                             const TNumberOfDataPoints numberOfDataPoints,
                             TDataPoint* dataPoints );
            
            /**
             *  Checks whether the current memory contains a ClientExtendedData message.
             *
             *  @return Result of the check.
             */
            TCheckResult
            CheckMessage( void ) const;
            
            /**
             *  Reads the number of data points in the message.
             *
             *  @return Nuber of Data points value.
             */
            TNumberOfDataPoints
            GetNumberOfDataPoints( void ) const;
            
            /**
             *  Copies the data points in a new allocated memory. @warning Deallocation is the responsibility of the user.
             *
             *  @return Array containing the data points.
             */
            std::shared_ptr<TDataPoint>
            GetDataPoints( void ) const;
        };
        
    } /* namespace Synchronous */
} /* namespace TerraSwarm */
#endif /* CLIENTDATA_H_ */
