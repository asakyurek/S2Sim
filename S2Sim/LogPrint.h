/**
 * @file LogPrint.h
 * Defines logging related functions.
 *  @date Oct 28, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef LOGPRINT_H_
#define LOGPRINT_H_

#include <iostream>
#include <mutex>
#include <map>
#include <thread>

class LogManager
{
    public:
    /**
     *  Logging level on how detailed the logs should be.
     *  - >3: All logs
     *  - 2: Warnings and errors
     *  - 1: Errors only
     *  - 0: No logging
     */
        enum class LogLevel
        {
            NoLogging = 0,
            ErrorOnly = 1,
            Warnings = 2,
            Logs = 3
        };
    
    /**
     *  Type indicating whether a logging function is being processed.
     */
        typedef bool TEnterState;
    
    /**
     *  Type indicating whether to print the function starts and ends.
     */
        typedef bool TFunctionPrint;
    
    /**
     *  Level of the stack. Increased at each function start and decreased at the end.
     */
        typedef unsigned int TCallLevel;
    
    /**
     *  Number of dots per stack level. For visual purposes only.
     */
        typedef unsigned int TDotLevel;
    
    /**
     *  Call level map per thread.
     */
        typedef std::map<std::thread::id, TCallLevel> TThreadMap;
    
    /**
     *  Alive map per thread.
     */
        typedef std::map<std::thread::id, bool> TThreadAliveMap;
    
    /**
     *  Type for the recursive mutex for log protection.
     */
        typedef std::recursive_mutex TMutex;
    
    private:
    /**
     *  Log detail level.
     */
        LogLevel m_logLevel;
    
    /**
     *  Indicates whether the log function is being used.
     */
        TEnterState m_logEntered;
    
    /**
     *  Indicates whether the warning function is being used.
     */
        TEnterState m_warningEntered;
    
    /**
     *  Indicates whether the error function is being used.
     */
        TEnterState m_errorEntered;
    
    /**
     *  Indicates whether the function name should be printed.
     */
        TFunctionPrint m_functionPrint;
    
    /**
     *  Number of dots to be printed per stack level.
     */
        TDotLevel m_dotLevel;
    
    /**
     *  Maps the thread id's to the stack level of the threads.
     */
        TThreadMap m_threadMap;
    
    /**
     *  Maps the thread id's to the alive or dead status of the threads.
     */
        TThreadAliveMap m_threadAliveMap;
    
    /**
     *  Mutex for log and output stream protection.
     */
        TMutex m_logMutex;
    
    private:
    /**
     *  Initializes parameters with default parameters.
     */
        LogManager( void );
    
    /**
     *  Reads the stack call level from the map.
     *
     *  @return Stack call level of the current thread.
     */
        TCallLevel GetCallLevel( void );
    
    public:
    /**
     *  Implements the singleton pattern.
     *
     *  @return The only instance of the LogManager class.
     */
        static LogManager& GetLogManager( void )
        {
            static LogManager logManager;
            return ( logManager );
        }
    
    /**
     *  Sets a new log level.
     *
     *  @param newLevel New log detail level.
     */
        void SetLogLevel( const LogLevel newLevel )
        {
            this->m_logLevel = newLevel;
        }
    
    /**
     *  Returns whether the current level is enough for logging.
     *
     *  @return Logging is permitted or not.
     */
        bool isLoggable( void ) const
        {
            return ( this->m_logLevel >= LogLevel::Logs );
        }
    
    /**
     *  Returns whether the current level is enough for warnings.
     *
     *  @return Warning is permitted or not.
     */
        bool isWarnable( void ) const
        {
            return ( this->m_logLevel >= LogLevel::Warnings );
        }
    
    /**
     *  Returns whether the current level is enough for errors.
     *
     *  @return Error is permitted or not.
     */
        bool isErrorable( void ) const
        {
            return ( this->m_logLevel >= LogLevel::ErrorOnly );
        }
    
    /**
     *  Used by logging function to protect the logs.
     */
        void EnterLog( void )
        {
            this->m_logEntered = true;
        }
    
    /**
     *  Used by logging function to complete protection.
     */
        void ExitLog( void )
        {
            this->m_logEntered = false;
        }
    
    /**
     *  Used by warning function to protect the warnings.
     */
        void EnterWarning( void )
        {
            this->m_warningEntered = true;
        }
    
    /**
     *  Used by warning function to complete protection.
     */
        void ExitWarning( void )
        {
            this->m_warningEntered = false;
        }
    
    /**
     *  Used by error function to protect the errors.
     */
        void EnterError( void )
        {
            this->m_errorEntered = true;
        }
    
    /**
     *  Used by error function to complete protection.
     */
        void ExitError( void )
        {
            this->m_errorEntered = false;
        }
    
    /**
     *  Indicates whether the log function is currently run.
     *
     *  @return In the log function or not.
     */
        TEnterState InLog( void ) const
        {
            return ( this->m_logEntered );
        }
    
    /**
     *  Indicates whether the warning function is currently run.
     *
     *  @return In the warning function or not.
     */
        TEnterState InWarning( void ) const
        {
            return ( this->m_warningEntered );
        }
    
    /**
     *  Indicates whether the error function is currently run.
     *
     *  @return In the error function or not.
     */
        TEnterState InError( void ) const
        {
            return ( this->m_errorEntered );
        }

    /**
     *  Sets the number of dots to be printed per call level.
     *
     *  @param dotLevel Number of dots per level.
     */
        void SetDotLevel( const TDotLevel dotLevel )
        {
            this->m_dotLevel = dotLevel;
        }
    
    /**
     *  Enables function name printing.
     */
        void EnableFunctionLogs( void )
        {
            this->m_functionPrint = true;
        }
    
    /**
     *  Disables function name printing.
     */
        void DisableFunctionLogs( void )
        {
            this->m_functionPrint = false;
        }
    
    /**
     *  Returns whether the function name will be printed.
     *
     *  @return Returns whether the function name is printed.
     */
        TFunctionPrint GetFunctionPrint( void )
        {
            return ( this->m_functionPrint );
        }
    
    /**
     *  Returns the mutex lock protecting the output stream.
     *
     *  @return Returns the reference of the mutex.
     */
        TMutex& GetMutex( void )
        {
            return ( this->m_logMutex );
        }
    
    /**
     *  Increases the call level of the current thread signalled by function enterance.
     */
        void IncreaseCallLevel( void );
    
    /**
     *  Decreases the call level of the current thread signalled by function exit.
     */
        void DecreaseCallLevel( void );
    
    /**
     *  Returns the indentation string using call level and dot numbers.
     *
     *  @return String containing indentation dots.
     */
        std::string GetIndentString( void );
    
    /**
     *  A thread uses this function as its last words.
     */
        void ThreadIsDying( void );
};

