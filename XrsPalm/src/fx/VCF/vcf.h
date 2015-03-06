#ifndef _VCF_H_
#define _VCF_H_

#include <stdio.h>

//#include "3MU.h"
#include "r_ffp/r_ffp.h"


#ifdef _DCC
	#define FHACKLEVEL afp( "0.01" )
#else
	#define FHACKLEVEL (0.01)
#endif

#define	FEEDBACK 4
#define 	STACKSIZE 5


class VCF{

	public :
	
			VCF();
		long 	Init( char VCFpoles, char VCFtype );
		long	Quit();
		r_float	Process( r_float inp, r_float env, r_float cutoff, r_float reso );
			
	private:
	
		r_float	IterateH( r_float *stack, const r_float *in, const r_float freq,
						const r_float reso );
						
		r_float Iterate2( r_float *stack, const r_float *in,
						 const r_float a, const r_float b, const r_float c );
	
	r_float *filter ;
	unsigned char type, poles;
	r_float flevel;					 	
	
};
#endif