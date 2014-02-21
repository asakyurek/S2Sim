/**
 * @file UDPSocket.h
 * Defines the UDPSocket class.
 *  @date Jun 21, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include "SocketBase.h"
#include "IPAddress.h"

/**
 *  Manages a UDP connection. If the port is selected, this socket can send to any UDP socket.
 */
class UDPSocket : private SocketBase<SOCK_DGRAM>
{
    public:
    /**
     *  Not used.
     */
        UDPSocket( void );
    
    /**
     *  Not used.
     */
        ~UDPSocket( void );

    /**
     *  Sets the IP Address of the socket. This method is not recommended as the IP Address of the user is already set. If the user is not sure, UDPSocket::SetPort() is the usual usage.
     *
     *  @param address IP Address of the socket.
     */
        void
        SetIPAddress( IPAddress & address );

    /**
     *  Sets the port of the UDP Socket.
     *
     *  @param port The port that the destination will send its data to.
     */
        void
        SetPort( const IPAddress::TPort port );

    /**
     *  Sends a data to another UDP socket.
     *
     *  @param buffer      Buffer, containing the data to be sent.
     *  @param length      Length of the data to be sent.
     *  @param destination IP Address of the destination socket.
     *
     *  @return Returns the actual number of bytes sent.
     */
        TNumberOfBytes
        SendData( const TBuffer buffer, const TNumberOfBytes length, IPAddress & destination );

    /**
     *  Receives data from another UDP Socket. This is a blocking call.
     *
     *  @param buffer             An allocated buffer where the received data will be stored in.
     *  @param receptionLength    Length of the buffer and the maximum reception length.
     *  @param destinationAddress IP Address, from which the data will be received.
     *
     *  @return Returns the actual received number of bytes.
     */
        TNumberOfBytes
        ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength, IPAddress & destinationAddress );

    private:
    /**
     *  Defines the base class for rapid development.
     */
        typedef SocketBase<SOCK_DGRAM> TBaseType;
};

#endif /* UDPSOCKET_H_ */
