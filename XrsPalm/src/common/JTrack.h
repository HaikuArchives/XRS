#ifndef _J_TRACK_
#define _J_TRACK_

#include "View.h"

class	Pattern;
#include "XNotesView.h"
#include "GraphicsDefs.h"
class 	XTrack;
class	Track;
class	BPopUpMenu;
class	BPot;

#include <PictureButton.h>

#define	XRS_SIMPLE_DATA	'xrsd'

//enum{
//	JTRACK_VOLUME,
//	JTRACK_NOTE,
//	JTRACK_MENU,
//	JTRACK_MIDI,
//	JTRACK_VST
//};

#define	JTRACK_VOLUME	1 //00000001b
#define	JTRACK_NOTE		2 //00000010b
#define	JTRACK_MENU	4 //00000100b
#define	JTRACK_VST		8 //00001000b
#define	JTRACK_MIDI		3 //00000011b

class JTrack:public BView
{
	public:
				JTrack(BRect,int16,BView*,uint32 type=7,const char*n="TrackNameOff");
		virtual void	Init(BView *t,BMessage *m);
		virtual void	RControl();
		virtual void	Reset(Pattern*,Track*);
		virtual void	Refresh(Pattern*);
		virtual void	Select();
		virtual void	Deselect();
		virtual int	getModel();
		void		Mute(bool,bool);
		//virtual 	BPopUpMenu*	getMenu();

		virtual void	SetName(const char*);
		virtual void	MessageReceived(BMessage*);

			Track*	getTrack() {return myTrack;};
	protected:
			BView*			tv;
			BPictureButton*	button_fx1;
			BPictureButton*	button_fx_line;
			Pattern*		myPat;
			Track*			myTrack;
			BPot*			volpot;
			BPot*			panpot;
			XNotesView*		vnc;
			XTrack*		xtr;
			int16	id;

			BPictureButton	*trackmenu;
			uint32			flags;

				void	ShowMenu();
	private:


			const char*			name_pad;
			BPictureButton	*button_mode;
			BPictureButton 	*button_piano;
			BPopUpMenu	*popup;


};

#endif
