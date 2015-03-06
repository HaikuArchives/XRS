// vcf303.cpp: "Voltage-Controlled" Filter simulating a Roland TB-303
// $Log: vcf303.cpp,v $
// Revision 1.10  1998/11/25 04:40:44  andude
// autoconf/autoheaderized in preparation for GPLing.  Added missing dial
// headings.
//
// Revision 1.9  1998/11/16 23:13:50  andude
// I don't know how it worked before, but the bigendian swapping code for
// saving and loading patterns is fixed.  Also, optimized the VCA to get
// around a strange MIPS FPUism which caused it to be really slow while notes
// were in the attack phase for a relatively long time.
//
// Revision 1.8  1998/11/15 02:21:33  andude
// Stupid warnings from gcc have been eliminated.
//
// Revision 1.7  1998/11/15 02:02:42  andude
// It turns out that ReBirth _does_ use the sawtooth.  And now so do I.  My
// resonance knob is tuned more accurately now, but I question envmod and
// cutoff's ranges.  Anyway, seems to sound much better now.
//
// Revision 1.6  1998/11/14 16:41:35  andude
// Made some lookup table optimizations in the VCF and distortion units.
// synth now runs at 10.8% CPU on my P166MMX when no knobs are being turned.
// No more sin/cos/exp for every sample!
//
// Revision 1.5  1998/11/12 23:50:47  andude
// Some fixes to voxware/OSS driver buffer sizing.
//
// Resonance tuning in vcf303.
//
// Sequencer doesn't change patterns while playing until the end of the
// current one.
//
// Revision 1.4  1998/11/09 05:58:01  andude
// Word!  Found correlation between resonance knob and filter envelope,
// implemented 303 sawtooth VCO.  Squarewave will be implemented later.
// Next: minislider control, tempo adjustment, delay amount (3, 4 beats), GUI
// timer to refacilitate weird blinking lights, and something else which I
// don't remember right now.
//
// Revision 1.3  1998/11/08 21:46:23  andude
// Added "fun with filters" program.
//
// Revision 1.2  1998/11/08 03:40:30  andude
// Finished internal changes.  ubersyn.cpp is deprecated.
//
// Revision 1.1  1998/11/07 22:43:20  andude
// Massive overhaul in internal design.  Components are now modular and each
// module acts on a whole buffer at a time.  Overhaul is still in progress;
// Next on the list is the 303 filter, the sequencer, and the amplifier.  Then
// delay (quasi-reverb) and distortion will be added.
//

#include <math.h>
#include "vcf303.h"

#define ENVINC 64
#define ENVBITS 6
#define ENVLEN 65536

VCF303::VCF303()
{
	envmod=0; cutoff=0;
	Cutoff(0);
	Resonance(0);
	Envmod(0);
	Decay(0);

	envpos=0;
	need_recalc=1;
	d1=d2=0;
}

VCF303::~VCF303()
{
}

void VCF303::Cutoff(float c)
{
	cutoff = c;
//	e1 = exp(6.109 + 1.5876*envmod + 2.1553*cutoff);
//	e0 = exp(5.613 - 0.8*envmod + 2.1553*cutoff);
// experimental:
//	e1 = exp(6.109 + 1.5876*envmod + 2.1553*cutoff - 3.02*(1.0-reso));
//	e0 = exp(5.613 - 0.8*envmod + 2.1553*cutoff - 2.86*(1.0-reso));
	e1 = exp(6.109 + 1.5876*envmod + 2.1553*cutoff - 1.2*(1.0-reso));
	e0 = exp(5.613 - 0.8*envmod + 2.1553*cutoff - 0.7696*(1.0-reso));
	e0*=M_PI/44100.0;
	e1*=M_PI/44100.0;
	e1 -= e0;
	need_recalc=1;
}

void VCF303::Resonance(float r)
{
	reso = r;
	Cutoff(cutoff);
}

void VCF303::Envmod(float e)
{
	envmod=e;
	Cutoff(cutoff);
}

void VCF303::Decay(float d)
{
	d = 0.2 + (2.3*d);
	d*=44100;
	envdecay = pow(0.1, 1.0/d);
	need_recalc=1;
}

// Only filters the left channel.  Pass single channels here and feed the
// output to panners/mixers for stereo effects.  Or instantiate two of them.
void VCF303::Process(const float *buf, int len, int channels)
{
	int i;
	long pos;

	if(need_recalc) recalc_env();

	float *a=abuf.GetBuf(), *b=bbuf.GetBuf();
	float c;
	float *out = sbuf.GetBuf(len/channels);
	
// This feedback low-pass filter comes from Musical Applications of
// Microprocessors by Hal Chamberlin.  a[] and b[] are calculated in
// recalc_env().
	for(i=0;i<len;i+=channels) {
		pos = envpos>>ENVBITS;
		c = 1 - a[pos] - b[pos];
		out[i] = a[pos]*d1 + b[pos]*d2 + c*buf[i];
		d2=d1; d1=out[i];
		if(envpos<(ENVLEN-1)) envpos++;
	}
	output->Process(out, len/channels, 1);
	// printf("loop! out[0]=%f\n",out[0]);
}

void VCF303::Message(SynthMessage msg, float data)
{
	switch(msg)
	{
		case NoteOn:
			envpos=0; // retrigger cutoff envelope
			break;
		case SystemReset:
			sbuf.Reset();
			break;
			
		default: break; // gcc is dumn
	}
	if(output) output->Message(msg, data);
}

void VCF303::recalc_env()
{
	float *a = abuf.GetBuf(ENVLEN>>ENVBITS);
	float *b = bbuf.GetBuf(ENVLEN>>ENVBITS);
	double decay16 = pow(envdecay, ENVINC);
	float whopping; // don't ask.
	float k;
	float r = exp(-1.20 + 3.455*reso);
	int i;

	c0=e1;
	for(i=0;i<(ENVLEN>>ENVBITS);i++) {
		whopping = e0+c0;
		k = exp(-whopping/r);
		c0*=decay16;

// This feedback low-pass filter comes from Musical Applications of
// Microprocessors by Hal Chamberlin.
		a[i] = 2.0*cos(2.0*whopping) * k;
		b[i] = -k*k;
	}
	need_recalc=0;
}



