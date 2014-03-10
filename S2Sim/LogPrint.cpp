/**
 * @file LogPrint.cpp
 * Defines logging related parameters.
 *  @date Oct 28, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "LogPrint.h"

void LogManager::IncreaseCallLevel( void )
{
    if ( this->m_threadMap.find( std::this_thread::get_id() ) == this->m_threadMap.end() )
    {
        this->m_threadMap[std::this_thread::get_id()] = 1;
    }
    else
    {
        ++( this->m_threadMap[std::this_thread::get_id()] );
    }
}

void LogManager::DecreaseCallLevel( void )
{
    --( this->m_threadMap[std::this_thread::get_id()] );
}

LogManager::TCallLevel LogManager::GetCallLevel( void )
{
    if ( this->m_threadMap.find( std::this_thread::get_id() ) == this->m_threadMap.end() )
    {
        this->m_threadMap[std::this_thread::get_id()] = 0;
    }
    return ( this->m_threadMap[std::this_thread::get_id()] );
}

std::string LogManager::GetIndentString( void )
{
    std::string indentString( this->m_dotLevel * this->GetCallLevel(), '.' );
    return ( indentString );
}

LogManager::LogManager( void ) : m_logLevel( LogLevel::Logs ),
    m_logEntered( false ),
    m_warningEntered( false ),
    m_errorEntered( false ),
    m_functionPrint( true ),
    m_dotLevel( 4 )
{
}
