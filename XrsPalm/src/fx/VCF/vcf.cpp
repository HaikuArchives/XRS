
/*
 *	VCF.cpp	
 *	voltage-controlled filter emulations
 */
 

#include "VCF.h"

#define FILT_HAMRE          0
#define FILT_2POLE          1


VCF::VCF()
{
	filter = NULL;
}
long 
VCF::Init( char VCFpoles, char VCFtype )
{
	int i;
	if( !VCFpoles )
		return( 0 );
	type = VCFtype;
	poles = ( VCFpoles + 1 ) & ~1L;	/* pyöristetään ylöspäin parilliseksi */
	filter = (r_float *)malloc( poles / 2 * STACKSIZE * sizeof( r_float ));
	if( filter == NULL )
	{
		poles = 0;
		return( -1 );
	}
	for( i = 0; i < poles / 2 * STACKSIZE; i++ )
		filter[ i ] = FP0;
	flevel = FHACKLEVEL;
	return( 0 );
}

long 
VCF::Quit( void )
{
	if( filter != NULL )
		free( filter );
	poles = 0;
	return( 0 );
}

r_float 
VCF::Process( r_float inp, r_float env, r_float cutoff, r_float reso )
{
	int fidx;
	r_float a, b, c;

	if( !poles )
		return( inp );

	cutoff = r_add( cutoff, env );
	if( r_cmpable( cutoff ) > r_cmpable( FP0_5 ))
		cutoff = FP0_5;	/* Nyquist strikes again */

	if( type == FILT_HAMRE )
		cutoff = r_mul( r_sin( r_mul( PI, cutoff )), FP2 );
	else
	{
		a = r_div( r_neg( r_mul( PI, cutoff )), reso );
		b = r_neg( r_exp( r_mul( FP2, a )));
		a = r_exp( a );
		a = r_mul( r_mul( FP2, r_cos( r_mul( TWO_PI, cutoff ))), a );
		c = r_sub( r_sub( FP1, a ), b );
	}
	
	for( fidx = 0; fidx < (int)( poles / 2 * STACKSIZE ); fidx += STACKSIZE )
	{
		if( type == FILT_HAMRE )
		{
			inp = IterateH( &filter[ fidx ], &inp, cutoff, reso );
		}
		else
			inp = Iterate2( &filter[ fidx ], &inp, a, b, c );
	}
	return( inp );
	
}

r_float 
VCF::IterateH( r_float *stack, const r_float *in, const r_float freq,
						const r_float reso )
{
	r_float *low = &stack[ 0 ], *mid = &stack[ 1 ],
		*hig = &stack[ 2 ], /* *rej = &stack[ 3 ], */
		feedback;

	feedback = r_mul( reso, *mid );

	/**** Useless (?) hack to simulate diode limiting of resonance ****/
	
#if 0
	if( r_cmpable( r_fabs( feedback )) > r_cmpable( flevel ))
	{
		r_float sq;
		sq = r_mul( r_sub( feedback, flevel ), FP2 );
		sq = r_mul( sq, sq );
		
		if( r_sign( feedback ))
			sq = r_neg( sq );
		feedback = r_add( feedback, sq );
	}
#endif
	
	/******************************************************************/

	*hig = r_sub( r_sub( *in, feedback ), *low );
	*mid = r_add( *mid, r_mul( freq, *hig ));
	*low = r_add( *low, r_mul( freq, *mid ));
	/*	*rej = r_add( *hig, *low ); */

	return( *low );
}


r_float
VCF::Iterate2( r_float *stack, const r_float *in,
						 const r_float a, const r_float b, const r_float c )
{
	r_float *d1 = &stack[ 0 ], *d2 = &stack[ 1 ];
	r_float out;

	out = r_add( r_add( r_mul( a, *d1 ), r_mul( b, *d2 )), r_mul( c, *in ));
	*d2 = *d1;
	*d1 = out;
	return( out );
}