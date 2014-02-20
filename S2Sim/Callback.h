/*
 * Callback.h
 *
 *  Created on: Oct 17, 2013
 *      Author: Alper
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

class Callback
{
    public:
        virtual void Execute( void ) = 0;
        virtual ~Callback( void ) = 0;
};


#endif /* CALLBACK_H_ */
