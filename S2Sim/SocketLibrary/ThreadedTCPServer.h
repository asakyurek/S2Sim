/**
 * @file ThreadedTCPServer.h
 * Defines the ThreadedTCPServer class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef THREADEDTCPSERVER_H_
#define THREADEDTCPSERVER_H_

#include <thread>
#include "TCPServer.h"
#include "ThreadedTCPConnectedClient.h"
#include "LogPrint.h"

/**
 *  Defines the threaded version of TCPServer that accepts clients in another thread in the background. When a connection is established, the user will be notified through a callback function.
 */
class ThreadedTCPServer : public TCPServer
{
    public:
    /**
     *  Defines the type of the callback notification function. The callback should only have a single input. The function will be provided the pointer of its caller.
     */
        typedef void ( *TNotification )( ThreadedTCPConnectedClient * );

    private:
    /**
     *  The execution body of the thread. The thread will start accepting connections.
     */
        void ExecutionBody( void );

    private:
    /**
     *  Thread object.
     */
        std::thread m_thread;
    
    /**
     *  Callback function.
     */
        TNotification m_notification;
    
    /**
     *  Indicates whether the thread is running.
     */
        bool m_started;

    public:
    /**
     *  Initializes the variables.
     */
        ThreadedTCPServer( void );
    
    /**
     *  Stops the execution of the thread.
     */
        ~ThreadedTCPServer( void ) throw();

    /**
     *  Sets the notification callback. If the notification callback function is legit, the thread is started. This function can be set only once. Adding a guard has currently no meaning but overhead.
     *
     *  @param notification Callback to the notification function.
     */
        void
        SetNotificationCallback( TNotification notification );
};

#endif /* THREADEDTCPSERVER_H_ */
