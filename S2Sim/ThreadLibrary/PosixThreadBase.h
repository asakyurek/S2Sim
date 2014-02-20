/*
 * PosixThreadBase.h
 *
 *  Created on: Dec 21, 2013
 *      Author: asakyurek
 */

#ifndef POSIXTHREADBASE_H_
#define POSIXTHREADBASE_H_

#include <pthread.h>

class ThreadBase;

void*
PosixThreadCover( void* );

class ThreadBase
{
    friend void* PosixThreadCover( void* );
    private:
        typedef pthread_t TThreadId;
        typedef pthread_attr_t TThreadAttribute;
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
        TThreadAttribute m_threadAttribute;

    private:
        void* ExecuteThread( void* input )
        {
            pthread_exit( this->ExecutionBody( input ) );
        }

        virtual void* ExecutionBody( void* ) = 0;

    public:
        ThreadBase( void ) : m_started( isNotStarted ),
                             m_threadId( 0 )
        {
            pthread_attr_init( &this->m_threadAttribute );
        }

        ThreadBase( void* input ) : m_started( isNotStarted ),
                                    m_threadId( 0 )
        {
            pthread_attr_init( &this->m_threadAttribute );
            this->StartThread( input );
        }

        virtual ~ThreadBase( void )
        {
            if ( !this->m_started )
            {
                pthread_attr_destroy( &this->m_threadAttribute );
            }
        }

        ThreadBase( const ThreadBase & copy ) : m_started( copy.m_started ),
                                                m_threadId( copy.m_threadId ),
                                                m_threadAttribute( copy.m_threadAttribute )
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
            pthread_attr_setstacksize( &this->m_threadAttribute, stackSize );
        }

        TStackSize
        GetStackSize( void ) const
        {
            if ( this->m_started )
            {
                class ERRORThreadStarted{};
                throw ( ERRORThreadStarted() );
            }
            TStackSize stackSize;
            pthread_attr_getstacksize( &this->m_threadAttribute, &stackSize );
            return ( stackSize );
        }

        void
        StartThread( void* input )
        {
            InputStructure* threadInput = new InputStructure( input, this );
            pthread_create( &this->m_threadId, &this->m_threadAttribute, &PosixThreadCover, threadInput );
        }
};

#endif /* POSIXTHREADBASE_H_ */
