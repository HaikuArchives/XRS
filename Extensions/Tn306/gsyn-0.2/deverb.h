// delay reverb type stuff.
// $Log: deverb.h,v $
// Revision 1.4  1998/12/05 20:56:11  andude
// Beginning of gtk+ conversion.  A knob widget and a set of seven knobs which
// control a 303 synthesizer have been added.  An additional Sun audio
// buffering hack (from Mattias Engdegård) is also in here.
//
// Revision 1.3  1998/11/14 16:41:34  andude
// Made some lookup table optimizations in the VCF and distortion units.
// synth now runs at 10.8% CPU on my P166MMX when no knobs are being turned.
// No more sin/cos/exp for every sample!
//
// Revision 1.2  1998/11/08 11:50:31  andude
// The sawtooth is still very wrong, I just discovered.  It looks like a
// lowpass-filtered sawtooth is what's supposed to happen but my filter isn't
// producing "authentic" results.  Either the resonant frequency isn't at the
// cutoff on ReBirth, or there's a second filter.
//
// Also, distortion is now harmonic instead of just overdriven.  I could add a
// "shape" knob, pretty simply.
//
// Revision 1.1  1998/11/08 07:27:55  andude
// Added delay-reverb and distortion.  Very, very cool sound.  Some tuning on
// resonance still needs doing, and perhaps distortion could use some fine
// tuning as well.  Stereo effects aren't implemented yet but wouldn't be
// difficult to add.
//

#ifndef __DEVERB_H
#define __DEVERB_H

#include "synel.h"

class Deverb: public Element
{
private:
	SampleBuf delaybuf, outbuf;
	SampleBuf distbuf;
	float amt, feedbk, dist;
	// lowpass filter for determining wave magnitude
	float wavemag;

	int dlen, dpos;
public:
	Deverb();
	virtual ~Deverb();

	void Distort(int d);
	float getDistort() { return dist;}
	void Vol(float v) { amt=v; }
	float getVol() { return amt;}
	void Feedback(float f) { feedbk=f; }
	float getFeedback() { return feedbk;}
	
	float * getBuffer(int len);
	
	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
private:
	void calc_disttbl();
};

#endif

