#include "MidiOutTrack.h"

#include "stdio.h"
#include "Note.h"
#include <math.h>
#include "stdio.h"
#include "Pattern.h"
#include "Note.h"



//BMidiLocalConsumer *outer;
#include "MidiProducer.h"
//#include "MidiConsumer.h"

MidiOutTrack::MidiOutTrack(BMidiLocalProducer *m):Track()
{
	mider=m;
	curNote=NULL;
	channel=0;
	latency=100;
	
}
MidiOutTrack::~MidiOutTrack()
{

}

int
MidiOutTrack::getModel()
{
	return 2;
}




void
MidiOutTrack::Process(float** buffer,int32 samples_num,int spiaz)
{
}



void MidiOutTrack::Message(SynthMessage msg, float data)
{
	//int n;
	
	switch(msg) {
		case TempoChange: 
			
			break;
		case SystemReset:
					
			mider->SprayProgramChange(channel,0);
			//curNote=NULL;
		break;
		case	SystemStop:
			mider->SprayControlChange(channel,123,0,latency);
			curNote=NULL;
		break;
		default: break; // gcc is dumn
	}
	
}


void
MidiOutTrack::NoteReady(int notenumber,int patternnumber,float sp)
{
	
	
	
	
	if(patternnumber!=EMPTY_PATTERN)
	{
		if(curNote !=NULL ) 
			if(playingPattern->getNoteAt(notenumber)->getValue()) {
				mider->SprayNoteOff(channel,curNote->getNote(),curNote->getVolume()*100,latency);
			//printf("Midi off note %ld\n",curNote->getNote());
		}
		
		
		if(playingPattern!=getPatternAt(patternnumber)) 
			
			playingPattern=getPatternAt(patternnumber);
		
		
		
		if(playingPattern->getNoteAt(notenumber)->getValue()) 
			{
				//int k=curNote->getOct()&7;
				//printf();
				//k=k-1;
				curNote=playingPattern->getNoteAt(notenumber);
				mider->SprayNoteOn(channel,curNote->getNote(),curNote->getVolume()*100,latency);	
				
			}	
	}
		
}

void
MidiOutTrack::SetChannel(int n)
{
	channel=n;
}

void MidiOutTrack::Reset(){

}

void
MidiOutTrack::goOn()
{ }
void
MidiOutTrack::goOff()
{ }
