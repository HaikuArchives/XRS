// $Log: vca303.cpp,v $
// Revision 1.8  1998/11/17 21:08:49  andude
// Oops.  if(mode=1)?  No.
//
// Revision 1.7  1998/11/17 18:08:49  andude
// Some more optimizations to the VCA.
//
// Revision 1.6  1998/11/17 16:53:22  andude
// Eliminated clicking in the VCA stage which showed up when resonance was
// high but cutoff/envmod were low.
//
// Revision 1.5  1998/11/16 23:31:54  andude
// Underflow exceptions were slowing the SGI down a lot.  They've been
// eliminated here.
//
// Revision 1.4  1998/11/16 23:13:49  andude
// I don't know how it worked before, but the bigendian swapping code for
// saving and loading patterns is fixed.  Also, optimized the VCA to get
// around a strange MIPS FPUism which caused it to be really slow while notes
// were in the attack phase for a relatively long time.
//
// Revision 1.3  1998/11/15 02:21:32  andude
// Stupid warnings from gcc have been eliminated.
//
// Revision 1.2  1998/11/08 07:28:06  andude
// Added delay-reverb and distortion.  Very, very cool sound.  Some tuning on
// resonance still needs doing, and perhaps distortion could use some fine
// tuning as well.  Stereo effects aren't implemented yet but wouldn't be
// difficult to add.
//
// Revision 1.1  1998/11/08 03:40:29  andude
// Finished internal changes.  ubersyn.cpp is deprecated.
//

#include "vca303.h"

VCA303::VCA303()
{ 
	mode=0; 
	a=0; 
	attack=1.0-0.94406088; 
	decay=0.99897516; 
	a0=0.5;
}

VCA303::~VCA303()
{}

void VCA303::Process(const float *buf, int len, int channels)
{
	float *out = sbuf.GetBuf(len);
	int i;
	if(!output) return;
	len*=channels;
	for(i=0;i<len;i++) {
		out[i] = buf[i]*a;
		if(!mode) a+=(a0-a)*attack;
		else if(mode==1) {
			a*=decay;
			if(a<(1/65536.0)) { a=0; mode=2; } // prevent underflow exceptions
		}
	}
	
	output->Process(out, len, channels);
}

void VCA303::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case NoteOn:
			mode=0;
			break;
		case NoteOff:
			mode=1;
			break;
		case SystemReset:
			sbuf.Reset();
			break;
		default: break; // gcc is dumn
	}

	if(output) output->Message(msg, data);
}

