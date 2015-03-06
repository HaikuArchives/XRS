#include "TBWindow.h"
//#include <stdio.h>
#include "knob303.h"
#include "SpecialTrack.h"

#include "Xed_Utils.h"

static const float MIN_WIDTH = 192.f;
static const float MIN_HEIGHT = 204.0f;
static const float XPOS = 100.0f;
static const float YPOS = 200.0f;



TBWindow::TBWindow():BWindow(BRect(XPOS,YPOS,XPOS+MIN_WIDTH,YPOS+MIN_HEIGHT), "",  B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_NOT_ZOOMABLE|B_NOT_CLOSABLE|B_NOT_RESIZABLE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS)
{	
	
	AddChild(knobs=new TB303Knobs(BRect(0,0,MIN_WIDTH,MIN_HEIGHT)));	
	BBitmap	*tile=::MakeBitmapSkin("TNPanel");
	knobs->SetViewBitmap(tile);

	st=NULL;
}



void
TBWindow::Reset(SpecialTrack *s)
{
	st=s;
	MoveTo(s->winx,s->winy);
	knobs->Reset(s);
}

void
TBWindow::FrameMoved(BPoint d)
{
	if(st!=NULL){
	st->winx=d.x;
	st->winy=d.y;
	}
}
bool
TBWindow::QuitRequested()
{
	return true;
}




