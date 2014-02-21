/**
 * @file Semaphore.cpp
 * Implements the Semaphore class under Windows 32/64bit and POSIX enables OSes.
 *  @date Jan 20, 2014
 *  @author Alper Sinan Akyurek
 */

#include "Semaphore.h"

#if defined( _WIN32 ) || defined( _WIN64 )

Semaphore::Semaphore( const TValue value )
{
    this->m_handle = CreateSemaphore( NULL, value, 32767, NULL );
}

Semaphore::Semaphore( const TValue value, const TName name )
{
    this->m_handle = CreateSemaphore( NULL, value, 32767, name );
}

Semaphore::~Semaphore( void )
{
    CloseHandle( this->m_handle );
}

Semaphore::Semaphore( const Semaphore & copy ) : m_handle( copy.m_handle )
{
}

bool
Semaphore::TakeSemaphore( void )
{
    DWORD result = WaitForSingleObject( this->m_handle, INFINITE )
    if ( result == WAIT_TIMEOUT )
    {
        return ( false );
    }
    return ( true );
}

bool
Semaphore::TryToTakeSemaphore(void)
{
    DWORD result = WaitForSingleObject( this->m_handle, 0 )
    if ( result == WAIT_TIMEOUT )
    {
        return ( false );
    }
    return ( true );
}

void
Semaphore::ReleaseSemaphore( void )
{
    ReleaseSemaphore( this->m_handle, 1, NULL );
}

#else

unsigned int Semaphore::nameCounter = 0;

Semaphore::Semaphore( const TValue value )
{
    char semaphoreName[32];
    sprintf( semaphoreName, "UnnamedSemaphore%d", nameCounter );
    this->OpenSemaphore( value, semaphoreName );
}

Semaphore::Semaphore( const TValue value, TName name )
{
    this->OpenSemaphore( value, name );
}

Semaphore::~Semaphore( void )
{
    sem_close( this->m_handle );
}

Semaphore::Semaphore( const Semaphore & copy ) : m_handle( copy.m_handle )
{
}

void
Semaphore::OpenSemaphore( const TValue value, const TName name )
{
    THandle* returnValue = sem_open( name, O_CREAT, S_IRWXU | S_IRWXG, value );
    if ( returnValue == SEM_FAILED )
    {
        std::cerr << "Semaphore create failed! Error: " << errno << std::endl;
        if ( errno == EACCES )
        {
            std::cerr << "WARNING: Unlinking Semaphore and recreating!" << std::endl;
            sem_unlink( name );
            this->OpenSemaphore( value, name );
        }
    }
    this->m_handle = returnValue;
}

bool
Semaphore::TakeSemaphore( void )
{
    if ( sem_wait( this->m_handle ) == 0 )
    {
        return ( false );
    }
    return ( true );
}

bool
Semaphore::TryToTakeSemaphore(void)
{
    if ( sem_trywait( this->m_handle ) == 0 )
    {
        return ( false );
    }
    return ( true );
}

void
Semaphore::ReleaseSemaphore( void )
{
    sem_post( this->m_handle );
}

#endif
