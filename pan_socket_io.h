
/*
 * Name:
 *   pan_socket_io.h
 *
 * Purpose:
 *   Socket I/O functions designed for use with the PANGU network protocol.
 *
 * Description:
 *   This file contains low-level functions (pan_socket_read/pan_socket_write)
 *   for sending and receiving data over sockets with basic support for packet
 *   fragmentation. Short reads and short writes must be handled by users of
 *   these functions.
 *
 *   This file also contains medium-level functions for reading and writing
 *   values of specific C data types in a portable format for PANGU systems.
 *   Short reads and short writes must be handled by users of these functions.
 *
 * Notes:
 *   The integer value returned by every read and write function declared
 *   below is the number of bytes successfully read/written. The table below
 *   defines the number of bytes that each supported data type occupies and
 *   it is up to the caller to ensure that the result of these calls matches
 *   the corresponding value in the table:
 *
 *        uchar/char     1
 *        bool           1
 *        short/ushort   2
 *        long/ulong     4
 *        float          4
 *
 * Language:
 *   ANSI C 89
 *
 * Author:
 *   Martin N Dunstan (mdunstan@computing.dundee.ac.uk)
 *   {add_authors_here}
 *
 * Copyright:
 *   (c) Space Systems Research Group, University of Dundee, 2001-2002.
 *
 * History:
 *   14-Nov-2002 (mnd):
 *      Original version.
 *   {add_changes_here}
 *
 * Future work:
 *   {add_suggestions_here}
 *
 * Bugs:
 *   {add_new_bugs_here}
 */

#ifndef PAN_SOCKET_IO_H
#define PAN_SOCKET_IO_H

#include "platform.h"
#include "pangu_endian.h"
#include "socket_stuff.h"

/*
 * Read functions for primitive data types.
 */
int pan_socket_read_uchar(SOCKET, unsigned char *);
int pan_socket_read_char(SOCKET, char *);
int pan_socket_read_bool(SOCKET, char *);
int pan_socket_read_ushort(SOCKET, unsigned short *);
int pan_socket_read_short(SOCKET, short *);
int pan_socket_read_ulong(SOCKET, unsigned long *);
int pan_socket_read_long(SOCKET, long *);
int pan_socket_read_float(SOCKET, float *);
int pan_socket_read_string(SOCKET, char **);

/*
 * Write functions for primitive data types.
 */
int pan_socket_write_uchar(SOCKET, unsigned char);
int pan_socket_write_char(SOCKET, char);
int pan_socket_write_bool(SOCKET, char);
int pan_socket_write_ushort(SOCKET, unsigned short);
int pan_socket_write_short(SOCKET, short);
int pan_socket_write_ulong(SOCKET, unsigned long);
int pan_socket_write_long(SOCKET, long);
int pan_socket_write_float(SOCKET, float);
int pan_socket_write_string(SOCKET, char *);

/*
 * Low-level support functions.
 */
long pan_socket_read(SOCKET, void *, unsigned long);
	/*
	 * pan_socket_read(s, p, n) reads "n" bytes from socket "s" and
	 * stores them in the block of memory pointed to by "p". This is
	 * a low-level socket read function.
	 */

int pan_socket_write(SOCKET, void *, unsigned long);
	/*
	 * pan_socket_write(s, p, n) reads "n" bytes from the block of
	 * memory pointed to by "p" and writes them to the socket "s".
	 * This is a low-level socket write function.
	 */

unsigned long float2ulong(float);
	/*
	 * Convert a normalised single-precision float into our own
	 * little-endian format. We ignore issues of NaNs, infinities
	 * and subnormals (platform-specific).
	 */

float ulong2float(unsigned long);
	/*
	 * Convert from our little-endian format into a native float.
	 * See float2ulong() above for more details.
	 */

#endif
