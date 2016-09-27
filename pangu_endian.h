
/* Simple compile-time tests to decide what format floating point */
/* numbers are stored in and what the endian-ness is. */
#ifndef PANGU_ENDIAN_H_INCLUDED
#define PANGU_ENDIAN_H_INCLUDED


/* PC-based systems */
#if defined(__MSDOS__) || defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__NT__) || defined(__OS2__) || defined(__i386__)
#define HW_LITTLE_ENDIAN
#endif


/* Vaxen and PDPs have an interesting endian-ness ... */
#if defined(vax) && defined(ultrix)
#define	HW_VAX_ENDIAN
#endif


/* Alpha AXP platforms running OSF, OpenVMS or Linux */
#if defined(__alpha) && (defined(__osf__) || defined(__linux__))
#define HW_LITTLE_ENDIAN
#endif
#if defined(__ALPHA) && defined(__VMS)
#define HW_LITTLE_ENDIAN
#endif


/* AIX PS/2 */
#if defined(i386) && defined(aiws)
#define HW_LITTLE_ENDIAN
#endif


/* AIX 370, AIX ESA */
#if defined(_AIX370) || defined(_IBMESA)
#define HW_370
#define HW_BIG_ENDIAN
#endif


/* CMS */
#if defined(__EXTENDED__) && !defined(_IBMR2) && !defined(__OS2__)
#define HW_370
#define HW_BIG_ENDIAN
#endif


/* Everything else is assumed to be big-endian */
#if !defined(HW_LITTLE_ENDIAN) && !defined(HW_VAX_ENDIAN)
#define HW_BIG_ENDIAN
#endif


/*
 * Structure for easing S/370 and x86 float translation. Note that the
 * S/370 and co uses base-16 floating-point arithmetic and thus there is
 * no implicit bit in the mantissa. Oh and there aren't any NaNs or Infs.
 */
typedef union
{
	float whole;
	struct
	{
		unsigned fraction:	24;
		unsigned exponent:	 7;
		unsigned sign:		 1;
	} parts;
} s370_float_struct;


/* Structure for easing big-endian and x86 float translation */
typedef union
{
	float whole;
	struct
	{
		unsigned sign:		 1;
		unsigned exponent:	 8;
		unsigned fraction:	23;
	} parts;
} bigieee_float_struct;

/* Structure for accessing x86 float components */
typedef union
{
	float whole;
	struct
	{
		unsigned fraction:	23;
		unsigned exponent:	 8;
		unsigned sign:		 1;
	} parts;
} smallieee_float_struct;

#endif
