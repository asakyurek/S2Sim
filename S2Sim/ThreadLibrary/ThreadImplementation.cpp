/*
 * ThreadImplementation.cpp
 *
 *  Created on: Dec 21, 2013
 *      Author: asakyurek
 */

#include "ThreadBase.h"

#if defined( __APPLE__ ) || defined ( __linux__ )

void*
PosixThreadCover( void* threadInput )
{
    ThreadBase::InputStructure* input = ( ThreadBase::InputStructure* )threadInput;
    ThreadBase* classPointer = input->m_classPointer;
    void* mainInput = input->m_mainInput;
    delete input;
    return ( classPointer->ExecutionBody( mainInput ) );
}

#endif

#if defined( _WIN32 ) || defined( _WIN64 )

DWORD WINAPI
WindowsThreadCover( LPVOID )
{
    ThreadBase::InputStructure* input = ( ThreadBase::InputStructure* )threadInput;
    ThreadBase* classPointer = input->m_classPointer;
    void* mainInput = input->m_mainInput;
    delete input;
    return ( ( DWORD )classPointer->ExecutionBody( mainInput ) );
}

#endif
