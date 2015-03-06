#include "JAnalizerTrack.h"
//#include "XNotesView.h"
#include "Xed_Utils.h"
#include "GraphicsDef.h"
#include "Pattern.h"
#include "PictureButton.h"
#include "Track.h"
#include "GfxMsg.h"
#include "pot.h"
#include "stdio.h"
#include "JMessages.h"
#include "PatControl.h"
#include "AnalizerTrack.h"

#include "defined.h"

#define	MSG_CHA	'007a'

#include "stdio.h"
#include "XTrack.h"

const 	rgb_color  	on 	={255,52,49};
const 	rgb_color  	off 	={214,219,239};


JAnalizerTrack::JAnalizerTrack(BRect rect,int16 n,BView* v): JTrack(rect,n,v,JTRACK_MIDI,"TrackAnalizer")
{}

JAnalizerTrack::~JAnalizerTrack()
{}

int
JAnalizerTrack::getModel(){	return MY_MODEL_ID; }
void
JAnalizerTrack::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		
	case MSG_CHA:
	
	if(myTrack!=NULL) 
		
		((AnalizerTrack*)myTrack)->SetChannel(channel->getValue()-1);
	
	break;
	
	default:
		
		JTrack::MessageReceived(message);
		
	break;
	}
}

void
JAnalizerTrack::RControl()
{
	JTrack::RControl();	
	AddChild(channel=new PatControl(BRect(0,2,SBUTTON_W,21),::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),1,16,1,MSG_CHA));
}



void
JAnalizerTrack::Init(BView *t,BMessage *m)
{
	JTrack::Init(t,m);
	xtr->SetColors(on,off);
	id=m->FindInt16("id");
}
void
JAnalizerTrack::Reset(Pattern* p ,Track* tr)
{
	JTrack::Reset(p,tr);
	 SetName("Analizer");
}


