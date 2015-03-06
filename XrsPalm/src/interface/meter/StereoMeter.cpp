#include "StereoMeter.h"
#include "Message.h"
#include "stdio.h"
#include "math.h"

StereoMeter::StereoMeter(	BRect	frame):BView(frame,  "_meter",  B_FOLLOW_NONE,B_WILL_DRAW) {
		
		value_l=0;
		value_r=0;
		current=0;
		SetViewColor(B_TRANSPARENT_COLOR);
}
		
void StereoMeter:: Draw(BRect k)	{
		
		BRect r(Bounds());
		SetHighColor(0,0,0);
		FillRect(r);
		r.InsetBy(1,1);
		
		//r.right +=1;
		r.left=(r.Width()/2)+1;
		
		if(value_l>0.9)	// clip?
			SetHighColor(255,0,0);
		else
			SetHighColor(0,255,0);
		
		r.top = r.bottom - r.Height()*value_l;
		FillRect(r);
		
		r=(Bounds());
		SetHighColor(0,0,0);
		//FillRect(r);
		r.InsetBy(1,1);
		
		r.right=r.Width()/2;
		
				
		if(value_r>0.9)	// clip?
			SetHighColor(0,255,0);
			
		else
			SetHighColor(255,0,0);
		
		r.top = r.bottom - r.Height()*value_r;
		FillRect(r);
}
	
	
void StereoMeter::MessageReceived(BMessage *message){	
		switch(message->what){
		
		case	ASYNC:
			value_l=fabs(message->FindFloat("value_l"));
			value_r=fabs(message->FindFloat("value_r"));
			Invalidate();
		break;
		default:
			_inherited::MessageReceived(message);
		break;
		}
}
