
#ifndef S_WINDOW_H
#define S_WINDOW_H
 
#include <Window.h>
#include "SamplerTrackBoost.h"


class StringView;

class Sample;
class SamplerTrack;

class FxWindow:public BWindow
{



 public:
				FxWindow(SamplerTrackBoost*);
	virtual void		MessageReceived(BMessage* message);
	virtual bool		QuitRequested();	
	void			SetTrack(SamplerTrack*);	
	
	
private:

	
	Sample				*my_sample;
	char txt[30];
	BStringView 			*text_info;
	SamplerTrackBoost		*booster;
	SamplerTrack			*curTrack;
	
};

#endif