/**
 * @file CompileTimeCheckerLibrary.h
 * This file contains template classes to do compile time checking.
 *  Created on: Oct 8, 2013
 *      Author: Alper
 */

#ifndef COMPILETIMECHECKERLIBRARY_H_
#define COMPILETIMECHECKERLIBRARY_H_

/**
 *  
    This class has two specializations. If the expression is true, the static Check() function will be public and compilation will continue.
 
    @tparam expression This is a boolean expression that can be evaluated in compile time.
    @tparam Reason This can be the name of any class that will be used for debug information only.
 */
template <bool expression, typename Reason = class NoReason>
class CompileCheck
{
    public:
    /**
     *  Enumeration for Result displaying.
     */
        enum
        {
            /**
             *  The check passed.
             */
            Result = true
        };
    /**
     *  Empty function for visual purposes.
     */
        static void
        Check( void ){}
};

/**
 *  Second specialization of the class, when the expression is not true. The static Check() function is this time private and the compilation will fail.
 */
template <typename Reason>
class CompileCheck<false, Reason>
{
    private:
    /**
     *  Enumeration for Result displaying.
     */
        enum
        {
            /**
             *  The check has failed.
             */
            Result = true
        };
    /**
     *  Empty function for visual purposes.
     */
        static void
        Check( void ){}
};

/**
 *  This class checks the size of a type with the given size and gives a compile error with the reason parameter is the check fails.
 
    @tparam checkedType A type that will be checked for size.
    @tparam checkedSize The size that the type should have.
    @tparam Reason Any class name that will be used for debug information only.
 */
template <typename checkedType, unsigned int checkedSize, typename Reason = class NoReason>
class SizeCheck
{
    public:
    /**
     *  Enumeration for Result displaying.
     */
        enum
        {
            /**
             *  Result is the result of the compile check class.
             */
            Result = CompileCheck<sizeof( checkedType ) == checkedSize, Reason>::Result
        };
    /**
     *  Empty class that will only compile if the check passes.
     */
        static void
        Check( void )
        {
            CompileCheck<sizeof( checkedType ) == checkedSize, Reason>::Check();
        }
};

#endif /* COMPILETIMECHECKERLIBRARY_H_ */
