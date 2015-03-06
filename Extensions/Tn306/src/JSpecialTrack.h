#ifndef _JSPECIAL_TRACK_
#define _JSPECIAL_TRACK_

#include "View.h"
#include "JTrack.h"

class	Pattern;
class	Track;
class	BPot;
//class 	TBWindow;
class PlugWindow;
class TB303Knobs;
class 	XNotesView;
class Meter;


class JSpecialTrack: public JTrack
{
	public:
			JSpecialTrack(BRect,int16,BView*);
			~JSpecialTrack();
		//virtual void	Init(BView *t,BMessage *m);
		virtual void	RControl();
		virtual void	Reset(Pattern*,Track*);
		virtual void	Refresh(Pattern*);
		virtual void	Select();
		virtual void	Deselect();
		virtual int	getModel();
		
		
		virtual void	SetName(const char*);
		virtual void	MessageReceived(BMessage*);
	
			void	SetMeter(float,float);
			void	GetConfig(BMessage* m);
			void	SetConfig(BMessage* m);
	private:
		
			//TBWindow *tbw;
			TB303Knobs*	knobs;
			PlugWindow*	win;
			Meter*		meter;
			Meter*		meter2;
			BMessage	*m_async;
					

};

#endif