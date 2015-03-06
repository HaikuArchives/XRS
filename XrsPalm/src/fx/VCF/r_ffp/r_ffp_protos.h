#ifndef R_FFP_PROTOS_H
#define R_FFP_PROTOS_H 1

/* r_ffp_protos.h */

#ifdef _DCC
/* what me silly? */
#include "r_ffp/r_ffp.h"
#include "r_ffp/types.h"
#else
#include "r_ffp.h"
#include "types.h"
#endif

extern long r_ftoa( r_float num, STRPTR dec, unsigned char pts );
extern long r_rndftoa( r_float f, STRPTR dest, unsigned char pts );
extern STRPTR r_sciftoa( STRPTR dest, r_float f, unsigned char nums );
extern STRPTR r_numftoa( STRPTR dest, r_float f, unsigned char nums );
extern r_float r_atof( STRPTR asc );

#endif
