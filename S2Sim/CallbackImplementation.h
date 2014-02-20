/*
 * CallbackImplementation.h
 *
 *  Created on: Oct 17, 2013
 *      Author: Alper
 */

#ifndef CALLBACKIMPLEMENTATION_H_
#define CALLBACKIMPLEMENTATION_H_

#include "Callback.h"

class EmptyClass{};

template <typename TObj, typename TIn1 = EmptyClass, typename TIn2 = EmptyClass, typename TIn3 = EmptyClass, typename TIn4 = EmptyClass>
class CallbackImplementation : public Callback
{
    friend Callback*
    MakeCallback<TObj,TIn1,TIn2,TIn3,TIn4>( TObj*,
                                            void ( TObj::*function )( TIn1, TIn2, TIn3, TIn4 ),
                                            const TIn1 & in1,
                                            const TIn2 & in2,
                                            const TIn3 & in3,
                                            const TIn4 & in4 );

    public:
        typedef void ( TObj::*TFunction )( TIn1, TIn2, TIn3, TIn4 );

    private:
        TObj* m_obj;
        TFunction m_function;
        TIn1 m_in1;
        TIn2 m_in2;
        TIn3 m_in3;
        TIn4 m_in4;

    public:
        void Execute( void )
        {
            m_obj->*m_function( m_in1, m_in2, m_in3, m_in4 );
        }

        CallbackImplementation( TObj* obj,
                                TFunction function,
                                const TIn1 & in1,
                                const TIn2 & in2,
                                const TIn3 & in3,
                                const TIn4 & in4 ) : m_obj( obj ),
                                                     m_function( function ),
                                                     m_in1( in1 ),
                                                     m_in2( in2 ),
                                                     m_in3( in3 ),
                                                     m_in4( in4 )
        {
        }

        CallbackImplementation( const CallbackImplementation& copy ) : m_obj( copy.m_obj ),
                                                                       m_function( copy.m_function ),
                                                                       m_in1( copy.m_in1 ),
                                                                       m_in2( copy.m_in2 ),
                                                                       m_in3( copy.m_in3 ),
                                                                       m_in4( copy.m_in4 )
        {
        }

        ~CallbackImplementation( void ){}
};

template <typename TObj, typename TIn1, typename TIn2, typename TIn3>
class CallbackImplementation<TObj, TIn1, TIn2, TIn3, EmptyClass> : public Callback
{
    public:
        typedef void ( TObj::*TFunction )( TIn1, TIn2, TIn3 );

    private:
        TObj* m_obj;
        TFunction m_function;
        TIn1 m_in1;
        TIn2 m_in2;
        TIn3 m_in3;

    public:
        void Execute( void )
        {
            m_obj->*m_function( m_in1, m_in2, m_in3 );
        }

        CallbackImplementation( TObj* obj,
                                TFunction function,
                                const TIn1 & in1,
                                const TIn2 & in2,
                                const TIn3 & in3 ) : m_obj( obj ),
                                                     m_function( function ),
                                                     m_in1( in1 ),
                                                     m_in2( in2 ),
                                                     m_in3( in3 )
        {
        }

        CallbackImplementation( const CallbackImplementation& copy ) : m_obj( copy.m_obj ),
                                                                       m_function( copy.m_function ),
                                                                       m_in1( copy.m_in1 ),
                                                                       m_in2( copy.m_in2 ),
                                                                       m_in3( copy.m_in3 )
        {
        }

        ~CallbackImplementation( void ){}
};

template <typename TObj, typename TIn1, typename TIn2>
class CallbackImplementation<TObj, TIn1, TIn2, EmptyClass, EmptyClass> : public Callback
{
    public:
        typedef void ( TObj::*TFunction )( TIn1, TIn2 );

    private:
        TObj* m_obj;
        TFunction m_function;
        TIn1 m_in1;
        TIn2 m_in2;

    public:
        void Execute( void )
        {
            m_obj->*m_function( m_in1, m_in2 );
        }

        CallbackImplementation( TObj* obj,
                                TFunction function,
                                const TIn1 & in1,
                                const TIn2 & in2 ) : m_obj( obj ),
                                                     m_function( function ),
                                                     m_in1( in1 ),
                                                     m_in2( in2 )
        {
        }

        CallbackImplementation( const CallbackImplementation& copy ) : m_obj( copy.m_obj ),
                                                                       m_function( copy.m_function ),
                                                                       m_in1( copy.m_in1 ),
                                                                       m_in2( copy.m_in2 )
        {
        }

