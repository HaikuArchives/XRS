
#ifndef _X_TRACK_
#define  _X_TRACK_

#include "View.h"

class XTrack : public BView
{
	public:
			XTrack (BRect,BBitmap*,BBitmap*,const char*);

	virtual void Draw(BRect);	
	virtual void MouseDown(BPoint);
	virtual void MessageReceived(BMessage*);
		 void 	Init(BView*,BMessage*);
		 void	Select();
		 void	Deselect();
		 bool	isSelected();
		void	SetName(const char *t);
		void	SetColors( rgb_color,rgb_color);
	private:

		BBitmap	*PicOn;
		BBitmap	*PicOff;
		
		rgb_color on;
		rgb_color off;
	
		bool	selected;
	
		BMessage	*msg;
		BView		*target;
		BRect		fra;
		//char		name[100]; 
		const char*name;
};
#endif

