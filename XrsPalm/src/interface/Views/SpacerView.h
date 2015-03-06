
#ifndef SPACER_VIEW_H
#define SPACER_VIEW_H
 
#include <View.h>
#include <Rect.h>
#include <Control.h>

class SpacerView:public BControl
{



 public:
					SpacerView(BRect,int32,int32,int32,BMessage*,BBitmap*);
		
		
	//virtual void		MessageReceived(BMessage* message);
	//virtual bool		QuitRequested();	
	virtual void Draw(BRect b);
	virtual void MouseDown(BPoint);
	virtual void MouseUp(BPoint where);	
	virtual void MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
		
		void  SetValue(int32,bool);
		void  SetIsOn(bool);
		bool  IsOn();
	
	 virtual void AttachedToWindow();
	
	int32 Value();
private:

	void	MakeChanges(BPoint) ;
	
	BPoint	value;
	BPoint 	base;
	int32 esc;
	float	 div;
	float	 scale;
	BMessage* notify;
	BBitmap	*mybar;
	bool	isOn;
	bool	isTracking;
};

#endif