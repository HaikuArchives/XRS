// $Id: vco303.h,v 1.2 1998/11/15 02:02:44 andude Exp $
// $Log: vco303.h,v $
// Revision 1.2  1998/11/15 02:02:44  andude
// It turns out that ReBirth _does_ use the sawtooth.  And now so do I.  My
// resonance knob is tuned more accurately now, but I question envmod and
// cutoff's ranges.  Anyway, seems to sound much better now.
//
// Revision 1.1  1998/11/09 05:58:02  andude
// Word!  Found correlation between resonance knob and filter envelope,
// implemented 303 sawtooth VCO.  Squarewave will be implemented later.
// Next: minislider control, tempo adjustment, delay amount (3, 4 beats), GUI
// timer to refacilitate weird blinking lights, and something else which I
// don't remember right now.
//

#ifndef __VCO303_H
#define __VCO303_H

#include "synel.h"

class VCO303: public Element
{
private:
	float inc;
	float k;

	SampleBuf out;
public:
	VCO303() { inc=0; k=0; }

	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
	
};

#endif

