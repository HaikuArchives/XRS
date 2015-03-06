// Distortion and reverb.
// $Log: deverb.cpp,v $
// Revision 1.5  1998/11/15 02:21:30  andude
// Stupid warnings from gcc have been eliminated.
//
// Revision 1.4  1998/11/15 02:02:41  andude
// It turns out that ReBirth _does_ use the sawtooth.  And now so do I.  My
// resonance knob is tuned more accurately now, but I question envmod and
// cutoff's ranges.  Anyway, seems to sound much better now.
//
// Revision 1.3  1998/11/14 16:41:33  andude
// Made some lookup table optimizations in the VCF and distortion units.
// synth now runs at 10.8% CPU on my P166MMX when no knobs are being turned.
// No more sin/cos/exp for every sample!
//
// Revision 1.2  1998/11/08 11:50:29  andude
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

#include "deverb.h"
#include"stdio.h"
#include <math.h>

Deverb::Deverb()
{
	Vol(0);
	Feedback(0);
	Distort(0);
	dlen=4096; dpos=0;
	wavemag=0;
	float *b = delaybuf.GetBuf(dlen);
	memset(b, 0, 4096*sizeof(float));
	calc_disttbl();
}

Deverb::~Deverb()
{
}

void Deverb::Process(const float *buf, int len, int channels)
{
	if(!output) return;

//	if(!enable) {
//		output->Process(buf, len, channels);
//		return;
//	}
	float *dbuf = delaybuf.GetBuf(dlen);
	float *out = outbuf.GetBuf(len);
	float *dtbl = distbuf.GetBuf();
	float xi;
	int i, dind = dpos-1;
	if(dind<0) dind+=dlen;

	wavemag = 8000+(1000/(dist+0.1));
	
		
	for(i=0;i<len;i++) {
		xi = buf[i];
		if(xi>32767) xi=32767;
		else if(xi<-32767) xi=-32767;
		xi += dist*dtbl[(long) (32768.0+xi)];
		
		if(xi > wavemag) xi=wavemag;
		if(xi < -wavemag) xi=-wavemag;

		out[i] = dbuf[dpos] + xi;
		dbuf[dind++]=amt*xi + feedbk*dbuf[dpos++];
		if(dind>=dlen) dind-=dlen;
		if(dpos>=dlen) dpos-=dlen;
	}
	
	
	output->Process(out, len, channels);
}

void Deverb::Message(SynthMessage msg, float data)
{
	switch(msg)
	{
		case TempoChange:
			data*=6.0;
			if((int)data > dlen) {
				float *b = delaybuf.GetBuf((int)data);
				memset(b+dlen, 0, (((int)data)-dlen) * sizeof(float));
			}
			dlen = (int) data;
			break;
		case SystemReset:
			
			outbuf.Reset();
			delaybuf.Reset();
					
			break;
			
		default: break; // i'm sick of that warning.
	}
	
	if(output) output->Message(msg, data);
}

void Deverb::Distort(int d)
{
	dist=d*3.0;
}

void Deverb::calc_disttbl()
{
	float *dbuf = distbuf.GetBuf(65536);
	float xi;
	int i;

	for(i=0;i<65536;i++) {
		xi = i-32768;
		xi = sin(xi*M_PI/32768.0);
		dbuf[i] = xi;
	}
}
float *
Deverb::getBuffer(int len)
{
	return outbuf.GetBuf(len);
}
