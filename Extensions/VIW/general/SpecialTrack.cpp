#include "SpecialTrack.h"
#include "stdio.h"
#include "Note.h"
#include <AppKit.h>
#include <StorageKit.h>
#include "stdio.h"
#include "malloc.h"
#include "VSTInstrument.h"

#include "PlugWindow.h"
#include "InterfaceKit.h"

#define	 FRAMES_NUM	1024


SpecialTrack::SpecialTrack(const char * path):Track()
{
	stream_l=new float[ FRAMES_NUM];
	stream_r=new float[ FRAMES_NUM];
	stream_s[0]=stream_l;
	stream_s[1]=stream_r;

	
	type=1978;	
	ResetName();

	viw=new VSTInstrumentPlugin(path);
	win=NULL;
	settings=NULL;
}	

void SpecialTrack::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case TempoChange: 
			// maybe it should be send?
			break;
		case SystemReset:
			//printf("Stop!\n");
			//return;
		break;
		
		case	SystemStop:
			viw->StopAll();
		break;
		
		default: break; // gcc is dumn
	}

}

void 
SpecialTrack::Reset(){type=1978;viw->Configure();}

SpecialTrack::~SpecialTrack(){

		if(win!=NULL)
		if(win->Lock())
			win->Quit();
}

int
SpecialTrack::getModel(){return 10;}

const char*
SpecialTrack::getPreferredName() { return "VIW"; } 

void
SpecialTrack::Process(float** buffer,int32 samples_num,int spiaz)
{	
	memset((char*)stream_s[0], 0, samples_num*4);
	memset((char*)stream_s[1], 0, samples_num*4);

	
	viw->FilterFloat(stream_s,stream_s,samples_num,NULL);
	
		
	for (int L=0; L< samples_num; L++) {
			
		buffer[0][spiaz+L]+=stream_s[0][L]*vol_right;
		buffer[1][spiaz+L]+=stream_s[1][L]*vol_left;
	
	}
}

void
SpecialTrack::NoteReady(int notenumber,int patternnumber,float sp)
{
	if(patternnumber!=EMPTY_PATTERN)
	{
		if(playingPattern!=getPatternAt(patternnumber)) playingPattern=getPatternAt(patternnumber);
		curNote=playingPattern->getNoteAt(notenumber);
		
		// send the note to the VSTi
		if(curNote->getValue())
		{
				viw->SendNote(curNote->getNote(),curNote->getVolume());
				//printf("Vslue %d\n",curNote->getNote());
		}
			
	}
	
	else {
		playingPattern=NULL;
	       }
}


void
SpecialTrack::goOn()
{ }
void
SpecialTrack::goOff()
{  
	viw->StopAll();
}