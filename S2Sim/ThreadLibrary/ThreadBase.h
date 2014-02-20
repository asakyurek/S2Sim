/*
 * ThreadBase.h
 *
 *  Created on: Dec 21, 2013
 *      Author: asakyurek
 */

#ifndef THREADBASE_H_
#define THREADBASE_H_

#if defined( __APPLE__ ) || defined ( __linux__ )

#include "PosixThreadBase.h"

#endif

#if defined( _WIN32 ) || defined( _WIN64 )

#include "WindowsThreadBase.h"

#endif

#endif /* THREADBASE_H_ */
