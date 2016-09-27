
/*
 * Name:
 *   pan_protocol_lib.h
 *
 * Purpose:
 *   Public C interface to the client end of the PANGU network protocol.
 *
 * Description:
 *
 * Notes:
 *
 * Language:
 *   ANSI C
 *
 * Author:
 *   Martin N Dunstan (mdunstan@computing.dundee.ac.uk)
 *   {add_authors_here}
 *
 * History:
 *   22-Oct-2002 (mnd):
 *      Original version.
 *   {add_changes_here}
 *
 * Future work:
 *   {add_suggestions_here}
 *
 * Bugs:
 *   {add_new_bugs_here}
 */

#ifndef PAN_PROTOCOL_LIB_H_INCLUDED
#define PAN_PROTOCOL_LIB_H_INCLUDED

#include "pan_socket_io.h"


/* Client message numbers */
enum
{
	/* Version 1.00 command/request messages */
	MSG_GOODBYE			=   0,
	MSG_GET_IMAGE			=   1,
	MSG_GET_ELEVATION		=   2,
	MSG_GET_ELEVATIONS		=   3,
	MSG_LOOKUP_POINT		=   4,
	MSG_LOOKUP_POINTS		=   5,
	MSG_GET_POINT			=   6,
	MSG_GET_POINTS			=   7,
	MSG_ECHO			=   8,
	/* Version 1.01 */
	MSG_GET_RANGE_IMAGE		=   9,
	MSG_GET_RANGE_TEXTURE		=  10,
	/* Version 1.02 */
	MSG_GET_VIEWPOINT_BY_ANGLE	=  11,
	MSG_GET_VIEWPOINT_BY_QUATERNION	=  12,
	/* Version 1.03 */
	MSG_GET_LIDAR_PULSE_RESULT	=  13,
	MSG_GET_LIDAR_MEASUREMENT	=  14,
	/* Version 1.06 */
	MSG_GET_RADAR_RESPONSE		=  15,

	/* Version 1.00 configuration messages */
	MSG_SET_VIEWPOINT_BY_ANGLE	= 256,
	MSG_SET_VIEWPOINT_BY_QUATERNION	= 257,
	MSG_SET_AMBIENT_LIGHT		= 258,
	MSG_SET_SUN_COLOUR		= 259,
	MSG_SET_SKY_TYPE		= 260,
	MSG_SET_FIELD_OF_VIEW		= 261,
	MSG_SET_ASPECT_RATIO		= 262,
	MSG_SET_BOULDER_VIEW		= 263,
	MSG_SET_SURFACE_VIEW		= 264,
	/* Version 1.03 */
	MSG_SET_LIDAR_PARAMETERS	= 265,
	/* Version 1.04 */
	MSG_SET_CORNER_CUBES		= 266,
	/* Version 1.05 */
	MSG_SET_CORNER_CUBE_ATTITUDE	= 267,

	MSG_CLIENT_LIMIT
};

/* Server message numbers */
enum
{
	MSG_OKAY			=   0,
	MSG_ERROR			=   1,
	MSG_IMAGE			=   2,
	MSG_FLOAT			=   3,
	MSG_FLOAT_ARRAY			=   4,
	MSG_3D_POINT			=   5,
	MSG_3D_POINT_ARRAY		=   6,
	MSG_MEMORY_BLOCK		=   7,
	MSG_ECHO_REPLY			=   8,
	MSG_LIDAR_PULSE_RESULT		=   9,
	MSG_LIDAR_MEASUREMENT		=  10,
	MSG_RADAR_RESPONSE		=  11,

	MSG_SERVER_LIMIT
};

/* Structure representing floating point values and a validity flag */
struct optional_float
{
	float	value;
	char	valid;
};


void pan_protocol_start(SOCKET);
	/*
	 * pan_protocol_start() starts a PANGU network protocol session.
	 */

void pan_protocol_finish(SOCKET);
	/*
	 * pan_protocol_finish() ends a PANGU network protocol session. The
	 * remote server will probably close the connection afterwards.
	 */

