/**
 * @file PosixThreadBase.h
 * Defines the ThreadBase class.
 *  @date Dec 21, 2013
 *  @authoruthor: Alper Sinan Akyurek
 */

#ifndef POSIXTHREADBASE_H_
#define POSIXTHREADBASE_H_

#include <pthread.h>

#include "LogPrint.h"

class ThreadBase;

/**
 *  Wrapper function called by the operating system a the thread body. It takes the actual class pointer and the input into the execution function from its input and executes the "actual thread body" with the desired input. This allows us to use the OS thread utilities to run a C++ class method, rather than a plain C function.
 *
 *  @param void* Special input structure containing the class address and the real input to the execution body.
 *
 *  @return Returns whatever the class method returns.
 */
void*
PosixThreadCover( void* );

/**
 *  Provides a base class that can be inherited from to gain threading capabilities.
 
    Any class that wants to have threading capability can inherit from this class and override its purely virtual method ThreadBase::ExecutionBody. This overriden method will be the threads main body.
 */
class ThreadBase
{
    /**
     *  Friend wrapper function that is actually called by the OS as the thread body. Uses the input to gain access to the class and the real thread input.
     *
     *  @param void* Special input structure containing the class address and the real input to the execution body.
     */
    friend void* PosixThreadCover( void* );
    
    private:
    /**
     *  Type for the thread handle used for the OS utilities.
     */
        typedef pthread_t TThreadId;
    
    /**
     *  Type for the thread attributes.
     */
        typedef pthread_attr_t TThreadAttribute;
    
    /**
     *  Special Input structure sent to the wrapper function: PosixThreadCover(). This structure is used to recover the class pointer and the actual input to the thread body.
     */
        struct InputStructure
        {
            /**
             *  Actual input to the thread body. This is stored in this structure because the OS only allows a single input to the thread body, which is structure itself.
             */
            void* m_mainInput;
            
            /**
             *  Pointer to the ThreadBase class to gain access to the real thread body.
             */
            ThreadBase* m_classPointer;
            
            /**
             *  Simple constructor to assign the values.
             *
             *  @param mainInput    Actual input to the thread body.
             *  @param classPointer Pointer to the ThreadBase class.
             */
            InputStructure( void* mainInput, ThreadBase* classPointer ) : m_mainInput( mainInput ),
                                                                          m_classPointer( classPointer )
            {
            }
        };

    public:
    /**
     *  Defines the Stack Size type for the thread.
     */
        typedef size_t TStackSize;
    
    /**
     *  Defines a type indicating whether the thread is started.
     */
        typedef bool TStarted;
    
    /**
     *  Defines values for the TStarted type.
     */
        enum StartedValues
        {
            isStarted = ( TStarted )true, /**< Indicates that the thread is started. */
            isNotStarted = ( TStarted )false /**< Indicates that the thread is not started yet. */
        };

    private:
    /**
     *  Indicates whether the thread is started to execute.
     */
        TStarted m_started;
    
    /**
     *  Variable holding the handle to the thread.
     */
        TThreadId m_threadId;
    
    /**
     *  Variable holding the attributes of the thread.
     */
        TThreadAttribute m_threadAttribute;

    private:
    /**
     *  Function called by the wrapper function PosixThreadCover().The method executes the overridden execution body method with the actual input and returns the output of the execution body through the OS.
     *
     *  @param input Actual input to the thread body.
     *
     *  @return Returns the exact thing that the thread body returns.
     */
        void* ExecuteThread( void* input )
        {
            pthread_exit( this->ExecutionBody( input ) );
        }

    /**
     *  Purely virtual method to be overridden by the inheriting class to implement its own thread body.
     *
     *  @param input Input to the thread body.
     
        @return Return is optional.
     */
        virtual void* ExecutionBody( void* input ) = 0;

    public:
    /**
     *  Initializes values but does not create a thread.
     */
        ThreadBase( void ) : m_started( isNotStarted ),
                             m_threadId( 0 )
        {
            pthread_attr_init( &this->m_threadAttribute );
        }

    /**
     *  Initializes values and starts the thread with the given input.
     *
     *  @param input Input to the thread body.
     */
        ThreadBase( void* input ) : m_started( isNotStarted ),
                                    m_threadId( 0 )
        {
            pthread_attr_init( &this->m_threadAttribute );
            this->StartThread( input );
        }

    /**
     *  Stops the thread and frees the allocates attribute.
     */
        virtual ~ThreadBase( void )
        {
            if ( this->m_started )
            {
                pthread_cancel( this->m_threadId );
            }
            pthread_attr_destroy( &this->m_threadAttribute );
        }

    /**
     *  Copies the contents of another thread.
     *
     *  @param copy Instance to be copied.
     */
        ThreadBase( const ThreadBase & copy ) : m_started( copy.m_started ),
                                                m_threadId( copy.m_threadId ),
                                                m_threadAttribute( copy.m_threadAttribute )
        {
        }

    /**
     *  Sets the stack size of the thread.
     *
     *  @param stackSize Desired stack size.
     */
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

    /**
     *  Gets the stack size of the thread.
     *
     *  @return Stack size of the thread.
     */
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

    /**
     *  Creates a thread and starts executing with the given input.
     *
     *  @param input Input to the thread body.
     */
        void
        StartThread( void* input )
        {
            InputStructure* threadInput = new InputStructure( input, this );
            pthread_create( &this->m_threadId, &this->m_threadAttribute, &PosixThreadCover, threadInput );
        }
};

#endif /* POSIXTHREADBASE_H_ */
