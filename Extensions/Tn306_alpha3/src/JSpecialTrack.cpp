#include "JSpecialTrack.h"


#include "GraphicsDef.h"
#include "Pattern.h"
#include "PictureButton.h"
#include "Track.h"
#include "GfxMsg.h"
#include "pot.h"
#include "TBWindow.h"

//#include "XNotesView.h"

#include "Xed_Utils.h"

#include "stdio.h"
#include "TBWindow.h"

#include "JMessages.h"

#define	POT_X		TRACK_LX+TRACK_MENU_LX+70+SBUTTON_W+10
#define	POT_Y		0
#define	POT_H		24
#define	POT_L		24

#define	MSG_V	'pipp'
#define	MSG_N	'plut'

#include "stdio.h"
#include "XTrack.h"

#include "PianoView.h"

const 	rgb_color  	on 	={255,52,49};
const 	rgb_color  	off 	={99,178,140}; //{214,109,115,255};

JSpecialTrack::JSpecialTrack(BRect rect,int16 n,BView* v): JTrack(rect,n,v,JTRACK_VOLUME|JTRACK_NOTE|JTRACK_VST,"TN306Off")
{
	tbw=new TBWindow();
	tbw->Show();
	id=0;
}

JSpecialTrack::~JSpecialTrack()
{
	 if(tbw->Lock())	tbw->Quit();
	// printf("tbw quitten!\n");
			
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
		
			tbw->Activate();
			Window()->PostMessage(message,Parent());
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

/*
void
JSpecialTrack::RControl()
{
	xtr=new XTrack(BRect(1+SBUTTON_W,0,1+SBUTTON_W+TRACK_LX,TRACK_LY-1),::MakeBitmapSkin("TrackNameOn"),::MakeBitmapSkin("TN306Off"),"  TN306 ");
	xtr->SetColors(on,off);
	AddChild(xtr);
	//AddChild(button_fx1 =new BPictureButton(BRect(0,SBUTTON_H,SBUTTON_W,SBUTTON_H*2),"note",::MakePictureSkin("ButtonFX1Off",tv),::MakePictureSkin("ButtonFX1On",tv),new BMessage(TRACK_FX1),B_TWO_STATE_BUTTON));
	AddChild(button_fx1 =new BPictureButton(BRect(0,6,SBUTTON_W-12,6+SBUTTON_H),"note",::MakePictureSkin("ButtonFXOff",tv),::MakePictureSkin("ButtonFXOn",tv),new BMessage(TRACK_FX1),B_TWO_STATE_BUTTON));
	AddChild(button_fx_line=new BPictureButton(BRect(21,6,SBUTTON_W,6+SBUTTON_H),"note",::MakePictureSkin("ButtonFXLine1",tv),::MakePictureSkin("ButtonFXLine2",tv),new BMessage(TRACK_FXLINE),B_TWO_STATE_BUTTON));

	AddChild(vnc=new XNotesView( BRect(160,0,530,BUTTON_LY),tv,id));
	//AddChild(f=new BPictureButton(BRect(530,0,530+TRACK_MENU_LX,TRACK_MENU_LY),"button_mode",::MakePictureSkin("TrackModeNormal",tv),::MakePictureSkin("TrackModeVolume",tv),new BMessage(TRACK_MODE),B_TWO_STATE_BUTTON));
	AddChild(f= new BPictureButton(BRect(530,2,530+TRACK_MENU_LX,11),"button_mode",::MakePictureSkin("TrackModeVolumeOff",tv),::MakePictureSkin("TrackModeVolumeOn",tv),new BMessage(MSG_V),B_TWO_STATE_BUTTON));
	AddChild( f2=new BPictureButton(BRect(530,13,530+TRACK_MENU_LX,22),"button_mode",::MakePictureSkin("TrackModePianoOff",tv),::MakePictureSkin("TrackModePianoOn",tv),new BMessage(MSG_N),B_ONE_STATE_BUTTON));

}
*/

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
	
	JTrack::Reset(p,tr);
	vnc->Reset(p,true);
	xtr->SetColors(on,off);
	tbw->Reset((SpecialTrack*)myTrack);
	 SetName("TN306");
	
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
	tbw->SetTitle(myTrack->getName());
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
