/*
 *	r_ffp.h		replacements for DICE float-functions
 *				version 1.25
 *
 *	Copyright © 1995, 1996 Jarno Seppänen; see 3MU.c for licensing info.
 */

#ifndef R_FFP_H
#define R_FFP_H 1


/*	-------- Matt Dillon's DICE float replacements -------- */
#ifdef _DCC

/* Amiga SPxxx prototypes */
#include <clib/mathffp_protos.h>
#include <clib/mathtrans_protos.h>


typedef signed long	r_float;

/* 1 if negative, 0 if positive */
#define r_sign(f)		(((f)&0x00000080)?1:0)


/* FFP constants */

#define	FP1E1		0xA0000044	/* 1*(10^1)	*/
#define	FP1E2		0xC8000047	/* 1*(10^2)	*/
#define	FP1E3		0xFA00004A	/* 1*(10^3)	*/
#define	FP1E4		0x9C40004E	/* 1*(10^4)	*/
#define	FP1E5		0xC3500051	/* 1*(10^5)	*/
#define	FP1E6		0xF4240054	/* 1*(10^6)	*/
#define	FP1E7		0x98968058	/* 1*(10^7)	*/
#define	FP1E8		0xBEBC205B	/* 1*(10^8)	*/
#define	FP1E9		0xEE6B285E	/* 1*(10^9)	*/
#define FP1E10		0x9502F962	/* 1*(10^10)*/

#define FP0			0x00000000	/* 0	*/
#define	FP1			0x80000041	/* 1	*/

#define FP0_5		0x80000040	/* 0,5		*/
#define	FP1__2		FP0_5		/* 1/2		*/
#define FP1__3		0xAAAAAB3F	/* 1/3		*/
#define FP1__4		0x8000003F	/* 1/4		*/
#define FP1__5		0xCCCCCD3E	/* 1/5		*/
#define FP0_1		0xCCCCCD3D	/* 0,1		*/
#define FP0_05		0xCCCCCD3C	/* 0,05		*/

extern r_float		callffp( (*MatFunc)( r_float, ... ), ... );
extern long			cmpable( r_float );

extern unsigned long afp( signed char *string );


/* replacement for <libraries/mathffp.h> */

#ifndef PI
#define PI		0xC90FDB42	/* 3.141592653589793	*/
#endif
#define TWO_PI	0xC90FDB43	/* PI * 2				*/
#define PI2		0xC90FDB41	/* PI / 2				*/
#define PI4		0xC90FDB40	/* PI / 4				*/
#ifndef E
#define E		0xADF85442	/* 2.718281828459045	*/
#endif
#define LOG10	0x935D8E42 	/* 2.302585092994046	*/

#define r_cmpable(a)	cmpable(a)

#define r_trunc(x) 		((long)callffp(SPFix,(x)))
#define r_round(x)	 	((long)callffp(SPFix,(callffp(SPAdd,(x),((x)&0x00000080)|FPHALF))))
#define r_itof(i)	  	callffp(SPFlt,(long)(i))

#define r_cmp(a,b)		((long)callffp(SPCmp,(a),(b)))
#define r_tst(a)		((long)callffp(SPTst,(a)))
#define r_neg(a)		callffp(SPNeg,(a))

#define r_add(a,b)		callffp(SPAdd,(a),(b))
#define r_sub(a,b)		callffp(SPSub,(b),(a))
#define r_mul(a,b)		callffp(SPMul,(a),(b))
#define r_div(a,b)		callffp(SPDiv,(b),(a))

#define	r_fabs(a)		callffp(SPAbs,(a))
#define r_floor(a)		callffp(SPFloor,(a))
#define	r_ceil(a)		callffp(SPCeil,(a))

#define r_sin(a)		callffp(SPSin,(a))
#define r_cos(a)		callffp(SPCos,(a))
#define	r_tan(a)		callffp(SPTan,(a))
#define r_asin(a)		callffp(SPAsin,(a))
#define r_acos(a)		callffp(SPAcos,(a))
#define	r_atan(a)		callffp(SPAtan,(a))
#define	r_sinh(a)		callffp(SPSinh,(a))
#define r_cosh(a)		callffp(SPCosh,(a))
#define r_tanh(a)		callffp(SPTanh,(a))

