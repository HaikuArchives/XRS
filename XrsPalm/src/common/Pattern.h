#ifndef _PATTERN_
#define _PATTERN_

#include "GlobalDef.h"

class Note;
class BList;

class Pattern
{
	public:
	
		Pattern(int);
		
	Note*	getNoteAt(int);
	void	setNoteAt(int,bool);				
	
		int getNumberNotes();
		void setNumberNotes(int);
	private:
	
		//Note	*noteList[NUM_NOTES];
		BList	*noteList;
};

#endif 