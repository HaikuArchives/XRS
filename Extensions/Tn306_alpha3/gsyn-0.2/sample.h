// $Id: sample.h,v 1.3 1998/11/08 07:28:00 andude Exp $
// $Log: sample.h,v $
// Revision 1.3  1998/11/08 07:28:00  andude
// Added delay-reverb and distortion.  Very, very cool sound.  Some tuning on
// resonance still needs doing, and perhaps distortion could use some fine
// tuning as well.  Stereo effects aren't implemented yet but wouldn't be
// difficult to add.
//
// Revision 1.2  1998/11/07 22:43:15  andude
// Massive overhaul in internal design.  Components are now modular and each
// module acts on a whole buffer at a time.  Overhaul is still in progress;
// Next on the list is the 303 filter, the sequencer, and the amplifier.  Then
// delay (quasi-reverb) and distortion will be added.
//
// Revision 1.1  1998/11/06 05:17:59  andude
// Added sampled impulse response from Rebirth RB-338.
//

#ifndef __SAMPLE_H
#define __SAMPLE_H

#include "synel.h"

class Sample: public Element
{
private:
	float *data;
	unsigned long len, retrig;
	unsigned long pos;

	SampleBuf out;
public:
	Sample() { data=NULL; len=0; pos=0; retrig=65536; }
	Sample(float *d, int l) { len=0; pos=0; data=d; len=l<<16; retrig=len; }

	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
};

#endif

