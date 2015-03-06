#include "Note.h"

Note::Note ()
{
	value=false;
	volume=0.8;
	oct=0;
	note=60;
}
float
Note::getVolume()
{
	return volume;
}
void
Note::setVolume(float val)
{
	volume=val;	
}
bool
Note::getValue()
{
	return value;	
}

void 
Note::setValue(bool val)
{
	value=val;		
}

void
Note::setOct(int val)
{
	oct=val;
}
int
Note::getOct()
{
	return oct;	
}
void
Note::Swap()
{
	value=!value;
}