/*
 * TCPServer.h
 *
 *  Created on: Jan 21, 2014
 *      Author: asakyurek
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "SocketBase.h"
#include "IPAddress.h"
#include "ThreadedTCPConnectedClient.h"

class TCPServer : public SocketBase<SOCK_STREAM>
{
    public: /* Type Definitions */

    public: /* Member Functions */
        TCPServer( void );
        ~TCPServer( void );

    public: /* Members */
        void
        SetAddress( IPAddress & address );

        void
        SetPort( const IPAddress::TPort port );

        bool
        Listen( void );

        ThreadedTCPConnectedClient*
        Accept( void );

    private:
        typedef SocketBase<SOCK_STREAM> TBaseType;
};

#endif /* TCPSERVER_H_ */
