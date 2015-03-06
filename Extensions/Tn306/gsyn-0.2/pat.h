// $Id: pat.h,v 1.2 1998/11/02 16:51:52 andude Exp $
// $Log: pat.h,v $
// Revision 1.2  1998/11/02 16:51:52  andude
// Added ID and Log tags.
//

#ifndef __PAT_H
#define __PAT_H

// oct actually contains:
// accent (bit 3), slide (bit 2), and octave (bits 1-0)
struct Pattern
{
	int note[16];
	int oct[16];
};

#endif