#define r_exp(a)		callffp(SPExp,(a))
#define r_pow(a,b)		callffp(SPPow,(b),(a))
#define r_log(a)		callffp(SPLog,(a))
#define r_log10(a)		callffp(SPLog10,(a))
#define r_sqrt(a)		callffp(SPSqrt,(a))

#define	LIBRARIES_MATHFFP_H 1



/*	-------- the previous in an ordinary manner -------- */
#else	/* _DCC */


#include <math.h>
#include <stdlib.h>

typedef float	r_float;

#define	r_sign(f) (((f)<0.0)?1:0)

#define FP0		(0.0)
#define FP1		(1.0)

#define	FP1E1	(1e1)
#define	FP1E2	(1e2)
#define	FP1E3	(1e3)
#define	FP1E4	(1e4)
#define	FP1E5	(1e5)
#define	FP1E6	(1e6)
#define	FP1E7	(1e7)
#define	FP1E8	(1e8)
#define	FP1E9	(1e9)
#define FP1E10	(1e10)
#define FP0_5	(0.5)
#define FP1__3	(1.0/3.0)
#define FP0_1	(0.1)
#define FP0_05	(0.05)

#define	afp(s)	((float)atof(s))

#ifndef PI
#define PI		(3.141592653589793)
#endif
#ifndef TWO_PI
#define TWO_PI	(PI*2)
#endif
#ifndef PI2
#define PI2		(PI/2)
#endif
#ifndef PI4
#define PI4		(PI/4)
#endif
#ifndef E
#define E		(2.718281828459045)
#endif
#ifndef LOG10
#define LOG10	(2.302585092994046)
#endif

#define r_cmpable(a)	(a)

#define r_trunc(x) 		((long)(x))
#define r_round(x) 		((long)(((x)>0.0)?((x)+0.5):((x)-0.5)))
#define r_itof(i)  		((float)(i))

#define r_cmp(a,b)		(0)
#define r_tst(a)		(0)
#define r_neg(a)		(-(a))

#define r_add(a,b)		((a)+(b))
#define r_sub(a,b)		((a)-(b))
#define r_mul(a,b)		((a)*(b))
#define r_div(a,b)		((a)/(b))

#define	r_fabs(a)		(float)fabs(a)
#define r_floor(a)		(float)floor(a)
#define	r_ceil(a)		(float)ceil(a)

#define r_sin(a)		(float)sin(a)
#define r_cos(a)		(float)cos(a)
#define	r_tan(a)		(float)tan(a)
#define r_asin(a)		(float)asin(a)
#define r_acos(a)		(float)acos(a)
#define	r_atan(a)		(float)atan(a)
#define	r_sinh(a)		(float)sinh(a)
#define r_cosh(a)		(float)cosh(a)
#define r_tanh(a)		(float)tanh(a)

#define r_exp(a)		(float)exp(a)
#define r_pow(a,b)		(float)pow((a),(b))
#define r_log(a)		(float)log(a)
#define r_log10(a)		(float)log10(a)
#define r_sqrt(a)		(float)sqrt(a)



/*	-------- universal stuff -------- */
#endif	/* _DCC */


#define MAXNUMS 7	/* Motorola FFP has 7 number precision */

#define	FP2			r_itof( 2 )		/* 2	*/
#define	FP3			r_itof( 3 )		/* 3	*/
#define	FP4			r_itof( 4 )		/* 4	*/
#define	FP5			r_itof( 5 )		/* 5	*/
#define	FP6			r_itof( 6 )		/* 6	*/
#define	FP7			r_itof( 7 )		/* 7	*/
#define	FP8			r_itof( 8 )		/* 8	*/
#define	FP9			r_itof( 9 )		/* 9	*/
#define FP10		r_itof( 10 )	/* 10	*/

#define FP_1		r_itof( -1 )	/* -1	*/
#define FP_10		r_itof( -10 )	/* -10	*/

#define FPTEN	FP10
#define FPONE	FP1
#define FPHALF	FP0_5
#define FPZERO	FP0

/* get the integer part of f	*/
#define KOKOSA(f)	r_trunc(f)

/* get lg e decimals of f		*/
#define DESOSA(f,e)	r_round(r_mul(r_sub((f),r_trunc(f)),(e)))


#endif	/* R_FFP_H */
