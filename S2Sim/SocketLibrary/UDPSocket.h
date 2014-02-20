/*
 * UDPSocket.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Alper
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include "SocketBase.h"
#include "IPAddress.h"

class UDPSocket : private SocketBase<SOCK_DGRAM>
{
    public: /* Type Definitions */

    public: /* Member Functions */
        UDPSocket( void );
        ~UDPSocket( void );

    public: /* Members */
        void
        SetIPAddress( IPAddress & address );

        void
        SetPort( const IPAddress::TPort port );

        TNumberOfBytes
        SendData( const TBuffer buffer, const TNumberOfBytes length, IPAddress & destination );

        TNumberOfBytes
        ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength, IPAddress & destinationAddress );

    private:
        typedef SocketBase<SOCK_DGRAM> TBaseType;
};

#endif /* UDPSOCKET_H_ */
