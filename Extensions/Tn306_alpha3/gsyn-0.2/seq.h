// 16-note Sequencer (typically for 303) class
//
// $Log: seq.h,v $
// Revision 1.5  1998/12/05 20:56:15  andude
// Beginning of gtk+ conversion.  A knob widget and a set of seven knobs which
// control a 303 synthesizer have been added.  An additional Sun audio
// buffering hack (from Mattias Engdegård) is also in here.
//
// Revision 1.4  1998/11/13 06:06:32  andude
// Added insert and delete key functions and '.' key which clears the current
// row and advances to the next one.
//
// Made use of fwt's new timer mechanism to update LEDs during playback.
//
// Revision 1.3  1998/11/12 23:50:46  andude
// Some fixes to voxware/OSS driver buffer sizing.
//
// Resonance tuning in vcf303.
//
// Sequencer doesn't change patterns while playing until the end of the
// current one.
//
// Revision 1.2  1998/11/08 11:50:35  andude
// The sawtooth is still very wrong, I just discovered.  It looks like a
// lowpass-filtered sawtooth is what's supposed to happen but my filter isn't
// producing "authentic" results.  Either the resonant frequency isn't at the
// cutoff on ReBirth, or there's a second filter.
//
// Also, distortion is now harmonic instead of just overdriven.  I could add a
// "shape" knob, pretty simply.
//
// Revision 1.1  1998/11/08 03:40:27  andude
// Finished internal changes.  ubersyn.cpp is deprecated.
//

#ifndef __SEQ_H
#define __SEQ_H

#include "synel.h"
#include "Pattern.h"

class Sequencer: public Element
{
private:
	Pattern *curpat, *nextpat;
	float sampsperbeat, beatpos;
	
	int tuneoffset;
	int enable;
	
	float new_sxp;

	void process_half_row();
public:
	

	Sequencer();
	virtual ~Sequencer();
	int row;
	void SetTempo(float bpm);  // Should be obvious.
	// Set tuning in halfsteps (more like transpose)
	void Tune(int halfsteps) { tuneoffset=halfsteps; }
	
	// Begin/end playback
	void Play(); 
	void Stop(); 
	void Reset();
	// Change patterns.  If playing, doesn't actually change until end of
	// currently playing pattern.
	void SetPattern(Pattern *p) {nextpat=p; }

	int IsEnabled(void) { return enable; }
	void Enable(int e);

	void ProposeTempo(float b);

	// Synth Element stuff
	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
};

#endif

