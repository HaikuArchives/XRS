/*
 *	r_ffp.c		replacement float-functions for DICE
 *				version 1.4 © Jarno Sepp‰nen 1996
 */

#include <stdio.h>
#include <string.h>

#include "ffphack.h"
#include "r_ffp.h"
#include "types.h"

#define CHARTOINT(c)	((c) - '0')	/* muuttaa ASCII-numeron c luvuksi */
#define INTTOCHAR(c)	((c) + '0')	/* muuttaa luvun c ASCII-numeroksi */

long r_ftoa( r_float num, STRPTR dec, unsigned char pts );
long r_rndftoa( r_float f, STRPTR dest, unsigned char pts );
STRPTR r_sciftoa( STRPTR dest, r_float f, unsigned char nums );
STRPTR r_numftoa( STRPTR dest, r_float f, unsigned char nums );
r_float r_atof( STRPTR asc );

static r_float readnum( STRPTR *asc, int *nums );
static r_float kokf( r_float num );
static r_float desf( r_float num );

/* long ko = r_ftoa( r_float num, STRPTR dec, unsigned char pts );
 * ko = kokonaisosa
 * num = konvattava numero
 * dec = paikka, minne desimaaliosa laitetaan
 * pts = desimaalin pituus
 * dec:ss‰ pit‰‰ olla tilaa pts + 1:lle merkille
 */
long r_ftoa( r_float num, STRPTR dec, unsigned char pts )
{
	long ko;		/* kokonaisosa */
	long tmp;
	unsigned char l = 0;

	if( dec == NULL )
		return( 0 );
/* tulostus vie tilaa pts+1 tavua */

	dec[ pts ] = 0;

	if( !ISLEGALFFP( num ))
	{
		strncpy( dec, "ILLEGAL", pts );
		return( 0 );
	}

	num = r_fabs( r_sub( num, r_itof( ko = r_trunc( num ))));

	while( l < pts )
	{
		num = r_mul( num, FPTEN );
		tmp = r_trunc( num );
		num = r_sub( num, r_itof( tmp ));

		dec[ l ] = INTTOCHAR( tmp );
		l++;
	}

	return( ko );
}

/* long ko = r_rndftoa( r_float num, STRPTR dec, unsigned char pts );
 * ko = kokonaisosa
 * num = konvattava numero
 * dec = paikka, minne desimaaliosa laitetaan
 * pts = desimaaliosan pituus
 * dec:ss‰ pit‰‰ olla tilaa pts + 1:lle merkille
 */
long r_rndftoa( r_float f, STRPTR dest, unsigned char pts )
{
	r_float offset;

	if( dest == NULL )
		return( 0 );
/* tulostus vie tilaa pts+1 tavua */

	/* offset = 0.05 / ( 10^pts ) */

	offset = r_div( FP0_05, r_pow( FP10, r_itof( (long)pts )));

	return( r_ftoa( r_add( f, offset ), dest, pts ));
}

/* tulostus vie tilaa nums+7 tavua */

/* nums=7
 *		vaihtoehdot ovat				tulostus
 *		esimerkki:
 * 1)	±000 000 000.123 456 7xx		`±1.234567e-01'
 * 2)	±000 000 123.456 7xx xxx		`±1.234567e+02'
 * 3)	±001 234 567.xxx xxx xxx		`±1.234567e+06'
 * 4)	±123 456 7xx.xxx xxx xxx		`±1.234567e+08'
 * nums=1
 *		esimerkki:
 * 1)	±000 000 000.123 456 7xx		`±1e-01'
 * 2)	±000 000 123.456 7xx xxx		`±1e+02'
 * 3)	±001 234 567.xxx xxx xxx		`±1e+06'
 * 4)	±123 456 7xx.xxx xxx xxx		`±1e+08'
 *		
 *		erikoistapaukset:
 * 1)	0								` 0.000000    '
 * 2)	‰‰retˆn tai miinus ‰‰retˆn		`±INFINITY    '
 * 3)	laiton FFP-luku					`ILLEGAL      '
 */
