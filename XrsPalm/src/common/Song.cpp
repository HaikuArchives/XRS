

#include "Song.h"
#include "Track.h"
//#include "BankManager.h"
#include "stdio.h"
#include "Xed_Utils.h"
#include "math.h"
#include "Sequence.h"
#include "String.h"
	
Song::Song()
{
	trk=new BList(1);
	tempo_bpm=120;
	modified=false;
	setTempo(tempo_bpm);
	file_entry=NULL;
	setCutoff(256);
	setResonance(256);
	setFilterIsOn(false);
	
	filter_on=false;
	
	beateach=4;
	
	all=false;
	popupdesc=true;
	
	sequence=new Sequence();
	sequence->Reset();
	
	num_notes=NUM_NOTES;
	
	AddMeasure();		// a song MUST have at least one Measure!
}


void
Song::setNumberNotes(int n)
{
	num_notes=n;
	
	for(int h=0;h<getNumberTrack();h++)
	{
		getTrackAt(h)->setNumberNotes(n);
	}
	//do all!
}
void
Song::AddMeasure()
{
	// polling su tutte le traccie..
	for(int h=0;h<getNumberTrack();h++)
	{
		getTrackAt(h)->AddMeasure(1,getNumberNotes());
	}
	//getSequence()->setMaxPat(getSequence()->getMaxPat()+1);	
	getSequence()->AddMeasure("Measure");
	//printf("Current Song num_mea %d\n",getSequence()->getMaxPat());
}
int
Song::getIndexOfTrack(Track* tx)
{ 
	return trk->IndexOf((void*)tx);
}

status_t
Song::RemoveMeasure(int num)
{
	if(getSequence()->getMaxPat()-1<1) return B_ERROR;
	
	// polling su tutte le traccie..
	for(int h=0;h<getNumberTrack();h++)
	{
		getTrackAt(h)->RemoveMeasure(num);
	}
	getSequence()->RemoveMeasure(num);
	getSequence()->setMaxPat(getSequence()->getMaxPat()-1);	
	
	return B_OK;
}


void
Song::AddTrack(Track* j)
{
	//j=new SpecialTrack();
	j->AddMeasure(getNumberMeasure(),getNumberNotes());
	j->moveToPattern(0);
	
	//if(getBKM()!=NULL)	j->setSample(bkm->getSampleAt(0));
	
	 trk->AddItem((void*)j);
	 
}


void
Song::RemoveTrack(Track* t)
{
	//Track* t;
	//t= (Track*)trk->ItemAt(p);
	int p=trk->IndexOf((void*)t);
	//printf("Song:: rmove track id %d\n",p);
	trk->RemoveItem(p);
	delete t;
	
}

int
Song::MeltTrack(Track* j )
{
	 trk->AddItem((void*)j);
	 return trk->IndexOf((void*)j);
	
}

int
Song::getNumberTrack()
{
	return trk->CountItems();
}

/*
void
Song::setVSTName(const char *d,int v)
{
	strncpy(vstname[v],(char*)d,100);
}

void
Song::getVSTName(const char *d,int v)
{
	strncpy((char*)d,vstname[v],100);
}

void
Song::setVSTIsOn(bool v,int l)
{
	vst_on[l]=v;
}
*/

// filter


bool
Song::isFilterOn()
{	
	return filter_on;
}

Song::~Song()
{
	for(int h=0;h<getNumberTrack();h++)
		delete 	getTrackAt(h);
		
	//delete bkm;
}
bool
Song::isModified()
{
	return modified;	
}
void
Song::setModified(bool t)
{
	modified=t;
}
Track*
Song::getTrackAt(int val)
{
	return (Track*)trk->ItemAt(val);
	
}
int
Song::getCurrentMeasure()
{
	return getSequence()->getMaxPat();
}

int
Song::getNumberMeasure()
{
	return getSequence()->getMaxPat();
	//return measure=getSequence()->getMaxPat()+1;
}

void
Song::setNumberMeasure(int val)
{
	getSequence()->setMaxPat(val);
	//measure=val;
}
void
Song::Init()
{

	
	for(int h=0;h<getNumberTrack();h++)
		{
	             getTrackAt(h)->moveToPattern(0);
		}
			
}
void
Song::ResetSamples()
{
	for(int h=0;h<getNumberTrack();h++)
		{
			
			//getTrackAt(h)->setType(UNSET_TYPE);			
	            }
}

void
Song::setTempo(int bpm)
{
	//bpm=message->FindInt32("be:value");
	tempo_bpm=bpm;
	note_size=(size_t)2646000/bpm;
	while(note_size % 4 !=0) note_size++;	
}
int
Song::getTempo()
{
	return tempo_bpm;
}
size_t
Song::getNoteSize()
{
	return note_size;
}
BEntry*
Song::getEntry()
{
	return file_entry;
}
void
Song::setEntry(BEntry* s)
{
	file_entry=s;
}
void
Song::setCutoff(float x)
{	// da ottimizzare!
	cutoff=(int32)x;
	vcf_cutoff=((x*0.07)/256.)+0.000050;
}
void
Song::setResonance(float x)
{	// da ottimizzare!
	resonance=(int32)x;
	vcf_resonance=((x*0.6)/256.)+0.01;
}