void pan_protocol_expect(SOCKET, unsigned long);
	/*
	 * pan_protocol_expect(s, t) reads the next message type from "s" and
	 * checks to see if it matches the message type "t". If it doesn't and
	 * the message type read is an error message then the error will be
	 * reported. Otherwise an "unexpected message" error will be reported.
	 * The program will terminate after reporting an error.
	 */

unsigned char *pan_protocol_get_image(SOCKET, unsigned long *);
	/*
	 * ptr = pan_protocol_get_image(s, &size) requests an image from the
	 * remote server using the current camera settings. Returns zero if an
	 * image could not be obtained from the server. The memory returned by
	 * the call is allocated by malloc() and may be released by free().
	 * The size field will be updated with the number of bytes in the
	 * result array.
	 */

unsigned char *pan_protocol_get_range_image(SOCKET, unsigned long *, float, float);
	/*
	 * ptr = pan_protocol_get_range_image(s, &size, o, k) requests a range
	 * image from the remote server using the current camera settings. All
	 * range values will have "o" subtracted before being multiplied by "k"
	 * to obtain a texture coordinate. The texture coordinate is clamped to
	 * lie in the range [0,1] and then multiplied by W-1 where W is the
	 * width of the range texture image to give a coordinate X. The value
	 * of range texture image pixel (X, 0) is then used as the pixel in the
	 * returned range image. The physical range is [o, o + 1/k].
	 *
	 * This function returns zero if an image could not be obtained from
	 * the server. The memory returned by the call is allocated by malloc()
	 * and may be released by free(). The size field will be updated with
	 * the number of bytes in the result array.
	 */

unsigned char *pan_protocol_get_range_texture(SOCKET, unsigned long *);
	/*
	 * ptr = pan_protocol_get_range_texture(s, &size) requests the range
	 * texture image from the remote server. This is a 1-D image whose
	 * width can be used to determine the depth resolution of images from
	 * pan_protocol_get_range_image(). The pixel values can be used to
	 * deduce physical ranges from pan_protocol_get_range_image() ranges.
	 *
	 * This function returns zero if an image could not be obtained from
	 * the server. The memory returned by the call is allocated by malloc()
	 * and may be released by free(). The size field will be updated with
	 * the number of bytes in the result array.
	 */

float pan_protocol_get_elevation(SOCKET, char *);
	/*
	 * h = pan_protocol_get_elevation(s, perr) returns the elevation of the
	 * camera relative to the remote model. If perr is not a null pointer
	 * then it will be set to zero if the returned elevation is invalid
	 * and non-zero if it is valid. An invalid elevation is returned when
	 * the camera is not directly above any part of the remote model.
	 */

void pan_protocol_get_elevations(SOCKET, unsigned long, float *, float *, char *);
	/*
	 * pan_protocol_get_elevations(s, n, pv, rv, ev) computes the camera
	 * elevations relative to the remote model for each of the "n" camera
	 * positions "pv" and writes the results into the array "rv". The ith
	 * elevation rv[i] is only valid (the position pv[3*i] is over the
	 * model) when ev[i] is non-zero. Both rv and ev must be point to an
	 * array of "n" elements.
	 */

void pan_protocol_lookup_point(SOCKET, float, float, float *, float *, float *, char *);
	/*
	 * h = pan_protocol_lookup_point(s, x, y, &px, &py, &pz, perr) returns
	 * the 3D position of the model (px, py, pz) under the pixel at
	 * coordinates (x, y) where (0, 0) represents the bottom left corner
	 * of the image and (1, 1) the top-right corner. If perr is not a null
	 * pointer then it will be set to zero if the returned point is invalid
	 * and non-zero if it is valid. An invalid point occurs when the centre
	 * of the specified pixel does not cover any part of the model.
	 */

void pan_protocol_lookup_points(SOCKET, unsigned long, float *, float *, char *);
	/*
	 * pan_protocol_lookup_points(s, n, pv, rv, ev) computes the 3D
	 * positions of each of the "n" pixels whose 2D positions are
	 * stored in "pv" and writes the 3D results into the array "rv".
	 * The ith position rv[3*i] is only valid if ev[i] is non-zero.
	 * The "rv" array must hold 3*n elements while the "ev" array must
	 * hold "n" elements.
	 */

