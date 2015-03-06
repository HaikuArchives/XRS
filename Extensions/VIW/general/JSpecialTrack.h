#ifndef _JSPECIAL_TRACK_
#define _JSPECIAL_TRACK_

#include "JTrack.h"

class  PlugWindow;
class VSTInstrumentPlugin;
class SamplerTrackBoost;

class JSpecialTrack: public JTrack
{
	public:
			JSpecialTrack(BRect,int16,BView*);
			~JSpecialTrack();
		virtual void	MessageReceived(BMessage*);
		virtual void	Reset(Pattern*,Track*);
		virtual int	getModel(){	return 10; }
		virtual void	SetName(const char*);
	

	private:
		
			VSTInstrumentPlugin	*instrument;
};

#endif