STRPTR r_sciftoa( STRPTR dest, r_float f, unsigned char nums )
{
	unsigned char endnum;
	long cf, eks, mank;
	char tek, tey;

	if( dest == NULL )
		return( NULL );
	if( !nums )
	{
		dest[ 0 ] = 0;
		return( dest );
	}
	endnum = nums + 6;
	if( !( nums - 1 ))
		endnum = nums + 5;
	dest[ endnum ] = 0;
	if( !ISLEGALFFP( f ))
	{
		if( f & FFP_INFINITY )
		{
			strncpy( dest, "+INFINITY", nums + 6 );
			if( f == FFP_NINFINITY )
				dest[ 0 ] = '-';
			return( dest );
		}

		strncpy( dest, "ILLEGAL", nums + 6 );
		return( dest );
	}
	cf = r_cmpable( f );
	if( cf == FP0 )
	{
		unsigned char cnt;
		for( cnt = 0; cnt < endnum; cnt++ )
			dest[ cnt ] = ' ';
		for( cnt = 1; cnt <= endnum - 5; cnt++ )
			dest[ cnt ] = '0';

		dest[ 2 ] = '.';
		return( dest );
	}

		/* lasketaan kokonaisosan 10-kantaisten numeroiden m‰‰r‰ */
	eks = 0;
	while( cf >= r_cmpable( r_pow( FP10, r_itof( eks ))))
		eks++;

		/* v‰hennet‰‰n siit‰ yksi ja saadaan; eksponentti!		*/

	eks--;

		/* skaalataan numero v‰lille [1, 10[						*/
		/* eli kerrotaan numero 10^(-eks) = 0.1^eks:ll‰				*/
	f = r_mul( f, r_pow( FP0_1, r_itof( eks )));

		/* lasketaan siit‰ mantissa								*/
	mank = 0;
	if( nums - 1 )
		mank = r_rndftoa( f, &(dest[ 3 ]), nums - 1 );

	dest[ 0 ] = '+';

	if( mank < 0 )
	{
		dest[ 0 ] = '-';
		mank = 0 - mank;
	}

	dest[ 1 ] = (char)mank + '0';
	if( nums - 1 )
		dest[ 2 ] = '.';

	dest[ endnum - 4 ] = 'e';
	dest[ endnum - 3 ] = '+';

	if( eks < 0 )
	{
		dest[ endnum - 3 ] = '-';
		eks = 0 - eks;
	}

	tek = (char)eks / 10;				/* eksponentin kymmenet		*/
	tey = (char)eks - ( tek * 10 );		/* eksponentin ykkˆset		*/

	dest[ endnum - 2 ] = tek + '0';
	dest[ endnum - 1 ] = tey + '0';

	return( dest );
}

/* r_numftoa vie destist‰ max nums+4 tavua, min nums+1 tavua */
/* nums=5:
 *	"12345"
 *	"-12345"
 *	"1234.5"
 *	"-1234.5"
 *	"0.12345"
 *	"-0.12345"
 */
STRPTR r_numftoa( STRPTR dest, r_float f, unsigned char nums )
{
	long cf, kn;

	if( dest == NULL )
		return( NULL );
	dest[ nums + 3 ] = 0;

	if( !ISLEGALFFP( f ))
	{
		if( f & FFP_INFINITY )
		{
			strncpy( dest, "+INFINITY", nums + 3 );
			if( f == FFP_NINFINITY )
				dest[ 0 ] = '-';
			return( dest );
		}

		strncpy( dest, "ILLEGAL", nums + 3 );
		return( dest );
	}

	/* lasketaan kokonaisosan 10-kantaisten numeroiden m‰‰r‰ */
	cf = r_cmpable( r_fabs( f ));
	kn = 0;
	while( cf >= r_cmpable( r_pow( FP10, r_itof( kn ))))
		kn++;

	/* jos kokonaisosa ei mahdu niin printataan r_sciftoa:lla */
	if( kn > nums - (unsigned char)r_sign( f ))
	{
		if( nums > ( 7 - 4 ))
			return( r_sciftoa( dest, f, nums ));
		else
		{
			strncpy( dest, "xxxxxxxxxxxxx", nums + 3 );
			dest[ nums + 3 ] = 0;
			return( dest );
		}
	}

	/* tehd‰‰n destiin nums-pituinen string: */
	if( desf( f ) != FP0 )
	{
		long dot = kn ? kn : 1;
		if( r_sign( f ))
			dot++;
		sprintf( &dest[ 0 ], "%ld", r_rndftoa( f, &dest[ dot + 1 ], nums - kn ));
		dest[ dot ] = '.';
		dest[ nums + 3 ] = 0;
	}
	else
		sprintf( dest, "%ld", r_round( f ));

	return( dest );
}

