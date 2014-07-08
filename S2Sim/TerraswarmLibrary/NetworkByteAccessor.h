/**
 * @file NetworkByteAccessor.h
 * Defines the NetworkByteAccessor class for byte order conversion and easier data access.
 *  @date Oct 8, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef NETWORKBYTEACCESSOR_H_
#define NETWORKBYTEACCESSOR_H_

#include <string.h>
#if ( defined(_WIN32) || defined(_WIN64) )
#include <winsock2.h>
#elif ( defined(__linux__) || defined(__APPLE__) )
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "CompileTimeCheckerLibrary.h"

namespace TerraSwarm
{
    /**
     *  Index of a byte in memory.
     */
    typedef unsigned int TByteIndex;
    
    /**
     *  Size of a data in memory.
     */
    typedef unsigned int TDataSize;

    /**
     *  Template class to automatically convert byte order and help access ordered bytes in the memory. This class is especially useful for message processing and preparing.
     
        @tparam byteIndex Index of the bytes that are to be manipulated/accessed.
        @tparam dataSize Number of bytes to be manipulated/accessed.
     */
    template <TByteIndex byteIndex, TDataSize dataSize>
    class NetworkByteAccessor
    {
        private:
        /**
         *  Template class that uses the correct conversion function according to the size of the data. The default specialization is for any size not equal to 1,2 or 4.
         *
         *  @tparam TInput Type of the field that is to be accessed.
            @tparam size Size of TInput, done automatically.
         */
            template <typename TInput, TDataSize size = sizeof( TInput )>
            class EndianConverter
            {
                public:
                /**
                 *  Converts the network byte order to host byte order.
                 *
                 *  @param value Values to be converted.
                 *
                 *  @return Converted result.
                 */
                    static TInput
                    NetworkToHost( const TInput & value )
                    {
                        return ( value );
                    }
                
                /**
                 *  Converts the host byte order to the network byte order.
                 *
                 *  @param value Value to be converted.
                 *
                 *  @return Converted value.
                 */
                    static TInput
                    HostToNetwork( const TInput & value )
                    {
                        return ( value );
                    }
            };

        /**
         *  Template specialization for a type with size 1 (char). No conversion is performed.
         */
            template <typename TInput>
            class EndianConverter<TInput, 1>
            {
                public:
                    static TInput
                    NetworkToHost( const TInput value )
                    {
                        return ( value );
                    }
                    static TInput
                    HostToNetwork( const TInput value )
                    {
                        return ( value );
                    }
            };

        /**
         *  Template specialization for a type with size 2 (short). Short conversion is performed.
         */
            template <typename TInput>
            class EndianConverter<TInput, 2>
            {
                public:
                    static TInput
                    NetworkToHost( const TInput value )
                    {
                        return ( ntohs( value ) );
                    }
                    static TInput
                    HostToNetwork( const TInput value )
                    {
                        return ( htons( value ) );
                    }
            };

        /**
         *  Template specialization for a type with size 4 (int). Integer conversion is performed.
         */
            template <typename TInput>
            class EndianConverter<TInput, 4>
            {
                public:
                    static TInput
                    NetworkToHost( const TInput value )
                    {
                        return ( ntohl( value ) );
                    }
                    static TInput
                    HostToNetwork( const TInput value )
                    {
                        return ( htonl( value ) );
                    }
            };

        public:
        /**
         *  Writes a value to the given address index with correct byte order. In addition, it checks for the size of the input type for mistakes in compile time.
         *
         *  @param input Value to be written.
         *
         *  @return Actually written value.
         */
            template <typename TInput>
            TInput
            Write( const TInput input )
            {
                SizeCheck<TInput, dataSize>::Check();
                TInput value = EndianConverter<TInput>::HostToNetwork( input );
                void* addressToBeWrittenTo = ( void* )( ( ( char* )this ) + byteIndex );
                memcpy( addressToBeWrittenTo, &value, dataSize );
                return ( value );
            }

        /**
         *  Same as Write() to simplify code writing.
         *
         *  @param input Value to be written.
         *
         *  @return Actually written value.
         */
            template <typename TInput>
            TInput
            operator =( const TInput input )
            {
                return ( this->Write( input ) );
            }

        /**
         *  Reads a value from the given address index with correct byte order. In addition, this function checks for the size of the input for mistakes inc ompile time.
         *
         *  @param value Reference to the value to be written to.
         *
         *  @return The value read from the address index.
         */
            template <typename TInput>
            TInput
            Read( TInput & value ) const
            {
                SizeCheck<TInput, dataSize>::Check();
                void* addressToBeReadFrom = ( void* )( ( ( char* )this ) + byteIndex );
                memcpy( &value, addressToBeReadFrom, dataSize );
                value = EndianConverter<TInput>::NetworkToHost( value );
                return ( value );
            }
    };

} /* namespace TerraSwarm */
#endif /* NETWORKBYTEACCESSOR_H_ */
