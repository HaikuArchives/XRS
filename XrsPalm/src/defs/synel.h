// Synthesizer element
// $Id: synel.h,v 1.7 1998/11/16 23:13:48 andude Exp $
// $Log: synel.h,v $
// Revision 1.7  1998/11/16 23:13:48  andude
// I don't know how it worked before, but the bigendian swapping code for
// saving and loading patterns is fixed.  Also, optimized the VCA to get
// around a strange MIPS FPUism which caused it to be really slow while notes
// were in the attack phase for a relatively long time.
//
// Revision 1.6  1998/11/14 16:41:35  andude
// Made some lookup table optimizations in the VCF and distortion units.
// synth now runs at 10.8% CPU on my P166MMX when no knobs are being turned.
// No more sin/cos/exp for every sample!
//
// Revision 1.5  1998/11/10 07:26:38  raybondo
// Added windows port and made a  wdspout for windows specifics because it is
// fundamentally different than the un*x dspout stuff.
//
// Revision 1.4  1998/11/08 07:28:04  andude
// Added delay-reverb and distortion.  Very, very cool sound.  Some tuning on
// resonance still needs doing, and perhaps distortion could use some fine
// tuning as well.  Stereo effects aren't implemented yet but wouldn't be
// difficult to add.
//
// Revision 1.3  1998/11/07 22:43:18  andude
// Massive overhaul in internal design.  Components are now modular and each
// module acts on a whole buffer at a time.  Overhaul is still in progress;
// Next on the list is the 303 filter, the sequencer, and the amplifier.  Then
// delay (quasi-reverb) and distortion will be added.
//
// Revision 1.2  1998/11/02 16:51:55  andude
// Added ID and Log tags.
//

#ifndef __SYNTHELEMENT_H
#define __SYNTHELEMENT_H

#ifndef NO_NAMESPACES
#include <list>
using namespace std;
#else
#include <list.h>
#endif
#include"stdio.h"
// Has zero or more inputs, zero or one outputs, and zero or more parameters.
// Basis for:
//    tone/sample output
//    filter
//    distortion/compression
//    mixer/clipper
//    playback
//    graphic displays of any sort

// All calculations are floating-point.  Why?
// On the Pentium, floating point multiplies (which are used very, very 
// frequently here) are much faster than integer ones.  On top of that,
// all floating point operations work on memory (i.e. our output buffer).

class SampleBuf
{
private:
	float *buf;
	int alloclen;
public:
	SampleBuf() { buf=NULL; alloclen=0; }
	virtual ~SampleBuf();

	float *GetBuf(int size);
	float *GetBuf() { return buf; }
	void	Reset();
};

// Message type definitions
enum SynthMessage {
//  enum             description (arg type)
	TempoChange,	 // samples per sixteenth note
	NoteChange,  		// change pitch of next note (float freq)
	NoteOn,		 // trigger note (float velocity)
	NoteOff,	 	// release note (float aftertouch?)
	NoteSlide,	 // slide to next note (none)
	SystemReset,	// Make a Reset!
	SystemStop
};

class Element
{
protected:
	list<Element*> inputs;
	Element *output;

// These are called when another element sets output to this element.
// This is so mixers know just how many things they're mixing.
// The base class uses these for housekeeping in case the chain is broken
// in the middle by deleting an element which another element outputs to.
	virtual void AddInput(Element *in);
	virtual void RemoveInput(Element *in);
	virtual void RemoveOutput(void);
public:
	Element() { output = NULL; }
	virtual ~Element();
	
	void SetOutput(Element *out);

// Process does whatever processing the filter does.
// Channel samples are interleaved in the buffer.
// If channels>1, expect filters to only operate on first channel.
// Synthesizers with no input get a buf=NULL and len=desired output buffer
// size.
	virtual void Process(const float *buf, int len, int channels);
// Flush the output for additive elements (multi-input filters, mixers)
	virtual void Flush(void);
// Control messages sent from high-level elements and propagated through outputs.
	virtual void Message(SynthMessage msg, float data);


};

class Mixer: public Element
{
	SampleBuf mixbuf;
	int inputcycle, index;
public:
	Mixer() { inputcycle=0; index=0; }
	virtual ~Mixer();
	virtual void Reset();
	virtual void Process(const float *buf, int len, int channels);
	
	// Call this after the process from each input is called
	// i.e. 
	// inp1->Process(); mixer->CycleInput(); 
	// inp2->Process(); mixer->CycleInput();
	void CycleInput();
};

#endif
