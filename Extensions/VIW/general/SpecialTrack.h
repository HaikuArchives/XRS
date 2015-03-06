#ifndef _SPECIAL_TRACK_H_
#define _SPECIAL_TRACK_H_

#include "Track.h"
#include "Pattern.h"

class  VSTInstrumentPlugin;
class	PlugWindow;
class  JSpecialTrack;

class SpecialTrack: public Track
{
	public:
	
				SpecialTrack(const char * path);
				~SpecialTrack();
	virtual 	void		Reset();
	virtual	void		Process(float**,int32,int);
	virtual	void		NoteReady(int,int,float);
	virtual	int		getModel();
	virtual	void		goOn();
	virtual	void		goOff();
	virtual 	const char*	getPreferredName();	

		VSTInstrumentPlugin *viw;
		PlugWindow*		win;
		BMessage*		settings;
private:
		
		Pattern*	playingPattern;	
			virtual void Message(SynthMessage msg, float data);
		float *stream_s[2];
		float *stream_l;
		float *stream_r;

		
};

#endif