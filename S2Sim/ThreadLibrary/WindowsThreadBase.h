/*
 * WindowsThreadBase.h
 *
 *  Created on: Dec 21, 2013
 *      Author: asakyurek
 */

#ifndef WINDOWSTHREADBASE_H_
#define WINDOWSTHREADBASE_H_

#include <windows.h>

class ThreadBase;

DWORD WINAPI
WindowsThreadCover( LPVOID );

class ThreadBase
{
    friend DWORD WINAPI WindowsThreadCover( LPVOID );
    private:
        typedef DWORD TThreadId;
        typedef HANDLE TThreadHandle;
        struct InputStructure
        {
            void* m_mainInput;
            ThreadBase* m_classPointer;
            InputStructure( void* mainInput, ThreadBase* classPointer ) : m_mainInput( mainInput ),
                                                                          m_classPointer( classPointer )
            {
            }
        };

    public:
        typedef size_t TStackSize;
        typedef bool TStarted;
        enum StartedValues
        {
            isStarted = ( TStarted )true,
            isNotStarted = ( TStarted )false
        };

    private:
        TStarted m_started;
        TThreadId m_threadId;
        TThreadHandle m_threadHandle;
        TStackSize m_stackSize;

    private:
        void* ExecuteThread( void* input )
        {
            return( this->ExecutionBody( input ) );
        }

        virtual void* ExecutionBody( void* ) = 0;

    public:
        ThreadBase( void ) : m_started( isNotStarted ),
                             m_threadId( 0 ),
                             m_threadHandle( NULL ),
                             m_stackSize( 0 )
        {
        }

        virtual ~ThreadBase( void )
        {
            if ( this->m_started )
            {
                CloseHandle( this->m_threadHandle );
            }
        }

        ThreadBase( const ThreadBase & copy ) : m_started( copy.m_started ),
                                                m_threadId( copy.m_threadId ),
                                                m_threadHandle( copy.m_threadHandle ),
                                                m_stackSize( copy.m_stackSize )
        {
        }

        void
        SetStackSize( const TStackSize stackSize )
        {
            if ( this->m_started )
            {
                class ERRORThreadStarted{};
                throw ( ERRORThreadStarted() );
            }
            this->m_stackSize = stackSize;
        }

        TStackSize
        GetStackSize( void ) const
        {
            if ( this->m_started )
            {
                class ERRORThreadStarted{};
                throw ( ERRORThreadStarted() );
            }
            return ( this->m_stackSize );
        }

        void
        StartThread( void* input )
        {
            InputStructure* threadInput = new InputStructure( input, this );
            CreateThread( NULL, this->m_stackSize, &WindowsThreadCover, threadInput, 0, &this->m_threadId );
        }
};

#endif /* WINDOWSTHREADBASE_H_ */
