/*
 * ThreadedTCPServer.h
 *
 *  Created on: Jan 22, 2014
 *      Author: asakyurek
 */

#ifndef THREADEDTCPSERVER_H_
#define THREADEDTCPSERVER_H_

#include "ThreadBase.h"
#include "TCPServer.h"
#include "ThreadedTCPConnectedClient.h"

class ThreadedTCPServer : public TCPServer, private ThreadBase
{
    public:
        typedef void ( *TNotification )( ThreadedTCPConnectedClient * );

    private:
        void* ExecutionBody( void* input );

    private:
        TNotification m_notification;
        bool m_started;

    public:
        ThreadedTCPServer( void );
        virtual ~ThreadedTCPServer( void ) throw();

        void
        SetNotificationCallback( TNotification notification )
        {
            this->m_notification = notification;
            if ( this->m_notification != NULL )
            {
                this->m_started = true;
                this->Listen();
                this->StartThread( NULL );
            }
        }
};

#endif /* THREADEDTCPSERVER_H_ */
