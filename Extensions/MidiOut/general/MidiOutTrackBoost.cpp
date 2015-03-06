#include "MidiOutTrackBoost.h"
#include "Application.h"
#include "JTrack.h"
#include "JMidiOutTrack.h"
#include "MidiOutTrack.h"
#include "filemaker_msg.h"
#include "Message.h"
#include "File.h"
#include "GfxMsg.h"
// DINAMIC CAST!!!!!!!! //

#include "MidiProducer.h"
#include "MidiConsumer.h"
#include "Bitmap.h"
#include"Roster.h"
#include "AppFileInfo.h"

BMidiLocalProducer *mider;
const char* LARGE_ICON_NAME = "be:large_icon";
const char* MINI_ICON_NAME = "be:mini_icon";
const uint32 LARGE_ICON_TYPE = 'ICON';
const uint32 MINI_ICON_TYPE = 'MICN';


MidiOutTrackBoost::MidiOutTrackBoost():TrackBoost()
{
	name[0]='M';
	name[1]='i';
	name[2]='d';
	name[3]='i';
	name[4]='O';
	name[5]='u';
	name[6]='t';
	name[7]='\0';
	id=2;
	mider=new BMidiLocalProducer("XRS:MidiOutExtension");
	mider->Register();
	// qui si puo' mettere l'icona..
	// come non si sa ma si puo'...
	// veder EndpointInfo.cpp in PacthBay
	BBitmap* largeIcon = new BBitmap(BRect(0,0,31,31), B_CMAP8);
	BBitmap* miniIcon = new BBitmap(BRect(0,0,15,15), B_CMAP8);
	GetIcons(largeIcon, miniIcon);
	BMessage msg;
	
	if (mider->GetProperties(&msg) == B_OK)
	{
		AddIcons(&msg, largeIcon, miniIcon);
		mider->SetProperties(&msg);
	} 	
	
	//mider->SetProperties(new BMessage('pa'));
	
}

Track*
MidiOutTrackBoost::getTrack()
{
	return (Track*)(new MidiOutTrack(mider));
}
JTrack*
MidiOutTrackBoost::getJTrack(BRect r,int16 n,BView* tv)
{
	JTrack *jt=new JMidiOutTrack(r,n,tv);

 	return jt;
 }

bool
MidiOutTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
}
/*void
MidiOutTrackBoost::Save(Track *cur,int t,BFile* file,int mea)
{
	
}*/
bool
MidiOutTrackBoost::NeedMessage(){return true;}
void
MidiOutTrackBoost::MessageReceived(BMessage* message)
{

	//printf("Target... \n");
		
	switch(message->what)
	{

	case POPUPMENU:
		
		ShowMenu();
		//printf("Target... \n");
	break;
	
	default:
		
		
			
		break;
	}

}
void
MidiOutTrackBoost::ShowMenu()
{

	/*
	//BMenuItem*	selected;
	BPoint	point(99,0);
	
	//ConvertToScreen(&point);
	//selected=
	popup->Go(point,true,true,true);*/
	
	
}
void MidiOutTrackBoost::GetIcons(BBitmap* largeIcon, BBitmap* miniIcon)
{
	app_info ai;
	status_t err;
	
	// get the icons from the app file info of the
	// currently running app
	err = be_app->GetAppInfo(&ai);
	if (err == B_OK) {
		BFile file(&(ai.ref), B_READ_ONLY);
		err = file.InitCheck();
		if (err == B_OK) {
			BAppFileInfo afi(&file);
			err = afi.InitCheck();
			if (err == B_OK) {
				afi.GetIcon(largeIcon, B_LARGE_ICON);
				afi.GetIcon(miniIcon, B_MINI_ICON);
			}
		}
	}
}
void MidiOutTrackBoost::AddIcons(BMessage* msg, BBitmap* largeIcon, BBitmap* miniIcon) const
{
	if (! msg->HasData(LARGE_ICON_NAME, LARGE_ICON_TYPE)) {
		msg->AddData(LARGE_ICON_NAME, LARGE_ICON_TYPE, largeIcon->Bits(),
			largeIcon->BitsLength());
	} else {
		msg->ReplaceData(LARGE_ICON_NAME, LARGE_ICON_TYPE, largeIcon->Bits(),
			largeIcon->BitsLength());
	}
	if (! msg->HasData(MINI_ICON_NAME, MINI_ICON_TYPE)) {
		msg->AddData(MINI_ICON_NAME, MINI_ICON_TYPE, miniIcon->Bits(),
			miniIcon->BitsLength());
	} else {
		msg->ReplaceData(MINI_ICON_NAME, MINI_ICON_TYPE, miniIcon->Bits(),
			miniIcon->BitsLength());
	}
}