/**
 *  Macro to indicate the start of a function for verbose debugging.
 */
#define LOG_FUNCTION_START() \
LogManager::GetLogManager().GetFunctionPrint() ? LogPrint( "-START-", __PRETTY_FUNCTION__ ) : LogPrint( "-START-" ); \
LogManager::GetLogManager().IncreaseCallLevel();

/**
 *  Macro to indicate the end of a function for verbose debugging.
 */
#define LOG_FUNCTION_END() \
LogManager::GetLogManager().DecreaseCallLevel(); \
LogManager::GetLogManager().GetFunctionPrint() ? LogPrint( "-END-", __PRETTY_FUNCTION__ ) : LogPrint( "-END-" );

/**
 *  Log function with a single parameter. This function is either called by user with a single argument or as the last specialization of the variadic template.
 *
 *  @param input1 Information to be displayed.
 *
 *  @tparam I1 Type of the information to be displayed.
 */
template <typename I1>
void LogPrint( I1 input1 )
{
    std::lock_guard<LogManager::TMutex> lockGuard(LogManager::GetLogManager().GetMutex());
    if ( LogManager::GetLogManager().isLoggable() )
    {
        if ( LogManager::GetLogManager().InLog() )
        {
            std::cout << input1 << std::endl;
        }
        else
        {
            std::cout << LogManager::GetLogManager().GetIndentString() << input1 << std::endl;
        }
    }
}

/**
 *  General log function with infinite parameters. This function takes the first parameter, prints it and sends the rest to a new specialization until a single parameter is left.
 *
 *  @param input1 First of the input list to be displayed.
 *  @param inputs Rest of the inputs for the next specialization.
 *
 *  @tparam I1 Type of the first input.
 *  @tparam OtherInputs List of types for the rest.
 */
