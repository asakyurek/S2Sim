/*
 * TCPClient.h
 *
 *  Created on: Jan 21, 2014
 *      Author: asakyurek
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "SocketBase.h"
#include "IPAddress.h"

class TCPClient : public SocketBase<SOCK_STREAM>
{
    public: /* Type Definitions */

    public: /* Member Functions */
        TCPClient( void );
        ~TCPClient( void );

    public: /* Members */
        bool
        Connect( IPAddress & address );

        TNumberOfBytes
        SendData( const TBuffer buffer, const TNumberOfBytes length );

        TNumberOfBytes
        ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength );

    private:
        typedef SocketBase<SOCK_STREAM> TBaseType;
};

#endif /* TCPCLIENT_H_ */
