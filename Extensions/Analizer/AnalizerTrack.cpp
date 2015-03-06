#include "AnalizerTrack.h"
#include "stdio.h"
#include "Note.h"
#include <math.h>
#include "stdio.h"
#include "Pattern.h"
#include "Note.h"
#include "defined.h"

#include "JAnalizerTrack.h"

AnalizerTrack::AnalizerTrack():Track()
{
	
	curNote=NULL;
	channel=0;
	latency=100;
	jat==NULL;
	
}
AnalizerTrack::~AnalizerTrack()
{

}

int AnalizerTrack::getModel(){	return MY_MODEL_ID;}

void AnalizerTrack::Process(float** buffer,int32 samples_num,int spiaz){

	
if(jat!=NULL)
	jat->DrawBuffer(buffer,samples_num,spiaz);

}



void AnalizerTrack::Message(SynthMessage msg, float data)
{
	//int n;
	
	switch(msg) {
		case TempoChange: 
			
			break;
		case SystemReset:
					
			
		break;
		case	SystemStop:
			
			curNote=NULL;
		break;
		default: break; // gcc is dumn
	}
	
}


void
AnalizerTrack::NoteReady(int notenumber,int patternnumber,float sp)
{
	
	
	
	
	if(patternnumber!=EMPTY_PATTERN)
	{
		if(curNote !=NULL ) 
			if(playingPattern->getNoteAt(notenumber)->getValue()) {
			//	mider->SprayNoteOff(channel,curNote->getNote(),curNote->getVolume()*100,latency);
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
				//mider->SprayNoteOn(channel,curNote->getNote(),curNote->getVolume()*100,latency);	
				
			}	
	}
		
}

void
AnalizerTrack::SetChannel(int n){	channel=n;}

void AnalizerTrack::Reset(){}
void AnalizerTrack::goOn(){}
void AnalizerTrack::goOff(){}
