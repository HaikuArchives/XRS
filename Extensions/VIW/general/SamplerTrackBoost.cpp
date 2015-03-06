#include "SamplerTrackBoost.h"
#include "Application.h"
#include "SpecialTrack.h"
#include "filemaker_msg.h"
#include "Message.h"
#include "File.h"
#include "GfxMsg.h"
#include "JSpecialTrack.h"
#include "TrackManager.h"
#include "Path.h"

#include "PlugWindow.h"
#include "VSTInstrument.h"

#define	MY_ID	10;



SamplerTrackBoost::SamplerTrackBoost():TrackBoost()
{
	
	name[0]='V';
	name[1]='I';
	name[2]='W';
	name[3]=' ';
	name[4]='m';
	name[5]='d';
	name[6]='a';
	name[7]='J';
	name[8]='X';
	name[9]='1';
	name[10]='0';
	name[11]='\0';
	id=MY_ID;
}

Track*
SamplerTrackBoost::getTrack()
{
	entry_ref	ref;
	tm->GetXRSDirectoryEntry(&ref,"VSTi");
	BPath p(&ref);
	p.Append("mdaJX10");
		
	return (Track*)(new SpecialTrack(p.Path()));
}
JTrack*
SamplerTrackBoost::getJTrack(BRect r,int16 n,BView* tv)
{
	
 	return (new JSpecialTrack(r,n,tv));
 }

bool
SamplerTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	bool ret=true;
	BMessage *msg;
	SpecialTrack*	st;	
	
	switch(va){
					
			case TRACK_SPECIFIC_DATA_START:
				
				st=(SpecialTrack*)cur;
				msg=new BMessage();
				msg->Unflatten(file);
				
				st->viw->SetConfig(msg);
				st->settings=new BMessage(*msg);				
			break;
			default:
				ret=false;
			break;
		}	
		
		return ret;
}
void
SamplerTrackBoost::Save(Track *cur,int t,BFile* file,int mea)
{
		if(cur==NULL) return;
		
		BMessage*	mes=new BMessage();
		((SpecialTrack*)cur)->viw->GetConfig(mes);
		((SpecialTrack*)cur)->win->SavePref(mes);
		//Devo recuperare la finestra..!!
		WriteFixed(TRACK_SPECIFIC_DATA_START,mes->FlattenedSize(),file);
		mes->Flatten(file);
		
}
