/**
 * @file LogPrint.h
 * Defines logging related functions.
 *  @date Oct 28, 2013
 *  @author: Alper Sinan Akyurek
 */

#ifndef LOGPRINT_H_
#define LOGPRINT_H_

#include <iostream>
#include "Semaphore.h"

/**
 *  This is a small hack to suppress compiler warning for default macro parameter initialization.
 */
#pragma GCC system_header

/**
 *  Logging level on how detailed the logs should be.
    - 1: All logs
    - 2: Warnings and errors
    - 3: Errors only
    - >3: No logging
 */
extern unsigned int logLevel;

/**
 *  Boolean to indicate whether the function name should be printed for each log.
 */
extern bool functionPrint;

/**
 *  Macro to indicate the start of a function for verbose debugging.
 */
#define LOG_FUNCTION_START() LogPrint( "-START-" );

/**
 *  Macro to indicate the end of a function for verbose debugging.
 */
#define LOG_FUNCTION_END() LogPrint( "-END-" );

/**
 *  8 parameter log message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5, typename I6, typename I7, typename I8>
void LogPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, I6 input6, I7 input7, I8 input8, const char* function = __PRETTY_FUNCTION__ )
{
    static Semaphore logSemaphore(1, "LogSemaphore");
    if ( logLevel >= 3 )
    {
        if ( functionPrint )
        {
            logSemaphore.TakeSemaphore();
            std::cout << "LOG " << function << std::endl;
            std::cout << "    : " << input1 << input2 << input3 << input4 << input5 << input6 << input7 << input8 << std::endl;
            logSemaphore.ReleaseSemaphore();
        }
        else
        {
            logSemaphore.TakeSemaphore();
            std::cout << "LOG    : " << input1 << input2 << input3 << input4 << input5 << input6 << input7 << input8 << std::endl;
            logSemaphore.ReleaseSemaphore();
        }
    }
}

/**
 *  7 parameter log message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5, typename  I6, typename I7>
void LogPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, I6 input6, I7 input7, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, input2, input3, input4, input5, input6, input7, "", function );
}

/**
 *  6 parameter log message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5, typename  I6>
void LogPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, I6 input6, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, input2, input3, input4, input5, input6, "", function );
}

/**
 *  5 parameter log message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5>
void LogPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, input2, input3, input4, input5, "", function );
}

/**
 *  4 parameter log message
 */
template <typename I1, typename I2, typename I3, typename I4>
void LogPrint( I1 input1, I2 input2, I3 input3, I4 input4, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, input2, input3, input4, "", function );
}

/**
 *  3 parameter log message
 */
template <typename I1, typename I2, typename I3>
void LogPrint( I1 input1, I2 input2, I3 input3, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, input2, input3, "", function );
}

/**
 *  2 parameter log message
 */
template <typename I1, typename I2>
void LogPrint( I1 input1, I2 input2, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, input2, "", function );
}

/**
 *  1 parameter log message
 */
template <typename I1>
void LogPrint( I1 input1, const char* function = __PRETTY_FUNCTION__ )
{
    LogPrint( input1, "", function );
}

/**
 *  6 parameter warning message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5, typename I6>
void WarningPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, I6 input6, const char* function = __PRETTY_FUNCTION__ )
{
    if ( logLevel >= 2 )
    {
        if ( functionPrint )
        {
            std::cout << "WARNING " << function << std::endl;
            std::cout << "    : " << input1 << input2 << input3 << input4 << input5 << input6 << std::endl;
        }
        else
        {
            std::cout << "WARNING    : " << input1 << input2 << input3 << input4 << input5 << input6 << std::endl;
        }
    }
}

/**
 *  5 parameter warning message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5>
void WarningPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, const char* function = __PRETTY_FUNCTION__ )
{
    WarningPrint( input1, input2, input3, input4, input5, "", function );
}

/**
 *  4 parameter warning message
 */
template <typename I1, typename I2, typename I3, typename I4>
void WarningPrint( I1 input1, I2 input2, I3 input3, I4 input4, const char* function = __PRETTY_FUNCTION__ )
{
    WarningPrint( input1, input2, input3, input4, "", function );
}

/**
 *  3 parameter warning message
 */
template <typename I1, typename I2, typename I3>
void WarningPrint( I1 input1, I2 input2, I3 input3, const char* function = __PRETTY_FUNCTION__ )
{
    WarningPrint( input1, input2, input3, "", function );
}

/**
 *  2 parameter warning message
 */
template <typename I1, typename I2>
void WarningPrint( I1 input1, I2 input2, const char* function = __PRETTY_FUNCTION__ )
{
    WarningPrint( input1, input2, "", function );
}

/**
 *  1 parameter warning message
 */
template <typename I1>
void WarningPrint( I1 input1, const char* function = __PRETTY_FUNCTION__ )
{
    WarningPrint( input1, "", function );
}

/**
 *  6 parameter error message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5, typename I6>
void ErrorPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, I6 input6, const char* function = __PRETTY_FUNCTION__ )
{
    if ( logLevel >= 1 )
    {
        if ( functionPrint )
        {
            std::cerr << "ERROR " << function << std::endl;
            std::cerr << "    : " << input1 << input2 << input3 << input4 << input5 << input6 << std::endl;
        }
        else
        {
            std::cerr << "ERROR    : " << input1 << input2 << input3 << input4 << input5 << input6 << std::endl;
        }
    }
}

/**
 *  5 parameter error message
 */
template <typename I1, typename I2, typename I3, typename I4, typename I5>
void ErrorPrint( I1 input1, I2 input2, I3 input3, I4 input4, I5 input5, const char* function = __PRETTY_FUNCTION__ )
{
    ErrorPrint( input1, input2, input3, input4, input5, "", function );
}

/**
 *  4 parameter error message
 */
template <typename I1, typename I2, typename I3, typename I4>
void ErrorPrint( I1 input1, I2 input2, I3 input3, I4 input4, const char* function = __PRETTY_FUNCTION__ )
{
    ErrorPrint( input1, input2, input3, input4, "", function );
}

/**
 *  3 parameter error message
 */
template <typename I1, typename I2, typename I3>
void ErrorPrint( I1 input1, I2 input2, I3 input3, const char* function = __PRETTY_FUNCTION__ )
{
    ErrorPrint( input1, input2, input3, "", function );
}

/**
 *  2 parameter error message
 */
template <typename I1, typename I2>
void ErrorPrint( I1 input1, I2 input2, const char* function = __PRETTY_FUNCTION__ )
{
    ErrorPrint( input1, input2, "", function );
}

/**
 *  1 parameter error message
 */
template <typename I1>
void ErrorPrint( I1 input1, const char* function = __PRETTY_FUNCTION__ )
{
    ErrorPrint( input1, "", function );
}

#endif /* LOGPRINT_H_ */
