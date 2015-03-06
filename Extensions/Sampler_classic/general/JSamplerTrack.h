#ifndef _JSAMPLER_TRACK_
#define _JSAMPLER_TRACK_

#include "JTrack.h"
#include "Window.h"

class 	XNotesView;
class 	XTrack;
class	BPopUpMenu;
class	 SamplerTrackBoost;
class	SamplerTrack;

class JSamplerTrack:public JTrack
{
	public:
			JSamplerTrack(BRect,int16,BView*,SamplerTrackBoost* );
			SamplerTrackBoost*	boost;
			
		virtual void 	Select();	
		virtual void	Deselect();
		virtual void	MessageReceived(BMessage*);
		//virtual void	RControl();
		virtual void	Reset(Pattern* p ,Track* tr);
		BWindow*	target;
		
	private:
		//BPictureButton*	button_rev;
		SamplerTrack*		mySamplerTrack;
};

#endif