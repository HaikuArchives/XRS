#include "SpecialTrack.h"



//#include "spanel.h"
#include "stdio.h"
//#include "Juice.h"

#include "Note.h"
#include <math.h>
#include "stdio.h"

bool re_synch;

SpecialTrack::SpecialTrack():Track()
{
	
	
	
	/* parte del Sequencer */
	
	beatpos=0;
	tuneoffset=0;
	row=-1;
	enable=1;
	
	/* Parte del SynthPanel */
	
	out=new DSPOut(44100,0, 4096); /* Questo potrebbe essere anche incorporato. */
	vco.SetOutput(&vcf);
	vcf.SetOutput(&vca);
	vca.SetOutput(&dly);
	dly.SetOutput(&mix);
	mix.SetOutput(out);
	output=&vco;
	sampsperbeat=0;
	
	winx=100;
	winy=200;
	
	playingPattern=NULL;
		
	
	SetTempo(0);
	Enable(true);
	Play();
	
	 re_synch=false;
	
}
SpecialTrack::~SpecialTrack()
{

}

int
SpecialTrack::getModel()
{
	return 1;
}

void SpecialTrack::SetTempo(float bpm)
{
	// samples per half-quarter-beat assuming 44.1kHz
	//sampsperbeat = 22050.0*15/bpm;

	//sampsperbeat=bpm;	// xeD says : proviamo a usare gli stessi sample per beat di Juice(/8)
	//if(output) output->Message(TempoChange, sampsperbeat);
}


void
SpecialTrack::Process(float** buffer,int32 samples_num,int spiaz)
{
	
	//printf("Guaglio'\n");	
	Process2(NULL,samples_num,1);
	
		
	mix.CycleInput();
	
		
	short *v=out->getBuffer();
	
	short val;
	if(note_status==0) return;
	if(row<0 || !output || !playingPattern || !curNote) return;
	
	for(int y=0;y<samples_num;y++)
	{
		
		memcpy(&val,(char*)v+y*2,2);
		
		buffer[0][spiaz+y]+=(float)val/32767.*vol_left*curNote->getVolume()*1.5;
		buffer[1][spiaz+y]+=(float)val/32767.*vol_right*curNote->getVolume()*1.5;
	}
	
}


int min(int a, int b) { return a<b ? a:b; }

void SpecialTrack::Process2(const float*, int len, int chan)
{
	if(!output) return;
	int buflen;
	// Break up the output into beat-sized chunks and process the pattern
	// between chunks.
	
	//printf("Seq sampleperbeat %f\n", sampsperbeat);
	
	while(len) {
	
		
			
		buflen=min(len, (int)ceil(sampsperbeat-beatpos));
		
		if(buflen<0) buflen=0;
			
		beatpos+=buflen; 
		
		
		//printf("buflen %d\n", buflen);
		if(buflen>0 && note_status==1)
		output->Process(NULL, buflen, chan);
		//else
		//printf("note_status =1\n");
		
		if(beatpos>=sampsperbeat) {
			
			beatpos-=sampsperbeat;
			process_half_row();
			
			
				
		}
		
		len-=buflen;
		//}
	}
	
}


void SpecialTrack::Message(SynthMessage msg, float data)
{
	int n;
	
	switch(msg) {
		case TempoChange: 
			sampsperbeat=data;
			//printf("tempo_set %d\n");
			break;
		case SystemReset:
			n=(int)data;
			
			if(n!=EMPTY_PATTERN)
			{
			 	if(playingPattern!=getPatternAt(n)) playingPattern=getPatternAt(n);
			 	curNote=playingPattern->getNoteAt(0);
			 		 	
			 }
	  			     else
			 	playingPattern=NULL;
			
			Reset();
			//row=1;
			
			return;
		break;
		default: break; // gcc is dumn
	}
	if(output) output->Message(msg, data);
}

// convert halftone number into frequency.  halftone 93 is concert A (440Hz).
static float notefreq(int halftone)
{
	return 55.0*pow(2, (float)(halftone-57)/12.0);
}

// row number is internally stored from 0..31.  Each odd "row" is only used to
// figure out when to release the note if necessary.
void SpecialTrack::process_half_row()
{
	int r = row>>1;
	/*if(row==0 && nextpat) {
		currentPattern=nextpat;
		nextpat=NULL;
	}*/
	
	
	if(row<0 || !output || !playingPattern ) {  return;}
	
	//printf("xx %d\n",row);
	
	
	if(enable) {
		if(row&1) {
			if(!(playingPattern->getNoteAt(r)->getOct()&8) && playingPattern->getNoteAt(r)->getNote()>=0)
				output->Message(NoteOff, 0);
		}

		else {
			if(playingPattern->getNoteAt(r)->getNote()>=0 && playingPattern->getNoteAt(r)->getValue() ) 
				
				{
			
				output->Message(NoteChange, notefreq(tuneoffset+playingPattern->getNoteAt(r)->getNote()));
				output->Message(NoteOn, 10); // accent or whatever
				
			} else {
				if(!(playingPattern->getNoteAt(r)->getOct()&8))
					output->Message(NoteOff, 0);
			}
		}
	}

	row++; if(row> playingPattern->getNumberNotes()*2-1) { row=0 ;  }
	
	
}
void
SpecialTrack::NoteReady(int notenumber,int patternnumber,float sp)
{
	
	if(patternnumber!=EMPTY_PATTERN)
	{
		if(playingPattern!=getPatternAt(patternnumber)) playingPattern=getPatternAt(patternnumber);
		curNote=playingPattern->getNoteAt(notenumber);
		
		if(notenumber==0)
		{
			row=0;
			process_half_row();
		}
		
		row=notenumber*2+1; // +1;
			
	}
	
	else {
		row=-1;
		playingPattern=NULL;
	       }
	 
	if(sampsperbeat==0) { sampsperbeat=sp/8;}
		
	if(sp/8 !=sampsperbeat || re_synch==true)
	{
		sampsperbeat=sp/8;
		 beatpos=0; 
		note_status=0;
		if(output) output->Message(TempoChange, sampsperbeat);
		re_synch=false;
	}
	else			
		note_status=1;
		
			
}
void SpecialTrack::Play() { row=0; }
void SpecialTrack::Stop() { row=-1; Message(NoteOff, 0); }


void SpecialTrack::Reset(){

	/* clearup buffers!*/
	vco.Message(SystemReset,0);
	mix.Reset();
	out->Reset();

	row=0 ; 
	beatpos=0; 
	process_half_row();
	enable=true; 

}

void
SpecialTrack::goOn()
{ re_synch=true;}
void
SpecialTrack::goOff()
{ re_synch=true; }

void SpecialTrack::Enable(int e)
{ 
	enable=e; 
	if(!e) Message(NoteOff, 0);
}