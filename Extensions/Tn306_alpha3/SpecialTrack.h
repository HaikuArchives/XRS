#ifndef _SPECIAL_TRACK_H_
#define _SPECIAL_TRACK_H_

#include "Track.h"
#include "synel.h"
#include "Pattern.h"
#include "dspout.h"
#include "vco303.h"
#include "vcf303.h"
#include "vca303.h"
#include "deverb.h"

class SpecialTrack: public Track
{
	public:
	
				SpecialTrack();
				~SpecialTrack();
	virtual void		Reset();
virtual	void		Process(float**,int32,int);
virtual	void		NoteReady(int,int,float);
virtual	int		getModel();
virtual	void		goOn();
virtual	void		goOff();
//virtual	void		goOff();

		void 		Tune(int halfsteps) { tuneoffset=halfsteps; }
		int 		getTune() { return tuneoffset;}

		VCO303 vco;
		VCF303 vcf;
		VCA303 vca;
		Deverb dly;
		
		float winx,winy;
		
	//void Process(int size) { vco.Process(NULL, size, 1);

	private:
			
			int		note_status;
	
			//Pattern *curpat, *nextpat;
			float sampsperbeat, beatpos;
			int tuneoffset;
			int enable;
			Element *output;
			float new_sxp;
			
				void process_half_row();
			int row;
				void SetTempo(float bpm);  // Should be obvious.

				void Play(); 
				void Stop(); 
				// Change patterns.  If playing, doesn't actually change until end of
				// currently playing pattern.
				//void SetPattern(Pattern *p) {nextpat=p; }

				int IsEnabled(void) { return enable; }
				void Enable(int e);

				

	// Synth Element stuff
	virtual void Process2(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
	
	/* da synth panel */
	
	Mixer mix;
		
		Pattern*	playingPattern;	

	DSPOut	*out;
};

#endif