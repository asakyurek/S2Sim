/**
 * @file TCPConnectedClient.h
 * Defines the TCPConnectedClient class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef TCPCONNECTEDCLIENT_H_
#define TCPCONNECTEDCLIENT_H_

#include "SocketBase.h"
#include "IPAddress.h"

/**
 *  Manages the connection to a connected client on the server side. When a client is accepted, an instance of the class is initialized to furthur maintain the connection.
 */
class TCPConnectedClient : public SocketBase<SOCK_STREAM>
{
    public:
    /**
     *  Initializes the class with a ready socket and address.
        @param socketId Handle to the socket.
        @param clientAddress IPAddress of the connected client.
     */
        TCPConnectedClient( const TSocketId socketId, IPAddress & clientAddress );
    
    /**
     *  Copies the contents of another instance.
        @param copy Instance to be copied.
     */
        TCPConnectedClient( const TCPConnectedClient & copy );
    
    /**
     *  Not used.
     */
        ~TCPConnectedClient( void );

    /**
     *  Sends data to the connected client.
     *
     *  @param buffer Buffer, holding the data to be sent.
     *  @param length Length of the data to be sent.
     *
     *  @return Actual number of bytes sent.
     */
        TNumberOfBytes
        SendData( const TBuffer buffer, const TNumberOfBytes length );

    /**
     *  Receives data from the connected client. This is a blocking call.
     *
     *  @param buffer          Buffer, where the received data will be stored int.
     *  @param receptionLength Length of the buffer and the maximum receivable data size.
     *
     *  @return The actual number of bytes received.
     */
        TNumberOfBytes
        ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength );

    /**
     *  Returns the IP address of the client.
     *
     *  @return IP address of the client.
     */
        IPAddress
        GetClientAddress( void ) const;

    private:
    /**
     *  Defines the base class for rapid development.
     */
        typedef SocketBase<SOCK_STREAM> TBaseType;

    private:
    /**
     *  IP Address of the client.
     */
        IPAddress m_clientAddress;
};

#endif /* TCPCONNECTEDCLIENT_H_ */
