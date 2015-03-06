#include "XWindowTitle.h"
#include "Window.h"
#include "stdio.h"
#include "PictureButton.h"
#include "WU_AppUtils.h"
#include "Font.h"

#define	X_WINDOW_ZOOM	'xwzc'

XWindowTitle::XWindowTitle(BRect r,char *txt):BView(r,"W",B_FOLLOW_NONE,B_WILL_DRAW)
{
	//AddChild(new BPictureButton(BRect(3,5,16,18),".",::GetAppPicture(3),::GetAppPicture(2),new BMessage(B_QUIT_REQUESTED)));
	AddChild(new BPictureButton(BRect(3,25,16,38),".",::GetAppPicture(4),::GetAppPicture(5),new BMessage(X_WINDOW_ZOOM)));

	down=false;
	
	SetViewColor(255,203,0);
	SetLowColor(255,203,0);
	
	BFont tmp(be_bold_font);
	GetFont(&tmp);
	tmp.SetRotation(90.0);	
	//tmp.SetFace(B_REGULAR_FACE);
	tmp.SetSize(16);
	SetFont(&tmp,B_FONT_ROTATION|B_FONT_FACE|B_FONT_SIZE|B_FONT_FAMILY_AND_STYLE);
	text=txt;
	

	spec=BPoint(0,0);
}

void
XWindowTitle::AttachedToWindow()
{
	//if(down)
	//SetViewColor(255,0,0);
	//else
	SetViewColor(255,203,0);
	SetEventMask(0,0);
}

void
XWindowTitle::MouseDown(BPoint p)
{
	down=true;
	SetViewColor(255,0,0);
	SetLowColor(255,0,0);
	here=p;
	p=ConvertToScreen(p);
	px=p.x-Window()->Frame().left;
	py=p.y-Window()->Frame().top;
	SetEventMask(B_POINTER_EVENTS,B_NO_POINTER_HISTORY);
	Window()->Activate(true);
	Invalidate();
	
}

void
XWindowTitle::MouseUp(BPoint p)
{
	down=false;
	SetViewColor(255,203,0);
	SetLowColor(255,203,0);
	SetEventMask(0,B_NO_POINTER_HISTORY);
	Invalidate();
}

void
XWindowTitle::MouseMoved(BPoint p,uint32 k,const BMessage*)
{
	//if(k==B_
	if(down)
	{
		p.x=p.x-px;
		p.y=p.y-py;
		Window()->MoveTo(ConvertToScreen(p));
	}	
	
}
void
XWindowTitle::Draw(BRect area)
{
	if(spec==BPoint(0,0))
	DrawString(text,BPoint(Bounds().Width()-6,Bounds().Height()-4));
		else
	DrawString(text,spec);
}
void
XWindowTitle::SetStringPos(BPoint x)
{
	spec=x;
}