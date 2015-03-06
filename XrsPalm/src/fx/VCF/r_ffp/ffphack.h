/* ffphack.h */

#ifndef FFPHACK_H
#define FFPHACK_H 1

/* omat FFP-'numerot' */

#define ILLEGAL_FFP		0x40000000		/* illegal FFP number mask	*/

#define FFP_OVERFLOW	( ILLEGAL_FFP | 0x000100 )
#define FFP_UNDERFLOW	( ILLEGAL_FFP | 0x000200 )
#define FFP_INFINITY	( ILLEGAL_FFP | 0x000300 )	/* infinity mask	*/
#define FFP_PINFINITY	( ILLEGAL_FFP | 0x00037F )	/* positive infinity	*/
#define FFP_NINFINITY	( ILLEGAL_FFP | 0x000380 )	/* negative infinity	*/
#define FFP_ERROR		( ILLEGAL_FFP | 0x800000 )


#define ISLEGALFFP(f)	(((f)==0L)||((f)>=0x80000000L))


#endif
