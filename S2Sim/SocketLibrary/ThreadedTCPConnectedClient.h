/**
 * @file ThreadedTCPConnectedClient.h
 * Defeines the ThreadedTCPConnectedClient class.
 *  @date Jan 22, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef THREADEDTCPCONNECTEDCLIENT_H_
#define THREADEDTCPCONNECTEDCLIENT_H_

#include "TCPConnectedClient.h"
#include "ThreadBase.h"
#include <mutex>

/**
 *  Manages the connection to an accepted client on the server side and receives data in a separate thread in the background.
 */
class ThreadedTCPConnectedClient : public TCPConnectedClient, private ThreadBase
{
    public:
    /**
     *  Defines the callback type. The function should have three inputs:
     - ThreadedTCPConnectedClient*: The function will be notified who the sender is.
     - TBuffer: The function will be provided the data received in a buffer on the stack. The function is responsible of copying the data or it will be overwritten.
     - TNumberOfBytes: The function will be provided how many bytes have been actually received.
     */
        typedef void ( *TNotification )( ThreadedTCPConnectedClient *, TBuffer data, const TNumberOfBytes size );

    private:
    /**
     *  The execution body of the thread. The thread waits for a semaphore, which is released only once when the notification callback is set. If the semaphore is taken, the thread will start receiving data from the server.
     *
     *  @param input Not used.
     *
     *  @return Not used. NULL.
     */
        void* ExecutionBody( void* input );

    private:
        /**
         *  Notification callback to be called.
         */
        TNotification m_notification;
        
        /**
         *  Boolean variable to stop the thread.
         */
        bool m_started;
        
        /**
         *  This mutex is unlocked only if there is a legitimate callback function.
         */
        std::mutex m_allowingMutex;

    public:
    /**
     *  Initializes the class with a ready socket and address, and starts the reception thread.
        @param socketId Handle to the socket.
        @param clientAddress IPAddress of the connected client.
     */
        ThreadedTCPConnectedClient( const TSocketId socketId, IPAddress & clientAddress );
    
    /**
     *  Copies the TCP connection information and starts a new thread.
     */
        ThreadedTCPConnectedClient( const TCPConnectedClient & client );
    
    /**
     *  Stops the execution of the thread.
     */
        ~ThreadedTCPConnectedClient( void ) throw();

    /**
     *  Sets the notification callback function. If the semaphore was already released, it is taken back. If the new callback is legit, it is released again.
     *
     *  @param notification Notification callback function.
     */
        void
        SetNotificationCallback( TNotification notification )
        {
            if ( this->m_notification != NULL )
            {
                this->m_allowingMutex.lock();
            }
            this->m_notification = notification;
            if ( this->m_notification != NULL )
            {
                this->m_allowingMutex.unlock();
            }
        }
};

#endif /* THREADEDTCPCONNECTEDCLIENT_H_ */