void pan_protocol_get_point(SOCKET, float, float, float, float *, float *, float *, char *);
	/*
	 * pan_protocol_get_point(s, dx, dy, dz, &px, &py, &pz, perr) returns
	 * the 3D position (px, py, pz) of the model visible along direction
	 * (dx, dy, dz). If perr is not a null pointer then it will be set to
	 * zero if the returned point is invalid and non-zero if it is valid.
	 * An invalid point occurs when no part of the model is visible along
	 * the specified direction.
	 */

void pan_protocol_get_points(SOCKET, unsigned long, float *, float *, char *);
	/*
	 * pan_protocol_get_points(s, n, pv, rv, ev) computes the 3D positions
	 * of the "n" points on the model visible along the directions pv[3*i]
	 * and writes the results into the array rv[]. Each pv[3*i] position
	 * is only valid if ev[i] is non-zero. The "rv" array must hold 3*n
	 * elements while the "ev" array must hold "n" elements.
	 */

unsigned char *pan_protocol_get_viewpoint_by_angle(SOCKET, float, float, float, float, float, float, unsigned long *);
	/*
	 * pan_protocol_get_viewpoint_by_angle(s, x, y, z, yw, pi, rl, &size)
	 * is used to set the camera/viewpoint position to (x, y, z) and
	 * yaw/pitch/roll to (yw, pi, rl) and return an image from that
	 * position. Returns zero if a image could not be obtained from the
	 * server. The memory returned by the call is allocated by malloc()
	 * and may be released by free(). The size field will be updated with
	 * the number of bytes in the result array.
	 */

unsigned char *pan_protocol_get_viewpoint_by_quaternion(SOCKET, float, float, float, float, float, float, float, unsigned long *);
	/*
	 * pan_protocol_get_viewpoint_by_quaternion(s,x,y,z,q0,q1,q2,q3,&size)
	 * is used to set the camera/viewpoint position to (x, y, z) and
	 * attitude as defined by the quaternion [q0, q1, q2, q3] and return
	 * an image from that position. Returns zero if a image could not be
	 * obtained from the server. The memory returned by the call is
	 * allocated by malloc() and may be released by free(). The size field
	 * will be updated with the number of bytes in the result array.
	 */

void pan_protocol_get_lidar_pulse_result(SOCKET, float, float, float, float, float, float, float *, float *);
	/*
	 * pan_protocol_get_lidar_pulse_result(s, x,y,z, dx,dy,dz, &r, &a) is
	 * used to obtain the result of a LIDAR pulse from position (x, y, z)
	 * along direction (dx, dy, dz). The range to the surface and the
	 * cosine of the incidence angle will be written to r and a on return.
	 */

void pan_protocol_set_viewpoint_by_angle(SOCKET, float, float, float, float, float, float);
	/*
	 * pan_protocol_set_viewpoint_by_angle(s, x, y, z, yw, pi, rl) is used
	 * to set the camera/viewpoint position to (x, y, z) and attitude
	 * yaw/pitch/roll to (yw, pi, rl).
	 */

void pan_protocol_set_viewpoint_by_quaternion(SOCKET, float, float, float, float, float, float, float);
	/*
	 * pan_protocol_set_viewpoint_by_quaternion(s, x, y, z, q0, q1, q2, q3)
	 * is used to set the camera/viewpoint position to (x, y, z) and
	 * attitude as defined by the quaternion [q0, q1, q2, q3].
	 */

void pan_protocol_set_ambient_light(SOCKET, float, float, float);
	/*
	 * pan_protocol_set_ambient_light(s, r, g, b) is used to set the colour
	 * and intensity of ambient light in the red, green and blue channels
	 * to (r, g, b).
	 */

void pan_protocol_set_sun_colour(SOCKET, float, float, float);
	/*
	 * pan_protocol_set_sun_colour(s, r, g, b) is used to set the colour
	 * and intensity of the Sun in the red, green and blue channels to
	 * (r, g, b).
	 */

