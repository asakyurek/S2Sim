/*
 * CompileTimeCheckerLibrary.h
 *
 *  Created on: Oct 8, 2013
 *      Author: Alper
 */

#ifndef COMPILETIMECHECKERLIBRARY_H_
#define COMPILETIMECHECKERLIBRARY_H_

template <bool expression, typename Reason = class NoReason>
class CompileCheck
{
    public:
        enum
        {
            Result = true
        };
        static void
        Check( void ){}
};

template <typename Reason>
class CompileCheck<false, Reason>
{
    private:
        enum
        {
            Result = true
        };
        static void
        Check( void ){}
};

template <typename checkedType, unsigned int checkedSize, typename Reason = class NoReason>
class SizeCheck
{
    public:
        enum
        {
            Result = CompileCheck<sizeof( checkedType ) == checkedSize, Reason>::Result
        };
        static void
        Check( void )
        {
            CompileCheck<sizeof( checkedType ) == checkedSize, Reason>::Check();
        }
};

#endif /* COMPILETIMECHECKERLIBRARY_H_ */
