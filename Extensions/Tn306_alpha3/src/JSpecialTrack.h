#ifndef _JSPECIAL_TRACK_
#define _JSPECIAL_TRACK_

#include "View.h"
#include "JTrack.h"

class	Pattern;
class	Track;
class	BPot;
class 	TBWindow;
class 	XNotesView;


class JSpecialTrack: public JTrack
{
	public:
			JSpecialTrack(BRect,int16,BView*);
			~JSpecialTrack();
		//virtual void	Init(BView *t,BMessage *m);
		//virtual void	RControl();
		virtual void	Reset(Pattern*,Track*);
		virtual void	Refresh(Pattern*);
		virtual void	Select();
		virtual void	Deselect();
		virtual int	getModel();
		
		
		virtual void	SetName(const char*);
		virtual void	MessageReceived(BMessage*);
	
	private:
		
			TBWindow *tbw;
			//BPictureButton *f;	
			//BPictureButton *f2;
			//int id;
					

};

#endif