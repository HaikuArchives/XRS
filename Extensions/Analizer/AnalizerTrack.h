#ifndef _MIDIOUT_TRACK_H_
#define _MIDIOUT_TRACK_H_

#include "Track.h"

class JAnalizerTrack;


class AnalizerTrack: public Track
{
	public:
	
				AnalizerTrack();
				~AnalizerTrack();
virtual 	void		Reset();
virtual	void		Process(float**,int32,int);
virtual	void		NoteReady(int,int,float);
virtual	int		getModel();
virtual	void		goOn();
virtual	void		goOff();
virtual 	void 		Message(SynthMessage msg, float data);

	void SetChannel(int n);

protected:
	friend JAnalizerTrack;
		JAnalizerTrack*	jat;

private:
			

	Pattern*	playingPattern;	
	Note*		curNote;
	
	
	
	int channel;
	time_t latency;

};

#endif