#include "XNotesView.h"
#include"GraphicsDef.h"
#include "Xed_Utils.h"
#include "TranslationUtils.h"
#include "InterfaceKit.h"
#include "GfxMsg.h"
#include "PianoView.h"

#include "Note.h"

#include "stdio.h"
#include <string.h>

#define	NOTE_VIEW	0
#define	VOL_VIEW	1
#define	FREQ_VIEW	2



XNotesView::XNotesView(BRect rect,BView* tv,int16 t): BControl(rect,"_xnotesview","",NULL,B_FOLLOW_LEFT,B_WILL_DRAW)
{

	picOn=::MakeBitmapSkin("ButtonRedON");
	picOff=::MakeBitmapSkin("ButtonRedOff");
	picOnB=::MakeBitmapSkin("ButtonBluOn");
	picOffB=::MakeBitmapSkin("ButtonBluOff");
		
	curPattern=NULL;
	SetViewColor(B_TRANSPARENT_COLOR);
	SetFont(be_fixed_font);
	SetFontSize(10);
	selected=-1;
	
}
XNotesView::~XNotesView()
{
	ClosePiano();
}
void
XNotesView::Draw(BRect r)
{
	if(curPattern==NULL) return;
	
	BRect rect;
	
	int	ax1=(int)floor(r.left/(BUTTON_LX+BUTTON_X_SPACE));
	int	ax2=(int)ceil(r.right/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax2>curPattern->getNumberNotes()) ax2=curPattern->getNumberNotes();
	
	
	
	for(int h=ax1;h<ax2;h++)
	{
	
	  if(view_type==NOTE_VIEW)
	  {
		rect.Set((BUTTON_LX+BUTTON_X_SPACE)*h,0,(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE,BUTTON_LY);
		
		
						
		if(h%4==0) 			
			if(curPattern->getNoteAt(h)->getValue()) 
					DrawBitmap(picOn,rect); 
				else
					DrawBitmap(picOff,rect);
		else
			
			if(curPattern->getNoteAt(h)->getValue()) 
					DrawBitmap(picOnB,rect); 
				else
					DrawBitmap(picOffB,rect); 
		
		//SetDrawingMode(B_OP_NORMAL);
	}
	else
	
	if(view_type==VOL_VIEW)
	{
			SetHighColor(Parent()->ViewColor());
					
			float vol=curPattern->getNoteAt(h)->getVolume();
			
			rect.Set((BUTTON_LX+BUTTON_X_SPACE)*h+1,0,(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY);
			FillRect(rect);
			
			if(curPattern->getNoteAt(h)->getValue()) 
				SetHighColor(255,255,206);
			else
				SetHighColor(0,0,99);
				
			rect.Set((BUTTON_LX+BUTTON_X_SPACE)*h+1,BUTTON_LY*(float)(1.-vol),(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY);
		//Pan?	rect.Set((BUTTON_LX+BUTTON_X_SPACE)*h+1,(BUTTON_LY/2),(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,(BUTTON_LY)*(float)(1.-vol));

			FillRect(rect);
			
			SetHighColor(0,0,0);
			rect.Set((BUTTON_LX+BUTTON_X_SPACE)*h+1,0,(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY);
			StrokeRect(rect);
				
						
	}
	
	}
}
void
XNotesView::MouseMoved(BPoint p, uint32 tra/* transit */,
	const BMessage* /* dragDropMsg */)
{	
	if(curPattern==NULL ) return;
	int	ax1=(int)floor(p.x/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax1>=curPattern->getNumberNotes()) ax1=curPattern->getNumberNotes()-1;
	if(ax1<0) ax1=0;
		
	if (IsTracking()  && tra==B_INSIDE_VIEW) {
		
		
		
		if(view_type==NOTE_VIEW && prev != ax1)
		{
			curPattern->getNoteAt(ax1)->setValue(set_state);
			Window()->PostMessage(new BMessage(GENERIC));
		}
		if(view_type==VOL_VIEW)
		{
			curPattern->getNoteAt(ax1)->setVolume(1.-((float)p.y/(float)BUTTON_LY));	
		}
		Invalidate(BRect(ax1*(BUTTON_LX+BUTTON_X_SPACE),0,(ax1+1)*(BUTTON_LX+BUTTON_X_SPACE)-1,BUTTON_LY));	
		
		prev = ax1;
	
	}
}
void
XNotesView::MouseUp(BPoint p)
{SetTracking(false); prev=-1;}
void
XNotesView::MouseDown(BPoint p)
{
	if(curPattern==NULL) return;
	int	ax1=(int)floor(p.x/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax1>= curPattern->getNumberNotes() ) ax1=curPattern->getNumberNotes()-1;
	if(ax1<0) ax1=0;
	
	prev = ax1;
	SetTracking(true);
	SetMouseEventMask(B_POINTER_EVENTS,
				B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
				
	set_state=!curPattern->getNoteAt(ax1)->getValue();	
	
	if(view_type==NOTE_VIEW)
	{
		curPattern->getNoteAt(ax1)->Swap();
		Window()->PostMessage(new BMessage(GENERIC));
	}
	else if(view_type==VOL_VIEW)
	{
		curPattern->getNoteAt(ax1)->setVolume(1.-((float)p.y/(float)BUTTON_LY));	
	}
	
	Invalidate(BRect(ax1*(BUTTON_LX+BUTTON_X_SPACE),0,(ax1+1)*(BUTTON_LX+BUTTON_X_SPACE)-1,BUTTON_LY));
	
}

status_t
XNotesView::Init(){
	
	view_type=NOTE_VIEW;
	return B_OK;
}

void
XNotesView::ShowPiano(bool extra)
{
if(pw==NULL)
		{
			//printf("Creating a new PianoWindow..\n");
			BRect r(ConvertToScreen(Bounds()));
		
			r.top +=r.Height();
			r.bottom +=126;//66;
			r.right -=2;
			r.left -=14;
			
			pw=new PianoView(r,Window(),Window()->CurrentFocus());
			pw->Reset(curPattern,extra);
		}
		else
		{
			BRect r(ConvertToScreen(Bounds()));
			pw->MoveTo(r.left - 14,r.top +r.Height());
			
			if(pw->Lock())
			{
				if(pw->IsHidden()) {
					
					pw->Show();
					
				}
				
				pw->Unlock();
			}
			pw->Reset(curPattern,extra);
		}
		pw->Activate(true);
		return;
		
	
	
}

void
XNotesView::setType(int val)
{
	if(val==2) return;
	view_type=val;
	selected=-1;
	Invalidate();
}
int
XNotesView::getType()
{
	return view_type;
}
void
XNotesView::ClosePiano()
{
	if(pw!=NULL)
	if(pw->Lock()) 	{
					pw->Quit();
					pw=NULL;
					printf("Piano closed!\n");
				}
	

}
void
XNotesView::Reset(Pattern* trk,bool b)
{
	curPattern=trk;
	pw->Reset(trk,b);
	Invalidate();			
}