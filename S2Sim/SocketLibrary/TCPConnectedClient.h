/*
 * TCPConnectedClient.h
 *
 *  Created on: Jan 21, 2014
 *      Author: asakyurek
 */

#ifndef TCPCONNECTEDCLIENT_H_
#define TCPCONNECTEDCLIENT_H_

#include "SocketBase.h"
#include "IPAddress.h"

class TCPConnectedClient : public SocketBase<SOCK_STREAM>
{
    public: /* Type Definitions */

    public: /* Member Functions */
        TCPConnectedClient( const TSocketId socketId, IPAddress & clientAddress );
        TCPConnectedClient( const TCPConnectedClient & copy );
        ~TCPConnectedClient( void );

    public: /* Members */
        TNumberOfBytes
        SendData( const TBuffer buffer, const TNumberOfBytes length );

        TNumberOfBytes
        ReceiveData( TBuffer buffer, const TNumberOfBytes receptionLength );

        IPAddress
        GetClientAddress( void ) const;

    private: /* Type Definitions */
        typedef SocketBase<SOCK_STREAM> TBaseType;

    private: /* Members */
        IPAddress m_clientAddress;
};

#endif /* TCPCONNECTEDCLIENT_H_ */
