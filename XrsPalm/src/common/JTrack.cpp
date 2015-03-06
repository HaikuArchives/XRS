#include "JTrack.h"
#include "XTrack.h"
//#include "XNotesView.h"
#include "JMessages.h"
#include "GraphicsDef.h"
#include "Pattern.h"
#include "PictureButton.h"
#include "Track.h"
#include "GfxMsg.h"
//#include "PopUpMenu.h"
#include "pot.h"
#include "stdio.h"
#include "Xed_Utils.h"

#include "TextControlFloater.h"

#define	POT_X		TRACK_LX+TRACK_MENU_LX+70+SBUTTON_W+10
#define	POT_Y		0
#define	POT_H		24
#define	POT_L		24

#include "ColorsDef.h"

#define	MSG_N		'plut'
const 	rgb_color  	onc 	={255,52,49};//{81,110,117,255};
//const 	rgb_color  	offc 	={57,186,255};//{107,146,156,255}; 

JTrack::JTrack(BRect rect,int16 n,BView* v,uint32 type=7,const char*na="TrackNameOff"):BView(rect,"_jtrack",B_FOLLOW_LEFT ,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP),
id(n),flags(type),name_pad(na)
{
	SetViewColor(bkColor);
	
	tv=v;
	popup=NULL;
	
	SetHighColor(0, 0, 0, 0);
	FillRect(Bounds()); 
        	SetDrawingMode(B_OP_ALPHA);
        	SetHighColor(0, 0, 0, 128);
        	SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_COMPOSITE);
        //	DrawBitmap(mSmallIcon);
	
	
	AddChild(volpot=new BPot(BRect(545,POT_Y,545+POT_H,POT_Y+POT_L), "name","pippo",new BMessage(TRACK_VOL),new BMessage(TRACK_ON),0, 100, ::MakeBitmapSkin("volpad"),::MakeBitmapSkin("volpadoff"),B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP));
	AddChild(panpot=new BPot(BRect(550+POT_H+3,POT_Y+3,550+POT_H+3+18,POT_Y+21), "pan","pippo",new BMessage(TRACK_PAN),new BMessage(PAN_RESET),-100, 100,::MakeBitmapSkin("panpad"),NULL, B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP));
	
}

void
JTrack::RControl()
{
	//popup = new BPopUpMenu("");
	//popup->SetLabelFromMarked(true);
	AddChild(button_mode = new BPictureButton(BRect(530,2,530+TRACK_MENU_LX,11),"button_mode",::MakePictureSkin("TrackModeVolumeOff",tv),::MakePictureSkin("TrackModeVolumeOn",tv),new BMessage(TRACK_MODE),B_TWO_STATE_BUTTON));
	if(flags&JTRACK_VST) AddChild(button_fx1 =new BPictureButton(BRect(0,6,SBUTTON_W-12,6+SBUTTON_H),"",::MakePictureSkin("ButtonFXOff",tv),::MakePictureSkin("ButtonFXOn",tv),new BMessage(TRACK_FX1),B_TWO_STATE_BUTTON));
	if(flags&JTRACK_VST) AddChild(button_fx_line=new BPictureButton(BRect(21,6,SBUTTON_W,6+SBUTTON_H),"",::MakePictureSkin("ButtonFXLine1",tv),::MakePictureSkin("ButtonFXLine2",tv),new BMessage(TRACK_FXLINE),B_TWO_STATE_BUTTON));
	if(flags&JTRACK_MENU) AddChild(trackmenu=new BPictureButton(BRect(1+SBUTTON_W+TRACK_LX+1,0,1+SBUTTON_W+TRACK_MENU_LX+TRACK_LX,TRACK_MENU_LY),"_menu",::MakePictureSkin("TrackMenuOff",tv),::MakePictureSkin("TrackMenuOn",tv),new BMessage(POPUPMENU),B_ONE_STATE_BUTTON));
	AddChild(xtr=new XTrack(BRect(1+SBUTTON_W,0,1+SBUTTON_W+TRACK_LX,TRACK_LY-1),::MakeBitmapSkin("TrackNameOn"),::MakeBitmapSkin(name_pad),"UnSet!"));
	AddChild(vnc=new XNotesView( BRect(160,0,530,BUTTON_LY),tv,id));
	if(flags&JTRACK_NOTE) AddChild( button_piano=new BPictureButton(BRect(530,13,530+TRACK_MENU_LX,22),"button_mode",::MakePictureSkin("TrackModePianoOff",tv),::MakePictureSkin("TrackModePianoOn",tv),new BMessage(MSG_N),B_ONE_STATE_BUTTON));
}


