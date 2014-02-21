/**
 * @file TCPClient.h
 * Defines the TCPClient class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "SocketBase.h"
#include "IPAddress.h"

class TCPClient : public SocketBase<SOCK_STREAM>
{
    public:
    /**
     *  No use.
     */
        TCPClient( void );
    
    /**
     *  No use.
     */
        ~TCPClient( void );

    /**
     *  Starts a TCP connection to a server.
     *
     *  @param address IPAddress of the server.
     *
     *  @return Success of the communication.
     */
        bool
        Connect( IPAddress & address );

    /**
     *  Sends data to the server.
     *
     *  @param buffer Buffer, holding the data to be sent.
     *  @param length Length of the data to be sent.
     *
     *  @return Actually sent number of bytes.
     */
        TNumberOfBytes
        SendData( const TBuffer buffer, const TNumberOfBytes length );

    /**
     *  Received data from the server. This is a blocking call.
     *
     *  @param buffer          Buffer that will hold the received data.
     *  @param receptionLength Length of the buffer and maximum receivable data.
     *
     *  @return The actual number of bytes received.
     */
        TNumberOfBytes
        ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength );

    private:
    /**
     *  Defines the base type for rapid development.
     */
        typedef SocketBase<SOCK_STREAM> TBaseType;
};

#endif /* TCPCLIENT_H_ */
