/*
 * ThreadedTCPConnectedClient.h
 *
 *  Created on: Jan 22, 2014
 *      Author: asakyurek
 */

#ifndef THREADEDTCPCONNECTEDCLIENT_H_
#define THREADEDTCPCONNECTEDCLIENT_H_

#include "TCPConnectedClient.h"
#include "ThreadBase.h"
#include "Semaphore.h"

class ThreadedTCPConnectedClient : public TCPConnectedClient, private ThreadBase
{
    public:
        typedef void ( *TNotification )( ThreadedTCPConnectedClient *, TBuffer data, const TNumberOfBytes size );

    private:
        void* ExecutionBody( void* input );

    private:
        TNotification m_notification;
        bool m_started;
        Semaphore m_allowingSemaphore;

    public:
        //ThreadedTCPConnectedClient( void );
        ThreadedTCPConnectedClient( const TSocketId socketId, IPAddress & clientAddress );
        ThreadedTCPConnectedClient( const TCPConnectedClient & client );
        virtual ~ThreadedTCPConnectedClient( void ) throw();

        void
        SetNotificationCallback( TNotification notification )
        {
            this->m_notification = notification;
            if ( this->m_notification != NULL )
            {
                this->m_started = true;
                this->m_allowingSemaphore.ReleaseSemaphore();
            }
            else
            {
                this->m_allowingSemaphore.TakeSemaphore();
            }
        }
};

#endif /* THREADEDTCPCONNECTEDCLIENT_H_ */
