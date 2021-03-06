// $Log: vco303.cpp,v $
// Revision 1.2  1998/11/15 02:02:43  andude
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

#include "vco303.h"

void VCO303::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case NoteChange:
			inc=14000.0*data/44100;
			break;
		case NoteOn:
			k=0;
			break;
		case SystemReset:
			out.Reset();
			break;
			
		default: break;
	}
	if(output) output->Message(msg,data);
}

void VCO303::Process(const float *buf, int len, int channels)
{
	float *outbuf = out.GetBuf(len);
	int i;
	
	
	//	SAWTOOTH
	for(i=0;i<len;i++) {
		outbuf[i]=k;
		k += inc;
		if(k>7000) k-=14000;
	}
	
			
	if(output) output->Process(outbuf, len, channels);
}

