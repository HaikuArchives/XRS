// PianoView.cpp

#include "PianoView.h"

#include <Debug.h>
#include <TextControl.h>

#include "stdio.h"
// ---------------------------------------------------------------- //
#include "Xed_Utils.h"
#include "Pattern.h"
#include "ScrollBar.h"
#include "Note.h"
#include "GfxMsg.h"


#define	NOTE	'note'
#define	SLIDE	'slid'
#define	MOVESCROLL	'mosc'
#define	NOTESIZE	10.
#define	TOTALSIZE	120.

static double	poz=-1;

class SliderControl:
	public BView{
	typedef	BView  _inherited;
	public:
		SliderControl(	BRect	frame) :
		BView(frame,  "",  B_FOLLOW_ALL_SIDES,B_WILL_DRAW) {
		noteMsg=new BMessage(SLIDE);
		noteMsg->AddInt8("n",0);
		curPattern=NULL;
		}
		
	void	Reset(Pattern* p){
	
		curPattern=p;
		Invalidate();
	}
		
	private:
		BMessage*				noteMsg;
		Pattern*				curPattern;	
		
	public:
	
	virtual void Draw(BRect r)
	{
		if(curPattern==NULL) return;
		
		//printf("Drawnd?\n");
	
		int	ax1=(int)floor(r.left/(23));
		int	ax2=(int)ceil(r.right/(23));
		
		if(ax2>curPattern->getNumberNotes()) ax2=curPattern->getNumberNotes();
		if(ax1<0) ax1=0;
		
		SetHighColor(0,0,0);
				StrokeLine(BPoint(ax1*23,0),BPoint(ax2*23-23,0));
		for(int h=ax1;h<ax2;h++)
		{
		
			SetHighColor(255,0,0);
	  		if(  curPattern->getNoteAt(h)->getOct() & 8)
	  		
	  		//FillTriangle(BPoint(),BPoint(),BPoint());
				 FillRect(BRect(h*23+1,0, h*23+22,Bounds().Height()+1 ));
		}
	}
	
	
	virtual void MouseDown(BPoint point)
	{	
		if(!Bounds().Contains(point) || curPattern==NULL)	return;
		
	
		
		int	ax1=(int)floor(point.x/(23));
		if(ax1>= curPattern->getNumberNotes() ) ax1=curPattern->getNumberNotes()-1;
		if(ax1<0) ax1=0;
		  curPattern->getNoteAt(ax1)->setOct(curPattern->getNoteAt(ax1)->getOct()^8);
		
		
		 Invalidate(BRect(ax1*23,0,ax1*23+23,Bounds().bottom));
		
		
	}
};

class PianoControl :
	public	BView {
	typedef	BView  _inherited;

private:
		Pattern*				curPattern;			
public:
		BBitmap*				piano;
		BMessage*				noteMsg;
		BView*				w_keypass;
		

	PianoControl(
		BRect								frame,
		const char*							name,
		BView*							keypass,
		uint32								resizingMode=B_FOLLOW_LEFT|B_FOLLOW_TOP,
		uint32								flags=B_WILL_DRAW|B_NAVIGABLE) :
		BView(frame,  name,   resizingMode, flags) {
	
		w_keypass=keypass;
		curPattern=NULL;
		SetEventMask(B_POINTER_EVENTS|B_KEYBOARD_EVENTS);
		piano=::MakeBitmapSkin("piano6");	
		noteMsg=new BMessage(NOTE);
		noteMsg->AddInt8("n",0);
		noteMsg->AddInt8("o",0);
	}
	
public:

	void	Reset(Pattern* p){
	
		curPattern=p;
		Invalidate();
	}	
	
	virtual void AllAttached() {
		Window()->Show();
	}
	
	virtual void Draw(BRect r)
	{
		if(curPattern==NULL) return;
		
	int	ax1=(int)floor(r.left/(23));
	int	ax2=(int)ceil(r.right/(23));
	
	int	ay=(int)floor(r.top/(float)TOTALSIZE);
	int	ay2=(int)floor(r.bottom/(float)TOTALSIZE);
	
	if(ax2>curPattern->getNumberNotes()) ax2=curPattern->getNumberNotes();
	
	
	//printf("ay.top  %d ay2.bottom %d  -- top: %f bottom: %f  -- top %f\n",ay,ay2,r.top,r.bottom,kk);
	
	for(int h=ax1;h<ax2;h++)
	{
	
	for(int j=ay;j<ay2+1;j++) DrawBitmap(piano,BPoint(h*23,j*TOTALSIZE));
	 //nota:
	 int oct=(curPattern->getNoteAt(h)->getNote())/12;
	 int n=(curPattern->getNoteAt(h)->getNote())%12;
	
		if( ay <= 8-oct && ay2 >= 8-oct)
		{ 	 
		
		 SetHighColor(255,0,0);
		
		if (n==1 || n==3 || n==6 || n==8 || n==10) // Note #
			 FillRect(BRect(h*23+1,     (9-oct)*TOTALSIZE-(n*NOTESIZE+NOTESIZE-1)    ,  h*23+10,  (9-oct)*TOTALSIZE-(n*NOTESIZE)-2 ));
		else
			 FillRect(BRect(h*23+12,     (9-oct)*TOTALSIZE-(n*NOTESIZE+NOTESIZE-1)    ,  h*23+21,  (9-oct)*TOTALSIZE-(n*NOTESIZE)-2 ));
		
		}
	}
	
	
}

	virtual void MouseMoved(BPoint point,uint32 i,const BMessage*)
	{
		if(i==B_INSIDE_VIEW)
		{
				int	ax2=(int)floor(point.y/NOTESIZE);
			
			noteMsg->ReplaceInt8("n",107-ax2);
			Window()->PostMessage(noteMsg);
			
			
		}	
		
	}

	virtual void MouseDown(BPoint point) {
		
	if(!Bounds().Contains(point) || curPattern==NULL)	return;
		
	uint32 buttons;
	GetMouse(&point, &buttons);
	
	int	ax1=(int)floor(point.x/(23));
	
	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		
		
		int	ax2=(int)floor(point.y/NOTESIZE);
	
		if(ax1>= curPattern->getNumberNotes() ) ax1=curPattern->getNumberNotes()-1;
		if(ax1<0) ax1=0;
	
		int old=curPattern->getNoteAt(ax1)->getNote();
	
		Invalidate(BRect(ax1*23,(107-old)*NOTESIZE,ax1*23+22,(107-old)*NOTESIZE+NOTESIZE+1));
		curPattern->getNoteAt(ax1)->setNote(107-ax2);
		Invalidate(BRect(ax1*23,(ax2)*NOTESIZE,ax1*23+22,(ax2)*NOTESIZE+NOTESIZE+1));
	}
	 else
		 	{
		 		BMessage* m=new BMessage(MOVESCROLL);
		 		m->AddInt16("goto",curPattern->getNoteAt(ax1)->getNote());
		 		Window()->PostMessage(m);	
		 	}
	
	}
	
	virtual void KeyDown(const char*	bytes,	int32	numBytes) {
		
		
		if(numBytes == 1 && *bytes == B_ESCAPE) {
			BWindow* w = Window(); 
			BMessenger(w).SendMessage(B_QUIT_REQUESTED);
			return;
		}
			
	}
};