        ~CallbackImplementation( void ){}
};

template <typename TObj, typename TIn1>
class CallbackImplementation<TObj, TIn1, EmptyClass, EmptyClass, EmptyClass> : public Callback
{
    public:
        typedef void ( TObj::*TFunction )( TIn1 );

    private:
        TObj* m_obj;
        TFunction m_function;
        TIn1 m_in1;

    public:
        void Execute( void )
        {
            m_obj->*m_function( m_in1 );
        }

        CallbackImplementation( TObj* obj,
                                TFunction function,
                                const TIn1 & in1 ) : m_obj( obj ),
                                                     m_function( function ),
                                                     m_in1( in1 )
        {
        }

        CallbackImplementation( const CallbackImplementation& copy ) : m_obj( copy.m_obj ),
                                                                       m_function( copy.m_function ),
                                                                       m_in1( copy.m_in1 )
        {
        }

        ~CallbackImplementation( void ){}
};

template <typename TObj>
class CallbackImplementation<TObj, EmptyClass, EmptyClass, EmptyClass, EmptyClass> : public Callback
{
    public:
        typedef void ( TObj::*TFunction )( void );

    private:
        TObj* m_obj;
        TFunction m_function;

    public:
        void Execute( void )
        {
            m_obj->*m_function();
        }

        CallbackImplementation( TObj* obj,
                                TFunction function ) : m_obj( obj ),
                                                       m_function( function )
        {
        }

        CallbackImplementation( const CallbackImplementation& copy ) : m_obj( copy.m_obj ),
                                                                       m_function( copy.m_function )
        {
        }

        ~CallbackImplementation( void ){}
};

template <typename TObj, typename TIn1, typename TIn2, typename TIn3, typename TIn4>
Callback*
MakeCallback( TObj* obj,
              void ( TObj::*function )( TIn1, TIn2, TIn3, TIn4 ),
              const TIn1 & in1,
              const TIn2 & in2,
              const TIn3 & in3,
              const TIn4 & in4 )
{
    CallbackImplementation* newCallback = new CallbackImplementation<TObj, TIn1, TIn2, TIn3, TIn4>( obj,
                                                                                                    function,
                                                                                                    in1,
                                                                                                    in2,
                                                                                                    in3,
                                                                                                    in4 );
    return ( dynamic_cast<Callback*>( newCallback ) );
}

template <typename TObj, typename TIn1, typename TIn2, typename TIn3>
Callback*
MakeCallback( TObj* obj,
              typename CallbackImplementation<TObj, TIn1, TIn2, TIn3>::TFunction function,
              const TIn1 & in1,
              const TIn2 & in2,
              const TIn3 & in3 )
{
    CallbackImplementation* newCallback = new CallbackImplementation<TObj, TIn1, TIn2, TIn3>( obj,
                                                                                              function,
                                                                                              in1,
                                                                                              in2,
                                                                                              in3 );
    return ( dynamic_cast<Callback*>( newCallback ) );
}

template <typename TObj, typename TIn1, typename TIn2>
Callback*
MakeCallback( TObj* obj,
              typename CallbackImplementation<TObj, TIn1, TIn2>::TFunction function,
              const TIn1 & in1,
              const TIn2 & in2 )
{
    CallbackImplementation* newCallback = new CallbackImplementation<TObj, TIn1, TIn2>( obj,
                                                                                        function,
                                                                                        in1,
                                                                                        in2 );
    return ( dynamic_cast<Callback*>( newCallback ) );
}

template <typename TObj, typename TIn1>
Callback*
MakeCallback( TObj* obj,
              typename CallbackImplementation<TObj, TIn1>::TFunction function,
              const TIn1 & in1 )
{
    CallbackImplementation* newCallback = new CallbackImplementation<TObj, TIn1>( obj,
                                                                                  function,
                                                                                  in1 );
    return ( dynamic_cast<Callback*>( newCallback ) );
}

template <typename TObj>
Callback*
MakeCallback( TObj* obj,
              typename CallbackImplementation<TObj>::TFunction function )
{
    CallbackImplementation* newCallback = new CallbackImplementation<TObj>( obj,
                                                                            function );
    return ( dynamic_cast<Callback*>( newCallback ) );
}

#endif /* CALLBACKIMPLEMENTATION_H_ */
