#include "Track.h"
#include "Pattern.h"
#include "Note.h"
#include "List.h"
#include "math.h"
   
Track::Track ()
{

	patternList=new BList(0);
	pan=0;
	volume=0.8;
	on=true;
	fx1=false;
	type=UNSET_TYPE;
	vst_line=0;
	CalcVols();
	curNote=NULL;
	name_set=false;
}

void
Track::setNumberNotes(int n)
{
	for(int h=0 ;  h<patternList->CountItems(); h++)  
	
		((Pattern*)patternList->ItemAt(h))->setNumberNotes(n);
}


void
Track::setName(const char* t)
{
	if(!name_set)	name.SetTo(t);
}
const char*
Track::getPreferredName()
{
	return "unset_";
}
void
Track::ResetName()
{
	if(getType()==UNSET_TYPE)
		name.SetTo("UnSet!");
	else
		name.SetTo(getPreferredName());
		
	name_set=false;
}
void
Track::LockName(bool b)
{
	name_set=b;
}
const char*
Track::getName()
{
	return name.String();
}
bool
Track::isNameLocked()
{
	return name_set;
}
void 
Track::Message(SynthMessage msg, float data)
{
	switch(msg) {
		
		case SystemReset:
			Reset();
			return;
		break;
		default: break; // gcc is dumn
	}
}

void
Track::AddMeasure(int val,int note)
{
	for(int h=0 ;  h<val; h++)  
	
		 patternList->AddItem(new Pattern(note));
		 
	currentPattern=(Pattern*)patternList->ItemAt(0);	 
}
void
Track::RemoveMeasure(int num)
{
	 patternList->RemoveItem(num);
		 
	currentPattern=(Pattern*)patternList->ItemAt(0);	 
}

Track::~Track()
{
}
int
Track::getModel()
{
	return 0;
}

float
Track::getVolume()
{
	return volume;
}	
void
Track::setVolume(float val)
{
	volume=val;
	CalcVols();
}
float
Track::getPan()
{
	return pan;
}
void
Track::setPan(float val)
{
	pan=val;
	CalcVols();
}

bool
Track::isOn()
{
	return on;
}
void
Track::setOn(bool val)
{
	on=val;
	if(val) goOn(); else goOff();
}
bool
Track::isFX1On()
{
	return fx1;
}
void
Track::setFX1On(bool val)
{
	fx1=val;
}
void
Track::moveToPattern(int val)
{
	currentPattern=(Pattern*)patternList->ItemAt(val);	
}
int
Track::getFX1Line()
{
	return vst_line;
}
void
Track::setFX1Line(int v)
{
	vst_line=v;
}	

Pattern*
Track::getPatternAt(int val)
{
	return (Pattern*)patternList->ItemAt(val);		
}

Pattern*
Track::getCurrentPattern()
{
	return currentPattern;
}

int
Track::getType()
{
	return type;
}
void
Track::setType(int val)
{
	type=val;
}
void
Track::CalcVols()
{
	vol_left=volume;
	vol_right=volume;
	
	
	if(pan>0) vol_left=volume*(1.-pan);
		
		else
			if(pan<0) vol_right=volume*(1.+pan);
			
}

void
Track::Reset()
{
	curNote=NULL;
}

void Track::NoteReady(int notenumber,int patternnumber,float samplesperbeat){}
void Track::Process(float ** buffer ,int32 sample_num,int spiaz){}