template<typename I1, typename... OtherInputs>
void LogPrint( I1 input1, OtherInputs... otherInputs )
{
    std::lock_guard<LogManager::TMutex> lockGuard(LogManager::GetLogManager().GetMutex());
    if ( LogManager::GetLogManager().isLoggable() )
    {
        if ( LogManager::GetLogManager().InLog() )
        {
            std::cout << input1;
            LogPrint( otherInputs... );
        }
        else
        {
            LogManager::GetLogManager().EnterLog();
            std::cout << LogManager::GetLogManager().GetIndentString() << input1;
            LogPrint( otherInputs... );
            LogManager::GetLogManager().ExitLog();
        }
    }
}

/**
 *  Warning function in most general form. This function is only called when no input is left in the main function to end the warning.
 *
 *  @param otherInputs Empty.
 *
 *  @tparam OtherInputs Empty type.
 */
template<typename... OtherInputs>
void WarningPrint( OtherInputs... otherInputs )
{
    std::lock_guard<LogManager::TMutex> lockGuard(LogManager::GetLogManager().GetMutex());
    if ( LogManager::GetLogManager().isWarnable() )
    {
        std::cout << std::endl << "__________WARNING__________" << std::endl;
    }
}

/**
 *  General warning function with infinite parameters. This function takes the first parameter, prints it and sends the rest to a new specialization until a single parameter is left.
 *
 *  @param input1 First of the input list to be displayed.
 *  @param inputs Rest of the inputs for the next specialization.
 *
 *  @tparam I1 Type of the first input.
 *  @tparam OtherInputs List of types for the rest.
 */
template<typename I1, typename... OtherInputs>
void WarningPrint( I1 input1, OtherInputs... otherInputs )
{
    std::lock_guard<LogManager::TMutex> lockGuard(LogManager::GetLogManager().GetMutex());
    if ( LogManager::GetLogManager().isWarnable() )
    {
        if ( LogManager::GetLogManager().InWarning() )
        {
            std::cout << input1;
            WarningPrint( otherInputs... );
        }
        else
        {
            std::cout << "__________WARNING__________" << std::endl;
            LogManager::GetLogManager().EnterWarning();
            std::cout << LogManager::GetLogManager().GetIndentString() << input1;
            WarningPrint( otherInputs... );
            LogManager::GetLogManager().ExitWarning();
        }
    }
}

/**
 *  Error function in most general form. This function is only called when no input is left in the main function to end the warning.
 *
 *  @param otherInputs Empty.
 *
 *  @tparam OtherInputs Empty type.
 */
template<typename... OtherInputs>
void ErrorPrint( OtherInputs... otherInputs )
{
    std::lock_guard<LogManager::TMutex> lockGuard(LogManager::GetLogManager().GetMutex());
    if ( LogManager::GetLogManager().isErrorable() )
    {
        std::cerr << std::endl << "__________ERROR__________" << std::endl;
    }
}

/**
 *  General error function with infinite parameters. This function takes the first parameter, prints it and sends the rest to a new specialization until no parameter is left.
 *
 *  @param input1 First of the input list to be displayed.
 *  @param inputs Rest of the inputs for the next specialization.
 *
 *  @tparam I1 Type of the first input.
 *  @tparam OtherInputs List of types for the rest.
 */
template<typename I1, typename... OtherInputs>
void ErrorPrint( I1 input1, OtherInputs... otherInputs )
{
    std::lock_guard<LogManager::TMutex> lockGuard(LogManager::GetLogManager().GetMutex());
    if ( LogManager::GetLogManager().isErrorable() )
    {
        if ( LogManager::GetLogManager().InError() )
        {
            std::cerr << input1;
            ErrorPrint( otherInputs... );
        }
        else
        {
            std::cerr << "__________ERROR__________" << std::endl;
            LogManager::GetLogManager().EnterError();
            std::cerr << LogManager::GetLogManager().GetIndentString() << input1;
            ErrorPrint( otherInputs... );
            LogManager::GetLogManager().ExitError();
        }
    }
}

#endif /* LOGPRINT_H_ */
