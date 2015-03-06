
#include "SpacerView.h"

#include "Window.h"
#include "stdio.h"

#define NOTE_VOL 'nvo';

const 	rgb_color 	blue 	={118,167,255,255};

rgb_color gray = {199,199,199,255};

SpacerView::SpacerView(BRect r,int32 min,int32 max,int32 center,BMessage *msg,BBitmap *bar) :
	BControl(r,"SpacerView","none",NULL,B_FOLLOW_NONE,B_WILL_DRAW)
{
	SetViewColor(gray);
//	SetHighColor(255,255,255);
	mybar=bar;
	esc=abs(max)-abs(min);
	scale=(float)(Bounds().bottom-Bounds().top);
	div=scale/(float)esc;
	isOn=false;
	notify=msg;
	isTracking=false;
	
	value.Set(r.right,r.bottom-6);
	base.Set(0,r.bottom);
	SetViewColor(189,186,115);
}
void SpacerView::Draw(BRect area)
{
	
	value.Set(Bounds().right,value.y);
	if(isOn) DrawBitmap(mybar,BRect( BPoint(0,value.y), BPoint(16,60) )    ,BRect(BPoint(0,value.y),BPoint(16,60)) );
		
}
void
SpacerView::MouseDown(BPoint pua)
{
		
	uint32 buttons;
	GetMouse(&pua, &buttons);
	
	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		if ( Bounds().Contains(pua)) {
			
			
			isTracking=true;
			
			SetMouseEventMask(B_POINTER_EVENTS,
				B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
				
			MakeChanges(pua);
		}
	}

		
}
void
SpacerView::MakeChanges(BPoint p)
{
	value=p;
	Invalidate();
	if(notify!=NULL)
	Window()->PostMessage(notify,Parent());
} 	
void
SpacerView::MouseUp(BPoint pua)
{
isTracking=false;
		

	
	//Window()->PostMessage(rel_msg);
		
}
void
SpacerView::MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg)
{
	if (isTracking && ( where.y>=0 && where.y <=60)) {
		MakeChanges(where);
		//printf("%f\n",where.y);
	}	
}
void 
SpacerView::SetValue(int32 val,bool noz)
{
		value.y=scale-(float)val*(float)div;
		
		isOn=noz;
		Invalidate();
				
}
void
SpacerView::SetIsOn(bool noz)
{
	isOn=noz;
	Invalidate();
}
bool
SpacerView::IsOn()
{
	return isOn;
}
int32
SpacerView::Value()
{
		return (int32)((scale-value.y)/div);
}
void
SpacerView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}