void
JTrack::MessageReceived(BMessage* message)
{

		
	if(message->what>1000 && message->what<2000)
	{
		if(message->what>1900) Window()->PostMessage(message);
		else Window()->PostMessage(new BMessage(GENERIC));
	}
	
			
	switch(message->what)
	{

	
	case TRACK_ON:
		myTrack->setOn(volpot->isOn());
	break;
	case TRACK_VOL:
		myTrack->setVolume((float)volpot->Value()/(float)100);
	break;
	case TRACK_PAN:
		myTrack->setPan((float)panpot->Value()/(float)100);
	break;
	case	B_SIMPLE_DATA:
	break;
	case PAN_RESET:
		myTrack->setPan(0);
		panpot->SetValue(0);
	break;
	
	case MSG_N:
		vnc->ShowPiano(false);
	break;
	
	case TRACK_MODE:
	
		if(vnc->getType()==0) vnc->setType(1);
			else
			vnc->setType(0);
	break;	
	case TRACK_FX1:
		if(flags&JTRACK_VST) myTrack->setFX1On(button_fx1->Value());
	break;
	case TRACK_FXLINE:
	if(flags&JTRACK_VST)
		if(button_fx_line->Value())
			myTrack->setFX1Line(1);
			else
			myTrack->setFX1Line(0);

	break;
	case TRACK_SET:
		Window()->PostMessage(message,Parent());
	break;

	case JMSG_NAME_SET:
		
		if(myTrack==NULL) return;
		
		const char* name;
		message->FindString("_value", &name);
		
		
		myTrack->LockName(false);
		
		if(strlen(name)>0) 
			{
				SetName(name);	
				myTrack->LockName(true);
			}
			
			else
				{
					myTrack->ResetName();
					xtr->SetName(myTrack->getName());
				}
		
		Window()->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
	break;
	case JMSG_NAME_NOTSET:
		//Window()->PostMessage(2227);
		//printf("Activationz..\n");
		Window()->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
	break;
	
	case 'ttt':
		{
		
		BPoint f,t;
		
		message->FindPoint("from", &f);
		message->FindPoint("to", &t);
		/*  Disabling KeyBoard for MainWindow */
		Window()->CurrentFocus()->SetEventMask(0);
		
		new TextControlFloater(BRect(f,t),B_ALIGN_LEFT,be_plain_font,myTrack->getName(),this,new BMessage(JMSG_NAME_SET),new BMessage(JMSG_NAME_NOTSET));
		
		}
	break;
	default:
		
		BView::MessageReceived(message);
		
		break;
	}

}
void
JTrack::ShowMenu()
{
	BMenuItem*	selected;
	BPoint	point(TRACK_LX,0);
	
	ConvertToScreen(&point);
	selected=popup->Go(point,true,true,true);
}
void
JTrack::Init(BView *t,BMessage *m)
{
	// Init del XNotesView  
	xtr->Init(this,m);
	vnc->Init();
	if(flags&JTRACK_NOTE) button_piano->SetTarget(this);
	if(flags&JTRACK_VST) button_fx1->SetTarget(this);
	if(flags&JTRACK_VST) button_fx_line->SetTarget(this);
	button_mode->SetTarget(this);
	if(flags&JTRACK_MENU) trackmenu->SetTarget(this);
	volpot->SetTarget(this);
	panpot->SetTarget(this);
}
void
JTrack::Reset(Pattern* p ,Track* tr)
{
	
	myTrack=tr;
	myPat=p; 
	
	int poz;
	if(myPat->getNumberNotes()>NUM_NOTES) poz=myPat->getNumberNotes();
	else
	poz=NUM_NOTES;
		
	
		
		vnc->ResizeTo(23*poz+2,vnc->Frame().Height());
		vnc->ClosePiano();
		
		ResizeTo(160+23*poz+2+97,Frame().Height());	
		
		volpot->MoveTo(160+23*poz+17,volpot->Frame().top);	
		panpot->MoveTo(160+23*poz+49,panpot->Frame().top);		
		button_mode->MoveTo(160+23*poz+2,button_mode->Frame().top);
		if(flags&JTRACK_NOTE) 
							button_piano->MoveTo(160+23*poz+2,button_piano->Frame().top);
							
	if(flags&JTRACK_VST) button_fx1->SetValue(myTrack->isFX1On());
	if(flags&JTRACK_VST) button_fx_line->SetValue(myTrack->getFX1Line());
	volpot->SetValue(myTrack->getVolume()*100);
	volpot->SetOn(myTrack->isOn());
	panpot->SetValue(myTrack->getPan()*100);
			
	if(tr->getType()==UNSET_TYPE) SetName("UnSet!");
	else
		SetName(tr->getName());
	
	vnc->Reset(p,false);
	
}
void
JTrack::SetName(const char* t)
{
	if(myTrack==NULL) return;
		
	if(Window()->Lock())
	{
		myTrack->setName(t);
		xtr->SetName(myTrack->getName());
		Window()->Unlock();
	}
}

void
JTrack::Refresh(Pattern* p )
{
	
	myPat=p; 
	vnc->Reset(p,false);
}
void
JTrack::Select()
{
	xtr->Select();
}
int
JTrack::getModel()
{
	return 0;
}
void
JTrack::Deselect()
{
	xtr->Deselect();
}
void
JTrack::Mute(bool b,bool c)
{

	//ATTENZIONE! ->Mute() si comporta non come sembra..	
	
	if(!c)
		volpot->SetOn(!volpot->isOn());
	
	else
		volpot->SetOn(b);
	
	
	myTrack->setOn(volpot->isOn());
}