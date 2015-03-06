
#ifndef _TBWINDOW_H
#define _TBWINDOW_H

#include "Window.h"


class TB303Knobs;
class SpecialTrack; 

class TBWindow : public BWindow
{
	public:
	
			TBWindow();
	virtual bool	QuitRequested();
	virtual void   FrameMoved(BPoint n);
		void	Reset(SpecialTrack *s);
		
		
	private:

			TB303Knobs	*knobs;
			SpecialTrack*	st;
};
#endif