// ---------------------------------------------------------------- //
// PianoView a BWindow
// ---------------------------------------------------------------- //

PianoView::PianoView(BRect	 frame,const BMessenger&	target, BView* keypass) :
	BWindow(frame, "PianoView",B_NO_BORDER_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL,0),
	m_target(target),
	m_keypass(keypass),
	m_sentUpdate(false) 
	{// inizia qui!

	BRect r(Bounds());
	
	r.bottom -=10;
	r.left +=14;
	
	m_control = new PianoControl(r,"piano",m_keypass,B_FOLLOW_ALL_SIDES);
	
	
	extra_control=new SliderControl(BRect(r.left,r.bottom+1,r.right,Bounds().bottom));
	
	
	sb=new BScrollBar(BRect(0,0,13,r.Height()),"oct",m_control,0,840,B_VERTICAL);	
	m_text=new BStringView(BRect(-2,r.bottom+1,13,r.bottom+15),"---","---");
	m_text->SetFont(be_fixed_font);
	m_text->SetFontSize(10);
	
	if(poz<0) sb->SetValue(380);
		else
		sb->SetValue(poz);
	
	Run();
	Lock();
	

	m_control->SetViewColor(0,0,0);
	extra_control->SetViewColor(0,0,200);
	
	AddChild(m_control);
	AddChild(m_text);
	AddChild(sb);
	AddChild(extra_control);
	m_control->MakeFocus();
	Unlock();
}


void PianoView::WindowActivated( bool activated) {

	if(!activated)
		{
			Hide();
			//Activate(false);
		}
}



void PianoView::MessageReceived( BMessage* message) {
	
	
	
	switch(message->what) {
		
		case NOTE:
			char x[10];
			sprintf(x,"%d",message->FindInt8("n"));
			m_text->SetText(x);
		break;
		case MOVESCROLL:
			{
				float pos=(107-message->FindInt16("goto"))*NOTESIZE-(m_control->Bounds().Height()/2.);
				sb->SetValue(pos);
			}
		break;
		default:
			_inherited::MessageReceived(message);
	}
	
}		

bool PianoView::QuitRequested() {
	
	//poz=sb->Value();
	//Hide();
	Activate(false);
	return false;
	
}
void PianoView::Reset(Pattern* p,bool extra) {

	if(Lock()){
		
		((PianoControl*)m_control)->Reset(p);
			
		if(!extra &&  !extra_control->IsHidden() ) extra_control->Hide();
		if(extra){
				if(extra_control->IsHidden()) extra_control->Show();
			 	 ((SliderControl*)extra_control)->Reset(p);
			 	
			  	
			 }
		Unlock();
	}
}
