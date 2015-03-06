// 303 amplifier
// $Id: vca303.h,v 1.3 1998/11/17 18:08:49 andude Exp $
// $Log: vca303.h,v $
// Revision 1.3  1998/11/17 18:08:49  andude
// Some more optimizations to the VCA.
//
// Revision 1.2  1998/11/17 16:53:23  andude
// Eliminated clicking in the VCA stage which showed up when resonance was
// high but cutoff/envmod were low.
//
// Revision 1.1  1998/11/08 03:40:30  andude
// Finished internal changes.  ubersyn.cpp is deprecated.
//

#ifndef __AMP303_H
#define __AMP303_H

#include "synel.h"

class VCA303: public Element
{
private:
	float attack, decay, a0, a;
	int mode;

	SampleBuf sbuf;
public:
	VCA303();
	virtual ~VCA303();

	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
	
};

#endif

