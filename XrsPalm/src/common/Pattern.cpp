#include "Pattern.h"
#include "Note.h"
#include "List.h"


Pattern::Pattern (int n)
{
	noteList=new BList(1);
	for(int h=0 ;  h<n ; h++)   noteList->AddItem((void*)new Note());
}

Note*
Pattern::getNoteAt(int val)
{
	return (Note*)noteList->ItemAt(val);
}

int
Pattern::getNumberNotes()
{
	return noteList->CountItems();
}
void
Pattern::setNumberNotes(int n)
{
	
	if(n<getNumberNotes())
	{
		for(int h=getNumberNotes()-1;h>=n;h--)
			noteList->RemoveItem(h);	
	}
	else
	if(n>getNumberNotes())
	{
		for(int h=getNumberNotes();h<n;h++)
			noteList->AddItem((void*)new Note());
	}
	
}
