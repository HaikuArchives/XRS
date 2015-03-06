#include "Meter.h"
#include "Message.h"
#include "stdio.h"
#include "math.h"

Meter::Meter(	BRect	frame):BView(frame,  "_meter",  B_FOLLOW_NONE,B_WILL_DRAW) {
		
		value=0;
		current=0;
		SetViewColor(B_TRANSPARENT_COLOR);
}
		
void Meter:: Draw(BRect k)	{
		
		BRect r(Bounds());
		SetHighColor(0,0,0);
		FillRect(r);
		r.InsetBy(1,1);
		
		if(value>0.9)	// clip?
			SetHighColor(255,0,0);
		else
			SetHighColor(0,255,0);
		
		r.top = r.bottom - r.Height()*value;
		FillRect(r);
}
	
	
void Meter::MessageReceived(BMessage *message){	
		switch(message->what){
		
		case	ASYNC:
			value=fabs(message->FindFloat("value"));
			Invalidate();
		break;
		default:
			_inherited::MessageReceived(message);
		break;
		}
}
