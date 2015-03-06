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
class AnalizerTrack;
class SampleView;

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
		
		virtual	void	Select();
		virtual	void	Deselect();
	protected:
	friend AnalizerTrack;
			void DrawBuffer(float** buffer,int32 samples_num,int spiaz);
	
	private:
			PatControl *channel;
			SampleView*	sw;
					

};

#endif