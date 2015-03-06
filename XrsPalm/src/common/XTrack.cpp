//Version 3 -- for XRS 1.2 powerUI


#include "XTrack.h"
#include "InterfaceKit.h"
#include "JMessages.h"

#define	XRS_SIMPLE_DATA	'xrsd'
#define	XRS_BANK_SAMPLE	'xbks'


//#define MSG_NN	

#include "stdio.h"

const 	rgb_color  	onc 	={255,52,49};//{81,110,117,255};
const 	rgb_color  	offc 	={57,186,255};//{107,146,156,255}; 

XTrack::XTrack(BRect rect,BBitmap *a,BBitmap*b,const char *t): BView(rect,"_xtrack",B_FOLLOW_NONE,B_WILL_DRAW)
{
	SetName(t);
	PicOn=a;
	PicOff=b;
	selected=false;	
	SetViewColor(B_TRANSPARENT_COLOR);
	SetFontSize(12);
	on=onc;
	off=offc;
	fra=rect;
	
}
void
XTrack::SetColors( rgb_color a,rgb_color b)
{
	on=a;
	off=b;
}

void
XTrack::Draw(BRect r)
{

		
	if(PicOn!=NULL && PicOff!=NULL)
	{
	   if(selected){
		DrawBitmapAsync(PicOn);
		//SetLowColor(81,110,117);
		SetLowColor(on);
		SetHighColor(0,0,0,255);
		DrawString((const char*)name,15,BPoint(8,18));
		
		SetHighColor(255,255,205);
		}
	   else
	   	{
	   	DrawBitmapAsync(PicOff);
	   
	   	//SetLowColor(107,146,156);
	   	SetLowColor(off);
	   	SetHighColor(42,42,42);
	   	
		}
	}
	
	DrawString((const char*)name,15,BPoint(8,18));
	
}
void
XTrack::Init(BView *t,BMessage *m)
{
	msg=m;
	msg->AddInt32("mouse",0);
	target=t;
	
}
void 
XTrack::MouseDown(BPoint where)
{
	int32 key;
	uint32 buttons;
	BMessage *m=Window()->CurrentMessage();
	 m->FindInt32("modifiers",&key);
	 
	 GetMouse(&where, &buttons);
	
	msg->ReplaceInt32("mouse",buttons);
	
	// B_SECONDARY_MOUSE_BUTTON
		 
	if(key & B_CONTROL_KEY) {
	
		BRect n(ConvertToScreen(Bounds()));
		//printf("rect %f %f -> %f %f\n",Bounds().left,Bounds().top,n.left,n.top);
		m=new BMessage('ttt');
		m->AddPoint("from",BPoint(n.left,n.top));
		m->AddPoint("to",BPoint(n.right,n.bottom));
		//new TextControlFloater(,B_ALIGN_LEFT,be_plain_font,(const char*)name,Parent(),new BMessage(JMSG_NAME_SET),new BMessage(JMSG_NAME_NOTSET));
		Looper()->PostMessage(m,Parent());
		return;
	 	
	}
	
	//m->ReplaceInt32("modifiers",key);
	
	//if(!selected)
		Looper()->PostMessage(msg,target);
			
}
void
XTrack::Select()
{
	if(Window()->Lock())
	{
		selected=true;
		Invalidate();
	Window()->Unlock();
	}	
}
void
XTrack::Deselect()
{
	if(Window()->Lock())
	{
		selected=false;
		Invalidate();
		Window()->Unlock();
	}
}
bool
XTrack::isSelected()
{
	return selected;
}
void
XTrack::SetName(const char *t)
{
	//sprintf(name,"%s",t);
	name=t;
	// printf("name %s %s\n",this->name,t);
	Invalidate();	
}
void
XTrack::MessageReceived(BMessage* message)
{

	
		
	switch(message->what)
	{


	case B_SIMPLE_DATA:
	
		message->what=XRS_SIMPLE_DATA;
	
	case XRS_BANK_SAMPLE:
		
		Window()->PostMessage(message,Parent());
	
	break;
	default:
		
		BView::MessageReceived(message);
		
		break;
	}

}