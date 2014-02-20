/*
 * ThreadedTCPClient.h
 *
 *  Created on: Jan 22, 2014
 *      Author: asakyurek
 */

#ifndef THREADEDTCPCLIENT_H_
#define THREADEDTCPCLIENT_H_

#include "TCPClient.h"
#include "ThreadBase.h"
#include "Semaphore.h"

class ThreadedTCPClient : public TCPClient, private ThreadBase
{
    public:
        typedef void ( *TNotification )( ThreadedTCPClient *, TBuffer data, const TNumberOfBytes size );

    private:
        void* ExecutionBody( void* input );

    private:
        TNotification m_notification;
        bool m_started;
        Semaphore m_allowingSemaphore;

    public:
        ThreadedTCPClient( void );
        ThreadedTCPClient( const TCPClient & client );
        ~ThreadedTCPClient( void ) throw();

        void
        SetNotificationCallback( TNotification notification )
        {
            this->m_notification = notification;
            if ( this->m_notification != NULL )
            {
                this->m_started = true;
                this->m_allowingSemaphore.ReleaseSemaphore();
            }
        }
};

#endif /* THREADEDTCPCLIENT_H_ */
