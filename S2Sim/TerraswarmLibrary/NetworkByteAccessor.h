/*
 * NetworkByteAccessor.h
 *
 *  Created on: Oct 8, 2013
 *      Author: Alper
 */

#ifndef NETWORKBYTEACCESSOR_H_
#define NETWORKBYTEACCESSOR_H_

#include <string.h>
#if ( defined(_WIN32) || defined(_WIN64) )
#include <winsock2.h>
#elif ( defined(__linux__) || defined(__APPLE__) )
#include <sys/socket.h>
#endif

#include "CompileTimeCheckerLibrary.h"

namespace TerraSwarm
{
    typedef unsigned int TByteIndex;
    typedef unsigned int TDataSize;

    template <TByteIndex byteIndex, TDataSize dataSize>
    class NetworkByteAccessor
    {
        private:
            template <typename TInput, TDataSize size = sizeof( TInput )>
            class EndianConverter
            {
                public:
                    static TInput
                    NetworkToHost( const TInput & value )
                    {
                        return ( value );
                    }
                    static TInput
                    HostToNetwork( const TInput & value )
                    {
                        return ( value );
                    }
            };

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

            template <typename TInput>
            TInput
            operator =( const TInput input )
            {
                return ( this->Write( input ) );
            }

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