void pan_protocol_set_sky_type(SOCKET, unsigned long);
	/*
	 * pan_protocol_set_sky_type(s, t) is used to set the sky background
	 * type to "t" where "t" may hold one of the following values:
	 *    0 : no sky (black)
	 *    1 : fake sky (sphere-mapped texture ignoring camera roll)
	 *    2 : raw sky (single-pixel stars of varying intensity)
	 *    3 : red screen (maximum red, no green, no blue)
	 *    4 : green screen (no red, maximum green, no blue)
	 *    5 : blue screen (no red, no green, maximum blue)
	 */

void pan_protocol_set_field_of_view(SOCKET, float);
	/*
	 * pan_protocol_set_field_of_view(s, f) is used to set the angular
	 * field of view width to "f" degrees. The angular field of view
	 * height is determined from the width and the pixel aspect ratio.
	 */

void pan_protocol_set_aspect_ratio(SOCKET, float);
	/*
	 * pan_protocol_set_aspect_ratio(s, r) is used to set the angular
	 * aspect ratio of each pixel. This is used in conjunction with the
	 * angular field of view width to determine the angular field of view
	 * height.
	 */

void pan_protocol_set_boulder_view(SOCKET, unsigned long, int);
	/*
	 * pan_protocol_set_boulder_view(s, t, tex) is used to set the boulder
	 * rendering method to "t" and to enable or disable boulder texturing
	 * via "tex".
	 */

void pan_protocol_set_surface_view(SOCKET, unsigned long, int, int);
	/*
	 * pan_protocol_set_surface_view(s, t, tex, det) is used to set the
	 * surface rendering method to "t" and to enable or disable surface
	 * texturing and surface detailing.
	 */

void pan_protocol_set_lidar_parameters(SOCKET, float, float, unsigned long, unsigned long, float, float, unsigned long, unsigned long, unsigned long, unsigned long, float, float, float, float, float);
	/*
	 * pan_protocol_set_lidar_parameters(
	 *    s, fx,fy, nx,ny, tx,ty, n,m, t,
	 *    fl, az,el, th, wx, wy) is
	 * used to configure the LIDAR emitter/detector with field of view
	 * (fx, fy) degrees horizontally and vertically, an emitter/detector
	 * grid of nx by ny beams, and subsampling of n by m samples. The scan
	 * type is "t" where:
	 *      0: TV scan (left-to-right, top-to-bottom)
	 *      1: mode 01 (LiGNC project zig-zag scan, azi mirror before ele)
	 *      1: mode 02 (LiGNC project zig-zag scan, ele mirror before azi)
	 * The centre of the scan has LIDAR azimuth/elevation (az,el) and the
	 * beam half-angle is th degrees. The LIDAR flags fl is a bit vector:
	 *	bit 0: return range/slope results if set
	 *	bit 1: return azimuth/elevation values if set
	 *	bit 2: return corner cube range/slope results if set
	 *	bit 3: return time-of-pulse-emission values
	 * All other bits must be clear.
	 * Each detector pixel is wx by wy degrees in size. Note that when
	 * n and/or m are greater than 1 these are the sizes of the subsample
	 * pixels not the full pixel.
	 */

float *pan_protocol_get_lidar_measurement(SOCKET, float,float,float, float,float,float,float, float,float,float, float,float,float, float,float,float, float,float,float, float,float,float, float,float,float, float *,float *, unsigned long *,unsigned long *, float *,float *, unsigned long *,unsigned long *, unsigned long *, unsigned long *, float *,float *, float *);
	/*
	 * v = pan_protocol_get_lidar_measurement(
	 *        s, px,py,pz, q0,q1,q2,q3,
	 *        vx,vy,vz, rx,ry,rz,
	 *        ax,ay,az, sx,sy,sz,
	 *        jx,jy,jz, tx,ty,tz,
	 *        &fx,&fy, &nx,&ny, &tx,&ty, &n,&m, &t, &fl, &az, &el, &th) is
	 * used to request a LIDAR scan from position (px,py,pz) with attitude
	 * quaternion (q0,q1,q2,q3), linear velocity (vx,vy,vz) and angular
	 * velocity (rx,ry,rx), linear acceleration (ax,ay,az), angular
	 * acceleration (sx,sy,sz), linear jerk (jx,jy,jz) and angular jerk
	 * (tx,ty,tz). The return value v is a pointer to an array of scan
	 * results while the LIDAR emitter/detector settings are written to fx
	 * etc. If the scan has W by H beams with sub-sampling factors N and M
	 * and B blocks of results then "v" will contain 2*B*(W*N)*(H*M)
	 * floating point values representing the 2D scan results at the
	 * sub-sampling resolution NxM. The number of blocks B depends on the
	 * results requested in the LIDAR emitter/detector settings flag field.
	 *
	 * This function automatically converts from PANGU network floats to
	 * native floats before returning.
	 */

