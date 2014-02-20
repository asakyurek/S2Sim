/*
 * test.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: asakyurek
 */

#include <pthread.h>
#include "ThreadBase.h"
#include <iostream>

class MyThread : public ThreadBase
{
    private:
        void* ExecutionBody( void* input )
        {
            std::cout << "Hello~" << (char*)input << std::endl;
            return ( NULL );
        }
};

int
main(int argc, char **argv)
{
    MyThread x;

    x.StartThread( ( void* )" all!" );
    int a = 0;
    std::cin >> a;
}


