/**
 * @file TCPServer.h
 * Defines the TCPServer class.
 *  @date Jan 21, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "SocketBase.h"
#include "IPAddress.h"
#include "ThreadedTCPConnectedClient.h"

/**
 *  Defines a TCP server that can listen to connection attempts and can accept them for communication.
 */
class TCPServer : public SocketBase<SOCK_STREAM>
{
    public:
    /**
     *  Not used.
     */
        TCPServer( void );
    
    /**
     *  Not used.
     */
        ~TCPServer( void );

    /**
     *  Sets the IP address of the server. This method is not recommended as the IP address of the computer is mostly set, while only the port number is required. If you are not sure, use TCPServer::SetPort() instead.
     *
     *  @param address IP Address to be used.
     */
        void
        SetAddress( IPAddress & address );

    /**
     *  Sets the listening port of the server.
     *
     *  @param port Port to be listened to for incoming connection attempts.
     */
        void
        SetPort( const IPAddress::TPort port );

    /**
     *  This method makes the server listen to incoming connection attempts.
     *
     *  @return Returns the success of the listen attempt.
     */
        bool
        Listen( void );

    /**
     *  This method makes the TCP server accept any incoming connection attempt. This is a blocking call. If any client is accepted, a ThreadedTCPConnectedClient instance is returned for client management and communication.
     *
     *  @return An instance of ThreadedTCPConnectedClient for client management and communication.
     */
        ThreadedTCPConnectedClient*
        Accept( void );

    private:
    /**
     *  Defines the type of the base class for rapid development.
     */
        typedef SocketBase<SOCK_STREAM> TBaseType;
};

#endif /* TCPSERVER_H_ */
