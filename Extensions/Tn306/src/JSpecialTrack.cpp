#include "JSpecialTrack.h"
#include "SpecialTrack.h"
#include "GraphicsDef.h"
#include "XTrack.h"
#include "stdio.h"
#include "Meter.h"
#include "async.h"
#include "Track.h"
#include "GfxMsg.h"

#include "TBWindow.h"
#include "stdio.h"
#include "PlugWindow.h"
#include "JMessages.h"

#define	POT_X		TRACK_LX+TRACK_MENU_LX+70+SBUTTON_W+10
#define	POT_Y		0
#define	POT_H		24
#define	POT_L		24

#define	MSG_V	'pipp'
#define	MSG_N	'plut'

#include "knob303.h"

const 	rgb_color  	on 	={255,52,49};
const 	rgb_color  	off 	={99,178,140}; //{214,109,115,255};


static const float MIN_WIDTH = 192.f;
static const float MIN_HEIGHT = 204.0f;


#include "Xed_Utils.h"

JSpecialTrack::JSpecialTrack(BRect rect,int16 n,BView* v): JTrack(rect,n,v,JTRACK_VOLUME|JTRACK_NOTE|JTRACK_VST,"TN306Off")
{
	knobs=new TB303Knobs(BRect(0,0,MIN_WIDTH,MIN_HEIGHT));	
	BBitmap	*tile=::MakeBitmapSkin("TNPanel");
		
	win=new PlugWindow("Tn306");
	win->SetControls(knobs,false);
	
	knobs->SetViewBitmap(tile);
	
	win->Show();

	id=0;
	m_async=new BMessage(ASYNC);
	m_async->AddFloat("value",0);
}

void
JSpecialTrack::SetMeter(float l,float r)
{
	if(Window()!=NULL)
	{
		m_async->ReplaceFloat("value",l);
		Window()->PostMessage(m_async,meter);
		m_async->ReplaceFloat("value",r);
		Window()->PostMessage(m_async,meter2);
	}
}
JSpecialTrack::~JSpecialTrack()
{
	
	  if(win->Lock())	win->Quit();
	
			
}
int
JSpecialTrack::getModel()
{
	return 1;
}
void
JSpecialTrack::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		/*case MSG_V:
	
		//f2->SetValue(false);
		
		if(f->Value()) vnc->setType(1);
			else
			{
				vnc->setType(0);
		
				 Window()->PostMessage(new BMessage(LOST_FOCUS));
			}
		break;	*/
		
		
		case MSG_N:
			vnc->ShowPiano(true);
		break;
		case TRACK_SET:
			win->Activate();
			Window()->PostMessage(message,Parent());
			if(message->FindInt32("mouse") & B_SECONDARY_MOUSE_BUTTON) 
			{
				if(win->Lock()){
				
					if(win->IsHidden()) 
						win->Show();
						else
						win->Hide();
					win->Unlock();
				}
			}
		break;
	case JMSG_NAME_SET:
		
		if(myTrack==NULL) return;
		
		const char* name;
		message->FindString("_value", &name);
		
		
		myTrack->LockName(false);
		
		SetName(name);	
		
		myTrack->LockName(true);
		Window()->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
		//printf("Activationz..\n");
	break;
	
	case JMSG_NAME_NOTSET:
		//printf("Activationz..\n");
		Window()->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
	break;
		default:
		JTrack::MessageReceived(message);
		break;
	}
}


void
JSpecialTrack::RControl()
{
	JTrack::RControl();
	AddChild(meter=new Meter(BRect(SBUTTON_W+TRACK_LX+5,2,SBUTTON_W+TRACK_LX+11,TRACK_MENU_LY-2)));
	AddChild(meter2=new Meter(BRect(SBUTTON_W+TRACK_LX+15,2,SBUTTON_W+TRACK_LX+21,TRACK_MENU_LY-2)));

}


/*
void
JSpecialTrack::Init(BView *t,BMessage *m)
{
	// Init del XNotesView  
	xtr->Init(this,m);
	vnc->Init();
	
	f->SetTarget(this);
	f2->SetTarget(this);
	
	id=m->FindInt16("id");
		
	//button_fx_line->SetTarget(this);
	
}*/

void
JSpecialTrack::Reset(Pattern* p ,Track* tr)
{
/*
	//printf("f! %f\n",p->getNoteAt(0)->getVolume());
	
	myTrack=tr;
	myPat=p; 
	int poz;
	if(myPat->getNumberNotes()>NUM_NOTES) poz=myPat->getNumberNotes();
	else
	poz=NUM_NOTES;
		
	
		
		vnc->ResizeTo(23*poz+2,vnc->Frame().Height());
		
		ResizeTo(160+23*poz+2+97,Frame().Height());	
		
		volpot->MoveTo(160+23*poz+17,volpot->Frame().top);	
		panpot->MoveTo(160+23*poz+49,panpot->Frame().top);		
		f->MoveTo(160+23*poz+2,f->Frame().top);
		f2->MoveTo(160+23*poz+2,f2->Frame().top);

	//t->SetPattern(myPat);
	
	
	
		
	button_fx1->SetValue(myTrack->isFX1On());
	button_fx1->SetTarget(this);
	
	
	volpot->SetValue(myTrack->getVolume()*100);
	volpot->SetOn(myTrack->isOn());
	volpot->SetTarget(this);
	
	panpot->SetValue(myTrack->getPan()*100);
	panpot->SetTarget(this);
		
	vnc->Reset(p);
	
	//if(tr->getType()==UNSET_TYPE)*/
	if(tr==NULL || tr!=myTrack)
	{
		win->MoveTo(((SpecialTrack*)tr)->winx,((SpecialTrack*)tr)->winy);
		if(win->Lock()){
		
		
		if(((SpecialTrack*)tr)->hidden)
				if(!win->IsHidden()) win->Hide();
		if(!((SpecialTrack*)tr)->hidden)
				if(win->IsHidden()) win->Show();
		win->Unlock();	
		}
		if(win->Lock()){	
		win->SetState(((SpecialTrack*)tr)->big,((SpecialTrack*)tr)->Ysize);
		win->Unlock();	
		}
		
		
	}
	((SpecialTrack*)tr)->async=this;
	JTrack::Reset(p,tr);
	vnc->Reset(p,true);
	xtr->SetColors(on,off);
	knobs->Reset((SpecialTrack*)myTrack);
	
	 SetName("TN306");
	
}
void
JSpecialTrack::GetConfig(BMessage* m)
{
	win->SavePref(m);
	bool b;
	if(win->Lock()){
	
		if(win->IsHidden()) b=true; else b=false;
		win->Unlock();
	}
	m->AddBool("hidden",b);	
}
void
JSpecialTrack::SetConfig(BMessage* m)
{
	win->LoadPref(m);

	/*if(win->Lock() && (m->AddBool("hidden",b)==B_OK) ){
	
		if(win->IsHidden()) b=true; else b=false;
		win->Unlock();
	}*/
	
}
void
JSpecialTrack::SetName(const char* t)
{
			
	if(Window()->Lock())
	{
		myTrack->setName(t);
		xtr->SetName(myTrack->getName());
		Window()->Unlock();
	}
	
	win->SetTitle(myTrack->getName());
}

void
JSpecialTrack::Refresh(Pattern* p )
{
	
	myPat=p; 		
	vnc->Reset(p,true);
}
void
JSpecialTrack::Select()
{
	xtr->Select();
}
void
JSpecialTrack::Deselect()
{
	xtr->Deselect();
}
