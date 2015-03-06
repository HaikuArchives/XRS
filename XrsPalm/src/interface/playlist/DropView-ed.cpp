/***************************************
/
/	SimpleJuice beta 02
/
/   Something like a DrumMachine.
/	Original idea by Andrea Anzani
/	
/	I hope to find someone that can help me in get
/	this app better!
/	Please write two lines to me.
/
/	andrea.anzani@lib.disco.unimib.it
/
*****************************************/


#include <stdio.h>


#ifndef DROP_VIEW_H
#include "DropView.h"
#endif

#include "Message.h"
#include "Entry.h"
#include "Window.h"
#include "GfxMsg.h"

DropView::DropView(BRect rect, const char *name, const char* text,int16 ix)
	   	   : BTextView(rect, name, BRect(0,0,rect.Width(),rect.Height()),B_FOLLOW_NONE,B_WILL_DRAW)
{
	Insert(text);
	MakeEditable(true);
	
	id=ix;
	SetFont(be_bold_font);
	SetFontSize(12);
	
	SetViewColor(216,216,216);
	SetLowColor(216,216,216);
	
	notify=new BMessage(FALSEPAT_MSG);
	notify->AddInt16("pos",id);
	
	
}



void
DropView::Select()
{
	
    	SetViewColor(255,203,0);
	SetLowColor(255,203,0);
	Invalidate();
}
void
DropView::Deselect()
{
	SetViewColor(216,216,216);
	SetLowColor(216,216,216);
	Invalidate();
}
void DropView::MouseDown(BPoint point)
{
	Window()->PostMessage(notify);
}