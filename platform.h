
/*
 * Name:
 *   platform.h
 *
 * Purpose:
 *   Define global platform-specific settings.
 *
 * Language:
 *   ANSI C
 * Author:
 *   Martin N Dunstan (mdunstan@computing.dundee.ac.uk)
 *   {add_authors_here}
 *
 * History:
 *   14-Nov-2002 (mnd)
 *      Converted from C++ into C (comments).
 * Future work:
 *   {add_suggestions_here}
 *
 * Bugs:
 *   {add_new_bugs_here}
 */

#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

/*----------------------------- WIN32 ---------------------------------*/
#ifdef _WIN32
/* MSVC++ 6.0: Identifier truncated to 255 chars in debug information */
#pragma warning (disable:4786)

/* MSVC++ 7.0: Exception specification ignored */
#pragma warning (disable:4290)

/* Sleep for a fixed number of seconds */
#define sleep(P)	Sleep((P)*1000)

/* Oh for platform independence ... */
#include <float.h>
#define isnan(d)	_isnan(d)
#define isinf(d)	_fpclass(d) & (_FPCLASS_NINF | _FPCLASS_PINF)

// Yet more platform dependencies.
#include <process.h>
#define getpid		_getpid

/* Socket stuff */
typedef int socklen_t;

/*----------------------------- !WIN32 ---------------------------------*/
#else
/* Assume UNIX-like */
#include <sys/types.h>
#include <unistd.h>
#endif

#endif
