
#include "FxWindow.h"
#include "Sample.h"
#include "SampleView.h"


#include <File.h>
#include <CheckBox.h>
#include "StringView.h"
#include "Sample.h"
#include "GlobalDef.h"
#include "GfxMsg.h"
#include <Message.h>
#include "SamplerTrack.h"
#include <stdio.h> 
#include <math.h>
#include "PatControl.h"
#include "BankManager.h"

#include "Xed_Utils.h"

#define 	REL_MSG 	'note'
#define 	REV_ON	'reo'
#define	PIT_ON 	'pio'
#define	MOD		'mod'


const 	rgb_color  	red 	={253,148,106,255};
const 	rgb_color  	cacca 	={80,80,250,255};

#include "Box.h"
#include "Slider.h"



BSlider * depth;
PatControl*  shift;	
SampleView 	*sw;
BCheckBox 	*rev;
BCheckBox 	*pit_ck;
BBox 		*rev_box;
BBox 		*pit_box;

/* banks Menu (da riordinare) */

#include "Menu.h"
#include "MenuField.h"
#include "MenuItem.h"

#include "Path.h"
#include "Directory.h"
#include "NodeInfo.h" //?

#define	REMOVE	'remv'
#define	REMOVEALL	'rema'
#define	LOADEXT	'loae'

FxWindow::FxWindow(SamplerTrackBoost* tb) :
	BWindow(BRect(0,200,180,430),"SampleWinddow", B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS)
	{
	booster=tb;
	
	BBox *boz= new BBox(BRect(0,0,180,430), "toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_PLAIN_BORDER);
	
	pit_box= new BBox(BRect(10,90,95,145), "", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		

	BBox *sam_box= new BBox(BRect(10,150,171,210) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
			
	BMenu		*menu=new BMenu(" ");
	menu->AddItem(new BMenuItem("Load a Sample..",new BMessage(LOADEXT)));
	menu->AddItem(new BMenuItem("Remove this Sample..",new BMessage(REMOVE)));
	menu->AddItem(new BMenuItem("Remove ALL the Samples..",new BMessage(REMOVEALL)));
	BMenuField	*field=new BMenuField(BRect(10,20,30,40),"","",menu);
	
	
	
	rev=new BCheckBox(BRect(100,90,171,100),"rev_check","Reverse",new BMessage(TRACK_REV));
	rev->SetValue(0);
	rev->SetFontSize(12);
	
	pit_ck=new BCheckBox(BRect(0,0,100,50),"","Strech",new BMessage(PIT_ON));
	pit_ck->SetValue(0);
	pit_ck->SetFontSize(12);
	
			
	
	depth=new BSlider(BRect(100,115,171,130),"Boost","Boost",new BMessage(REL_MSG),1,4,B_TRIANGLE_THUMB);
	depth->SetTarget(this);
	//depth->SetModificationMessage(info);
	depth->SetValue(1);
	depth->SetBarColor(cacca);
	depth->SetFontSize(12);
	

	
	
		
	BView* tv=new BView(BRect(0,0,10,10),"",B_WILL_DRAW,0);
	AddChild(tv);
	shift=new PatControl(BRect(22,25,60,42),::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),1,32,16,MOD,::MakeBitmapSkin("PatBase"));

	
	//shift->SetTarget(this);
	
	text_info=new BStringView(BRect(10,215,140,227.5),"info_control","",B_FOLLOW_NONE);
	text_info->SetFontSize(12);
	
	sw=new SampleView(BRect(2,2,159,58));
	
	sam_box->AddChild(sw);
	
	pit_box->AddChild(shift);
	
	pit_box->SetLabel(pit_ck);
	
	
	boz->AddChild(sam_box);
	boz->AddChild(pit_box);
	boz->AddChild(rev);
	boz->AddChild(field);
	boz->AddChild(text_info);
	boz->AddChild(depth);
	AddChild(boz);
		
	
		
	my_sample=NULL;
	rev->SetValue(false);
	text_info->SetText("No sample selected");
	depth->SetEnabled(true);

	curTrack=NULL;
	
	SetPulseRate(2000);
		
}

bool
FxWindow::QuitRequested()
{
	Hide();
	return false;
}

void
FxWindow::SetTrack(SamplerTrack *tr)
{
	
	curTrack=tr;
	if(Lock()){
	if(tr==NULL)
	{
		sw->Init(NULL,0,0);
		SetTitle("SamplerTrackPanelx");
		shift->setValue(16);
		pit_ck->SetValue(false);
		text_info->SetText("No sample selected");
		depth->SetValue(1);
		Unlock();
		return;
	}
	if(tr->getType()!=UNSET_TYPE)
	{
		my_sample=tr->getSample();
		sw->Init(my_sample->data,my_sample->totalbytes/4,tr->isReversed());
		SetTitle(tr->getName());
		text_info->SetText(my_sample->name);
		shift->setValue(tr->getResample());
		pit_ck->SetValue(tr->isResampleEnable());
		rev->SetValue(tr->isReversed());
		depth->SetValue(tr->amp);
		sw->SetAmp(tr->amp);

		
	}
	Unlock();
	}
}

void
FxWindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
	
		case REL_MSG:
			if(curTrack==NULL) return;
			curTrack->amp=(float)depth->Value();
			sw->SetAmp(curTrack->amp);
			
		break;
		
	 	case TRACK_SAMP_EXT:
	 		booster->ChangeSample(message->FindInt16("sample"));
	 	break;
	 	case MOD:
	 		if(curTrack==NULL) return;
	 		
	 		curTrack->setResample(message->FindInt16("pos"));
	 	
	 	break;
	 	case PIT_ON:
	 		if(curTrack==NULL) return;
	 		curTrack->setResampleEnable((bool)pit_ck->Value());
	 	break;
	 	case TRACK_REV:
			if(curTrack==NULL) return;
			curTrack->setReversed(rev->Value());
			sw->SetReversed(rev->Value());
		
		break;
		case LOADEXT:
			booster->LoadSample();
		break;
		case REMOVEALL:
			booster->RemoveAll();
		break;
		case REMOVE:
			booster->RemoveSample(((SamplerTrack*)curTrack)->getSample());
		break;
		case B_REFS_RECEIVED:
		{
		if(curTrack==NULL) return;	//still not defined..
		
			entry_ref	ref;
			if(message->FindRef("refs",&ref)==B_OK)
			{
					booster->RefReceived(ref,curTrack);
					booster->RefreshSelected();
			}
		}			
		break;	
	 	default:
			BWindow::MessageReceived(message);
		break;
	}
}



