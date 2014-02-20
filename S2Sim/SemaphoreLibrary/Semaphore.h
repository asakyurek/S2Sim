/*
 * Semaphore.h
 *
 *  Created on: Jan 20, 2014
 *      Author: asakyurek
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <iostream>

#if ( defined( _WIN32 ) || defined( _WIN64 ) )

#include <windows.h>

class Semaphore
{
    private:
        typedef HANDLE THandle;

    public:
        typedef char* TName;
        typedef unsigned int TValue;

    private:
        THandle m_handle;

    public:
        Semaphore( const TValue value );
        Semaphore( const TValue value, const TName name );
        ~Semaphore( void );

        Semaphore( const Semaphore & copy );

        bool
        TakeSemaphore( void );

        bool
        TryToTakeSemaphore( void );

        void
        ReleaseSemaphore( void );
};

#elif ( defined( __LINUX__ ) || defined( __APPLE__ ) )

#include <semaphore.h>

class Semaphore
{
    private:
        typedef int sem_t;
        typedef sem_t THandle;

    public:
        typedef char* TName;
        typedef unsigned int TValue;

    private:
        THandle* m_handle;
        static unsigned int nameCounter;

    private:
        void
        OpenSemaphore( const TValue value, const TName name );

    public:
        Semaphore( const TValue value );
        Semaphore( const TValue value, const TName name );
        ~Semaphore( void );

        Semaphore( const Semaphore & copy );

        bool
        TakeSemaphore( void );

        bool
        TryToTakeSemaphore( void );

        void
        ReleaseSemaphore( void );
};

#endif

#endif /* SEMAPHORE_H_ */
