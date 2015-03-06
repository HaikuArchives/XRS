
#include "FxWindow.h"

#include "Sample.h"


#include "SampleView.h"
#include "VStringView.h"

#include <File.h>
#include <CheckBox.h>

#include "StringView.h"
#include "Sample.h"
#include "GlobalDef.h"


#include <Message.h>
#include <stdio.h> 
#include <math.h>





//#include "WU_AppUtils.h"

#define 	REL_MSG 	'note'
#define 	REV_ON	'reo'
#define	PIT_ON 	'pio'
#define	 MOD			'mod'

const 	rgb_color  	red 	={253,148,106,255};
const 	rgb_color  	cacca 	={80,80,250,255};

#include "Box.h"
#include "Slider.h"
BSlider * depth;
BSlider*  mix;
BSlider*  shift;	



SampleView 	*sw;
BCheckBox 	*mute;
BCheckBox 	*pit_ck;
BBox 		*rev_box;
BBox 		*pit_box;




FxWindow::FxWindow() :
	BWindow(BRect(0,200,180,430),"SampleWindow", B_FLOATING_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_WILL_ACCEPT_FIRST_CLICK|B_NOT_ZOOMABLE)
{
	
	BBox *boz= new BBox(BRect(20,0,180,430), "toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_PLAIN_BORDER);
	
	rev_box= new BBox(BRect(10,45,151,140), "toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);
	
	pit_box= new BBox(BRect(10,5,151,40), "", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		

	BBox *sam_box= new BBox(BRect(10,150,151,210) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
			
	
	mute=new BCheckBox(BRect(0,0,100,50),"mute_check","Reverb",new BMessage(REV_ON));
	mute->SetValue(0);
	
	pit_ck=new BCheckBox(BRect(0,0,100,50),"","PitchShift",new BMessage(REV_ON));
	pit_ck->SetValue(0);
	
	
	
	VStringView	*erba;
	erba=new VStringView(BRect(0,0,20,230),"DSP Effects");
	erba->SetStringPos(BPoint(15,150));
	AddChild(erba);
	
	

	
	BMessage*	info;
	BMessage*	the_cutter=new BMessage(REL_MSG);
	
	
	// Depth //"           Depth"
	
	info=new BMessage(MOD);
	depth=new BSlider(BRect(12,20,120,35),"Depth","Depth",the_cutter,0,30,B_TRIANGLE_THUMB);


	depth->SetTarget(this);
	depth->Archive(info);
	depth->SetModificationMessage(info);
	depth->SetValue(10);
	depth->SetBarColor(cacca);
	depth->SetFontSize(10);
	
	//----------------//
	
	
	info=new BMessage(MOD);
	mix=new BSlider(BRect(12,55,120,68),"Mix","Mix",the_cutter,0,100,B_TRIANGLE_THUMB);
	
	mix->SetTarget(this);
	mix->Archive(info);
	mix->SetModificationMessage(info);
	mix->SetValue(50);
	mix->SetBarColor(red);
	mix->SetFontSize(10);
	
	//----------------//
	
	
	info=new BMessage(MOD);
	shift=new BSlider(BRect(12,15,120,25),"Mix",NULL,the_cutter,0,30,B_TRIANGLE_THUMB);
	shift->SetTarget(this);
	shift->Archive(info);
	shift->SetModificationMessage(info);
	shift->SetValue(10);
	shift->SetBarColor(red);
	
	
	
	text_info=new BStringView(BRect(10,190,140,227.5),"info_control","",B_FOLLOW_NONE);
	boz->AddChild(text_info);
	
	sw=new SampleView(BRect(2,2,139,58));
	
	sam_box->AddChild(sw);
		
	//t60_val=1;
	//mix_val=0.5;
	//type_val=1;
	

	rev_box->AddChild(depth);
	rev_box->AddChild(mix);
	
	pit_box->AddChild(shift);
	
	rev_box->SetLabel(mute);
	pit_box->SetLabel(pit_ck);
	
	boz->AddChild(rev_box);
	boz->AddChild(sam_box);
	boz->AddChild(pit_box);
	
	AddChild(boz);
		
		
	my_sample=NULL;
	mute->SetValue(false);
	text_info->SetText("No sample selected");
	depth->SetEnabled(false);
	mix->SetEnabled(false);
	shift->SetEnabled(false);
		
	
	
}
bool
FxWindow::QuitRequested()
{
	BMessage *quix;
	//quix=new BMessage(SHOW_FX);
	//boss->PostMessage(quix);
	//Hide();
	return true;
}

void
FxWindow::SetJSample(Sample *samp)
{
	bool en;
	
	my_sample=samp;
	if(Lock()){
	
	
	sw->Init(my_sample->data,my_sample->totalbytes/4,0);
	SetTitle(samp->name);
	
	depth->SetValue(10);
	mix->SetValue(100);
	
	shift->SetValue(10);
	
		
	bool en=true;
		
	mute->SetValue(en);
	depth->SetEnabled(en);
	mix->SetEnabled(en);
	

	
	pit_ck->SetValue(en);
	shift->SetEnabled(en);
	
	Unlock();
	}
}

void
FxWindow::MessageReceived(BMessage* message)
{

	
		
	switch(message->what)
	{
	
	 	
	 	/*
	 	case MOD:
			sprintf(txt,"%10s : %10ld",message->FindString("_label"),message->FindInt32("be:value"));
			text_info->SetText(txt);
		break;
	 
	 		 	
	 	case REL_MSG:
	 		
	 		my_sample->par[3]=(float)depth->Value()/(float)10;
			my_sample->par[4]=(float)mix->Value()/(float)100;
			my_sample->par[6]=(float)shift->Value()/(float)10;
			AddFx();
	 	break;
	 	
	 	
	 	case REV_ON:
	 	
	 	if(my_sample==NULL) {
	 	
	 		mute->SetValue(false);
	 		pit_ck->SetValue(false);
	 		text_info->SetText("No sample selected");
	 	}
	 	
	 	else
	 	
	 	{
	 		text_info->SetText("");
	 		
	 		my_sample->par[2]=mute->Value();
	 		my_sample->par[7]=pit_ck->Value();
	 		
	 		AddFx();
	 	}
	 	
	 	depth->SetEnabled(mute->Value());
	 	mix->SetEnabled(mute->Value());
	 	shift->SetEnabled(pit_ck->Value());
	 	
	 	
	 	break;*/
	 	
	 	default:
			BWindow::MessageReceived(message);
		break;
	}
}


void
FxWindow::ResetJSample()
{

	// Lock??????
	if(my_sample==NULL) return;
	
	int8	default_data=0x80;
	
	BFile	*tfile;
	
   	//tfile=my_sample->temp_file;
   	
   	/*if(my_sample->modifier==3){
   	
   	tfile->Seek(0,SEEK_SET);
   	tfile->Read(my_sample->wave_data,my_sample->totalbytes);
	
	
	memset((char*)my_sample->wave_data+my_sample->totalbytes,default_data,my_sample->totalbytes*2);
	my_sample->modifier=1;
	
	}*/
	
}




void
FxWindow::AddFx()
{}
