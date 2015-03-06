// vcf303.h: "Voltage-Controlled" Filter simulating a Roland TB-303
// $Log: vcf303.h,v $
// Revision 1.3  1998/11/14 16:41:36  andude
// Made some lookup table optimizations in the VCF and distortion units.
// synth now runs at 10.8% CPU on my P166MMX when no knobs are being turned.
// No more sin/cos/exp for every sample!
//
// Revision 1.2  1998/11/08 03:40:31  andude
// Finished internal changes.  ubersyn.cpp is deprecated.
//
// Revision 1.1  1998/11/07 22:43:20  andude
// Massive overhaul in internal design.  Components are now modular and each
// module acts on a whole buffer at a time.  Overhaul is still in progress;
// Next on the list is the 303 filter, the sequencer, and the amplifier.  Then
// delay (quasi-reverb) and distortion will be added.
//

#ifndef __VCF303_H
#define __VCF303_H

#include "synel.h"

class VCF303: public Element
{
private:
	float cutoff, envmod, envdecay, reso;
	float e0, e1;
	float c0; // c0=e1 on retrigger; c0*=envdecay every sample; cutoff=e0+c0
	float d1,d2; // for filter innerloop

	SampleBuf sbuf;
	// filter parameter envelopes
	SampleBuf abuf, bbuf;
	int need_recalc;
	int envpos;
public:
	VCF303();
	virtual ~VCF303();

// Valid values here are in the 0-1 range (linear with knob setting)
	void Cutoff(float c);
	float getCutoff() { return cutoff;}
	void Resonance(float r);
	float getResonance() { return reso;}
	void Envmod(float e);
	float getEnvmod() { return envmod;}
	void Decay(float d);
	float getDecay() { return envdecay;}

	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
private:
	void recalc_env();
};

#endif

