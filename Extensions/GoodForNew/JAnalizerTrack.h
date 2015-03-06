#ifndef _JMIDIOUT_TRACK_
#define _JMIDIOUT_TRACK_

#include "View.h"
#include "JTrack.h"

class	Pattern;
class	Track;
class	BPot;
class 	TBWindow;
class 	XNotesView;
class PatControl;


class JAnalizerTrack: public JTrack
{
	public:
			JAnalizerTrack(BRect,int16,BView*);
			~JAnalizerTrack();
		virtual void	Init(BView *t,BMessage *m);
		virtual void	RControl();
		virtual void	Reset(Pattern*,Track*);
		virtual int	getModel();
		virtual void	MessageReceived(BMessage*);
	
	private:
			PatControl *channel;
					

};

#endif