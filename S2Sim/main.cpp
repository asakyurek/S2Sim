/**
 * @file main.cpp
 * Main file that starts an infinite running loop on the SystemManager::AdvanceTimeStep method.
 *  @date Sep 19, 2013
 *  @author: Alper Sinan Akyurek
 */

#include "MatlabManager.h"
#include "ConnectionManager.h"
#include "ControlManager.h"
#include "SystemManager.h"

int
main( int argc, char **argv )
{
    LOG_FUNCTION_START();
    LogPrint( "S2Sim Started" );
    
    GetConnectionManager();
    GetMatlabManager();
    GetControlManager();
    GetSystemManager();
    
    unsigned int iterationNumber = 0;
    
    GetControlManager().WaitUntilReady();
    //int starting;
    //std::cin >> starting;
    while ( 1 )
    {
        //usleep(1);
        GetSystemManager().AdvanceTimeStep();
        ++iterationNumber;
        LogPrint( "Time: ", iterationNumber );
    }
    LOG_FUNCTION_END();
    return ( EXIT_SUCCESS );
}



