

#ifndef __DSPOUT_H
#define __DSPOUT_H


#include "synel.h"

class DSPOut: public Element
{
public:

// 16bit is assumed.
	DSPOut(int hz, int chan, int buf=8192);
	virtual ~DSPOut();


	void	Reset();

	void BeginPlayback();
	void StopPlayback();
	int bufsize;
	short* outbuf;

// Every time the buffer needs refilling, this event is supposed to fill
// the output buffer with size samples.  In the synthesizer environment,
// just call Process on the sequencers at the top of the chain.
	virtual void SoundEvent(int size);
	short*	getBuffer();
// The DSPOut Process function blocks, so it is meant to be called from
// the thread spawned by the DSPOut constructor.  That thread calls
// SoundEvent repeatedly so that derived classes can call Process on all
// of the synthesizer elements so that the chain eventually reaches back
// to DSPOut.  Thus all sound processing is done in the second thread.
	virtual void Process(const float *buf, int len, int channels);
};

#endif