/*	1			+1			-1
 *	1.0			+1.0		-1.0
 *	1,0			+1,0		-1,0
 *
 *	1e0			+1e0		-1e0		1E0			+1E0		-1E0
 *	1.0e0		+1.0e0		-1.0e0		1.0E0		+1.0E0		-1.0E0
 *	1,0e0		+1,0e0		-1,0e0		1,0E0		+1,0E0		-1,0E0
 *	1e0.0		+1e0.0		-1e0.0		1E0.0		+1E0.0		-1E0.0
 *	1.0e0.0		+1.0e0.0	-1.0e0.0	1.0E0.0		+1.0E0.0	-1.0E0.0
 *	1,0e0.0		+1,0e0.0	-1,0e0.0	1,0E0.0		+1,0E0.0	-1,0E0.0
 *
 *	1e+0		+1e+0		-1e+0		1E+0		+1E+0		-1E+0
 *	1.0e+0		+1.0e+0		-1.0e+0		1.0E+0		+1.0E+0		-1.0E+0
 *	1,0e+0		+1,0e+0		-1,0e+0		1,0E+0		+1,0E+0		-1,0E+0
 *	1e+0.0		+1e+0.0		-1e+0.0		1E+0.0		+1E+0.0		-1E+0.0
 *	1.0e+0.0	+1.0e+0.0	-1.0e+0.0	1.0E+0.0	+1.0E+0.0	-1.0E+0.0
 *	1,0e+0.0	+1,0e+0.0	-1,0e+0.0	1,0E+0.0	+1,0E+0.0	-1,0E+0.0
 *
 *	1e-0		+1e-0		-1e-0		1E-0		+1E-0		-1E-0
 *	1.0e-0		+1.0e-0		-1.0e-0		1.0E-0		+1.0E-0		-1.0E-0
 *	1,0e-0		+1,0e-0		-1,0e-0		1,0E-0		+1,0E-0		-1,0E-0
 *	1e-0.0		+1e-0.0		-1e-0.0		1E-0.0		+1E-0.0		-1E-0.0
 *	1.0e-0.0	+1.0e-0.0	-1.0e-0.0	1.0E-0.0	+1.0E-0.0	-1.0E-0.0
 *	1,0e-0.0	+1,0e-0.0	-1,0e-0.0	1,0E-0.0	+1,0E-0.0	-1,0E-0.0
 */

r_float r_atof( STRPTR asc )
{
	r_float *f, m = FP0, e = FP0, tf;
	BOOL sign = 0, dot = 0, min = 0, exp = 0;
	int nums = 0, tnums;

	f = &m;	/* luetaan ekaksi mantissaa */

	while( *asc )
	{
		switch( *asc )
		{
			case '+':
				if( sign )
					return( FFP_ERROR );
				sign = 1;
				break;
			case '-':
				if( sign )
					return( FFP_ERROR );
				sign = 1;
				min = 1;
				break;
			case '.':
			case ',':
				if( dot )
					return( FFP_ERROR );
				dot = 1;
				break;
			case 'e':
			case 'E':
				if( exp )
					return( FFP_ERROR );
				exp = 1;
				sign = 0;
				dot = 0;
				min = 0;
				f = &e;		/* luetaan eksponenttia */
				break;
			default:
				tf = readnum( &asc, &tnums );
				if( !tnums )
					return( FFP_ERROR );
				if( tf == FFP_OVERFLOW );
					return( FFP_OVERFLOW );
				if( !exp )
					nums += tnums;
				if( min )
					tf = r_neg( tf );
				if( dot )
				{
					tf = r_mul( tf, r_pow( FP0_1, r_itof( tnums )));
					*f = r_add( *f, tf );
				}
				else
					*f = tf;
				break;
		}
	}
	return( r_mul( m, r_pow( FP1E1, e )));
}

static r_float readnum( STRPTR *asc, int *nums )
{
	r_float num = FP0;
	*nums = 0;
	while( **asc >= '0' && **asc <= '9' && *nums <= MAXNUMS )
	{
		num = r_mul( num, FP10 );
		switch( **asc )
		{
			case '1':
				num = r_add( num, FP1 );
				break;
			case '2':
				num = r_add( num, FP2 );
				break;
			case '3':
				num = r_add( num, FP3 );
				break;
			case '4':
				num = r_add( num, FP4 );
				break;
			case '5':
				num = r_add( num, FP5 );
				break;
			case '6':
				num = r_add( num, FP6 );
				break;
			case '7':
				num = r_add( num, FP7 );
				break;
			case '8':
				num = r_add( num, FP8 );
				break;
			case '9':
				num = r_add( num, FP9 );
				break;
		}
		(*asc)++;
		(*nums)++;
	}
	return( *nums <= MAXNUMS ? num : FFP_OVERFLOW );
}

static r_float kokf( r_float num )
{
	return( r_itof( r_trunc( num )));
}
static r_float desf( r_float num )
{
	return( r_fabs( r_sub( num, kokf( num ))));
}
