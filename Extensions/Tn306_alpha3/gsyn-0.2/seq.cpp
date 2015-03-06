// $Log: seq.cpp,v $
// Revision 1.9  1998/12/05 20:56:15  andude
// Beginning of gtk+ conversion.  A knob widget and a set of seven knobs which
// control a 303 synthesizer have been added.  An additional Sun audio
// buffering hack (from Mattias Engdegård) is also in here.
//
// Revision 1.8  1998/11/15 02:23:25  andude
// 16-note Sequencer (typically for 303) class
//
// *** empty log message ***
//
// Revision 1.7  1998/11/15 02:21:31  andude
// Stupid warnings from gcc have been eliminated.
//
// Revision 1.6  1998/11/12 23:50:45  andude
// Some fixes to voxware/OSS driver buffer sizing.
//
// Resonance tuning in vcf303.
//
// Sequencer doesn't change patterns while playing until the end of the
// current one.
//
// Revision 1.5  1998/11/10 17:41:32  andude
// Whoops!  It's twice as fast now since the stereo bit is set correctly.
//
// Revision 1.4  1998/11/08 21:46:22  andude
// Added "fun with filters" program.
//
// Revision 1.3  1998/11/08 11:50:34  andude
// The sawtooth is still very wrong, I just discovered.  It looks like a
// lowpass-filtered sawtooth is what's supposed to happen but my filter isn't
// producing "authentic" results.  Either the resonant frequency isn't at the
// cutoff on ReBirth, or there's a second filter.
//
// Also, distortion is now harmonic instead of just overdriven.  I could add a
// "shape" knob, pretty simply.
//
// Revision 1.2  1998/11/08 07:28:01  andude
// Added delay-reverb and distortion.  Very, very cool sound.  Some tuning on
// resonance still needs doing, and perhaps distortion could use some fine
// tuning as well.  Stereo effects aren't implemented yet but wouldn't be
// difficult to add.
//
// Revision 1.1  1998/11/08 03:40:26  andude
// Finished internal changes.  ubersyn.cpp is deprecated.
//

#include "seq.h"
#include "Note.h"
#include <math.h>
#include "stdio.h"

Sequencer::Sequencer()
{
	curpat=NULL;  nextpat=NULL;
	beatpos=0;
	tuneoffset=0;
	row=-1;
	SetTempo(240);
	enable=1;
}

Sequencer::~Sequencer()
{
}

void Sequencer::SetTempo(float bpm)
{
	// samples per half-quarter-beat assuming 44.1kHz
	//sampsperbeat = 22050.0*15/bpm;

	new_sxp=sampsperbeat=bpm;	// xeD says : proviamo a usare gli stessi sample per beat di Juice(/8)
		
	if(output) output->Message(TempoChange, sampsperbeat);
}

void Sequencer::ProposeTempo(float b)
{
	new_sxp=b;
}
//int min(int a, int b) { return a<b ? a:b; }
void Sequencer::Process(const float*, int len, int chan)
{
	if(!output) return;
	int buflen;
	// Break up the output into beat-sized chunks and process the pattern
	// between chunks.
	
	//printf("Seq sampleperbeat %f\n", 22050.0*15/20);
	
	while(len) {
		//buflen=min(len, (int)ceil(sampsperbeat-beatpos));
		beatpos+=buflen; 
		
		output->Process(NULL, buflen, chan);
		
		if(beatpos>=sampsperbeat) {
			
			beatpos-=sampsperbeat;
			process_half_row();
			
			if(new_sxp!=sampsperbeat)
				sampsperbeat=new_sxp;
				if(output) output->Message(TempoChange, sampsperbeat);
				
		}
		
		len-=buflen;
		
	}
	
}


void Sequencer::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case TempoChange: 
			sampsperbeat=data;
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
void Sequencer::process_half_row()
{
	int r = row>>1;
	if(row==0 && nextpat) {
		curpat=nextpat;
		nextpat=NULL;
	}
	
	//printf("xx %d\n",row);
	
	if(row<0 || !output || !curpat) return;
	
	
	
	if(enable) {
		if(row&1) {
			if(!(curpat->getNoteAt(r)->getOct()&8) && curpat->getNoteAt(r)->getNote()>=0)
				output->Message(NoteOff, 0);
		}

		else {
			if(curpat->getNoteAt(r)->getNote()>=0) {
				output->Message(NoteChange, notefreq(60+tuneoffset+curpat->getNoteAt(r)->getNote() +
					12*(curpat->getNoteAt(r)->getOct()&3)));
				output->Message(NoteOn, 10); // accent or whatever
				
			} else {
				if(!(curpat->getNoteAt(r)->getOct()&8))
					output->Message(NoteOff, 0);
			}
		}
	}

	row++; if(row>31) row=0;
	
	
}

void Sequencer::Play() { row=0; }
void Sequencer::Stop() { row=-1; Message(NoteOff, 0); }
void Sequencer::Reset(){row=0 ; beatpos=0; process_half_row();}
void Sequencer::Enable(int e)
{ 
	enable=e; 
	if(!e) Message(NoteOff, 0);
}