void pan_protocol_set_corner_cubes(SOCKET, unsigned long, unsigned long, float *);
	/*
	 * pan_protocol_set_corner_cubes(s, n, f, p) is used to pass an
	 * array of "n" corner cube records starting at address "p" using the
	 * array format "f". Currently "f" must be set to zero and "p" must
	 * consist of "n" consecutive blocks of corner cube records. Each
	 * block contains seven floating point values (px,py,pz,nx,ny,nz,r)
	 * where (px,py,pz) is the position of the cube, (nx,ny,nz) its face
	 * normal and r its effective radius.
	 *
	 * Note that corner cubes are only used when the LIDAR parameters
	 * flags field has bit 2 set.
	 */

void pan_protocol_set_corner_cube_attitude(SOCKET, float, float, float, float, float, float, float, float, float, float, float, float, float);
	/*
	 * pan_protocol_set_corner_cube_attitude(
	 *        s, q0,q1,q2,q3, rx,ry,rx, ax,ay,az, jx,jy,jz) is used
	 * to define the attitude (q0,q1,q2,q3), angular velocity (rx,ry,rz),
	 * angular acceleration (ax,ay,az) and angular jerk (jx,jy,jz) of the
	 * corner cube lattice at the centre of all future LIDAR frames.
	 */


float *pan_protocol_get_radar_response(SOCKET, unsigned long, unsigned long, unsigned long, unsigned long, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, unsigned long *, float *, float *, float *, float *, float *, float *, float *, float *, float *, float *, unsigned long *, unsigned long *, unsigned long *);
	/*
	 * v = pan_protocol_get_radar_response(s,fl,n,nr,ns,ox,oy,oz,vx,vy,vz,
	 *     q0,q1,q2,q3,bw,rmd,smd,rbs,sbs,
	 *     &st,&mxv,&totv,&offr,&offs,&bsr,&bss,&mnr,&mxr,&mns,&mxs,&nrb,
	 *     &nsb, &nused)
	 * is used to retrieve the RADAR response for a beam emitted from the
	 * point (ox,oy,oz) moving with velocity (vx,vy,vz) with axis
	 * (q0,q1,q2,q3) and width bw degrees. The beam is sampled n times and
	 * the results are integrated using a 2D histogram with nr range bins
	 * and ns speed bins. The fl argument defines flags for the simulation
	 * with the meanings of each bit defined as follows:
	 *      bit  0: if set then zero align the of the range histograms
	 *      bit  1: if set then centre align the range histograms on rmd
	 *      bit  2: if set then round range histogram width to power of 10
	 *	bit  3: if set then use fixed range histogram bin size rbs
	 *      bit  4: if set then zero align the speed histograms
	 *      bit  5: if set then centre align the speed histograms on smd
	 *      bit  6: if set then round speed histogram width to power of 10
	 *	bit  7: if set then use fixed speed histogram bin size sbs
	 *      bit  8: if set then surface slope effects are ignored
	 *      bit  9: if set then each sample is worth 1 not 1/n
	 * The status of the response is returned in st with the maximum signal
	 * value in mxv. The range associated with the left edge of the first
	 * histogram bin is offr. The minimum and maximum ranges before being
	 * clipping by the histogram are minr and maxr respectively; the
	 * minimum and maximum speeds is mins and maxs respectively. The size
	 * of each bin is bs. The number of bins actually created is nrb and
	 * nsb. The 2D array of nrb*nsb bins is returned as v consisting of
	 * nrb range values for the first speed histogram, then the next nrb
	 * range values for the second speed histogram etc. The sum of all
	 * elements of v is returned in totv. The number of samples actually
	 * used in the histogram (the number that hit a target) is returned in
	 * nused.
	 */

#endif
