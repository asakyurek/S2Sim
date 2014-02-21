/**
 * @file Semaphore.h
 * Defines the Semaphore class under Windows 32/64bit and POSIX enables OSes.
 *  @date Jan 20, 2014
 *  @author: Alper Sinan Akyurek
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <iostream>

/**
 *  Windows Implementation
 */
#if ( defined( _WIN32 ) || defined( _WIN64 ) )

#include <windows.h>

/**
 *  This class is an abstraction on the Windows semaphore utilities.
 */
class Semaphore
{
    private:
    /**
     *  Defines the semaphore handle type.
     */
        typedef HANDLE THandle;

    public:
    /**
     *  Defines the name of the semaphore.
     */
        typedef char* TName;
    
    /**
     *  Defines the value of the semaphore.
     */
        typedef unsigned int TValue;

    private:
    /**
     *  Handle used within OS utilities.
     */
        THandle m_handle;

    public:
    /**
     *  Constructs the semaphore with an initial value and without a name. To maintain compatibility with POSIX, the maximum semaphore value is 32767.
     
        @param value Initial value of the semaphore.
     */
        Semaphore( const TValue value );
    
    /**
     *  Constructs the semaphore with an initial value and a name.
     
        @param value Initial value of the semaphore.
        @param name Name of the semaphore.
     */
        Semaphore( const TValue value, const TName name );
    
    /**
     *  Closes the handle for the semaphore.
     */
        ~Semaphore( void );

    /**
     *  Copy constructor copies the contents without calling the OS utilities.
     */
        Semaphore( const Semaphore & copy );

    /**
     *  Takes the semaphore or waits until it is successful.
     *
     *  @return Indication regarding the success of the operation.
     */
        bool
        TakeSemaphore( void );

    /**
     *  Tries to take the semaphore. If it is not available, the method does not block the call and returns false.
     *
     *  @return Indication regarding the success of the operation.
     */
        bool
        TryToTakeSemaphore( void );

    /**
     *  This method releases a single value for the semaphore.
     */
        void
        ReleaseSemaphore( void );
};

/**
 *  POSIX Implementation
 */
#elif ( defined( __LINUX__ ) || defined( __APPLE__ ) )

#include <semaphore.h>

/**
 *  This class is an abstraction to POSIX semaphore utilities.
 */
class Semaphore
{
    private:
    /**
     *  Semaphore type.
     */
        typedef int sem_t;
    
    /**
     *  Redefinition of the semaphore type for having the same name under all implementations.
     */
        typedef sem_t THandle;

    public:
    /**
     *  Name of the semaphore.
     */
        typedef char* TName;
    
    /**
     *  Value of the semaphore.
     */
        typedef unsigned int TValue;

    private:
    /**
     *  Handle to the semaphore used in OS utilities.
     */
        THandle* m_handle;
    
    /**
     *  Since nameless semaphores are not supported under MACOS, this counter is used to create a distinct name for the semaphore.
     */
        static unsigned int nameCounter;

    private:
    /**
     *  Opens a new semaphore with the given value and name.
     *
     *  @param value Initial value of the semaphore.
     *  @param name  Name of the semaphore.
     */
        void
        OpenSemaphore( const TValue value, const TName name );

    public:
    /**
     *  Creates a nameless semaphore by assigning a unique name.
     
        @param value Initial value of the semaphore.
     */
        Semaphore( const TValue value );
    
    /**
     *  Creates a named semaphore.
     
        @param value Initial value of the semaphore.
        @param name Name of the semaphore.
     */
        Semaphore( const TValue value, const TName name );
    
    /**
     *  Closes the handle to the semaphore.
     */
        ~Semaphore( void );

    /**
     *  Copies the contents of the semaphore without creating a new one.
     */
        Semaphore( const Semaphore & copy );

    /**
     *  Takes the semaphore or waits until it is successfull.
     *
     *  @return Indication regarding the success of the operation.
     */
        bool
        TakeSemaphore( void );

    /**
     *  Tries to take the semaphore but does not block the call if the semaphore is not available.
     *
     *  @return Indication regarding the success of the operation.
     */
        bool
        TryToTakeSemaphore( void );

    /**
     *  This method releases a single value of the semaphore.
     */
        void
        ReleaseSemaphore( void );
};

#endif

#endif /* SEMAPHORE_H_ */
