#ifndef _SONG_H_
#define _SONG_H_

#include "GlobalDef.h"
#include "SupportKit.h"
#include "List.h"

#include <Entry.h>

class Track;
//class BankManager;
//class ExternalManager;
class Sequence;
class BString;

class Song
{
	public:
					Song ();
					~Song();

		void			Init();
		void			ResetSamples();

		void			AddTrack(Track*);
		void			RemoveTrack(Track*);
		int			MeltTrack(Track*);
		Track*			getTrackAt(int);
		int			getNumberTrack();
		int			getIndexOfTrack(Track* tx);

		int			getNumberNotes(){return num_notes;};
		void			setNumberNotes(int);
		size_t			getNoteSize(); //in frame

		void			setTempo(int bpm); //in bpm
		int			getTempo();

		BEntry*		getEntry();
		void			setEntry(BEntry*);
		void			setModified(bool);
		bool			isModified();

		// Pattern

		void			AddMeasure();
		status_t		RemoveMeasure(int num);
		int			getCurrentMeasure();
		int			getNumberMeasure();
		void			setNumberMeasure(int);

		// Sequence

		Sequence*		getSequence(){return sequence;};

		// filter

		void			setResonance(float val);
		void			setCutoff(float val);

		int32			getResonance(){return resonance;};
		int32			getCutoff(){return cutoff;};

		float			getVCFResonance(){return vcf_resonance;};
		float			getVCFCutoff(){return vcf_cutoff;};

		void			setFilterIsOn(bool v){filter_on=v;};
		bool			isFilterOn();

		void			SetBeatEach(int v){beateach=v;};
		int			GetBeatEach(){return beateach;};

		bool			all;
		bool			popupdesc;

		const char*		getDescription() {return description.String(); };
		void			setDescription(const char*d ){description.SetTo(d);};
		size_t			getSizeDescription(){return description.Length();};

	private:


			BList*			trk;
			BString			description;
			Sequence*		sequence;
			size_t			note_size;
			int			tempo_bpm;
			BEntry*		file_entry;
			bool			modified;
			bool 			filter_on;
			int32			cutoff,resonance;
			float			vcf_cutoff,vcf_resonance;
			int			beateach;
			//Vst
			//char			vstname[2][100] ;// ?? BufferOverflow!!
			//bool			vst_on[2];	//Number dei vst!
			int			num_notes;
};

#endif
