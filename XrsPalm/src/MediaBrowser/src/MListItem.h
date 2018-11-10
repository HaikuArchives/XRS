#include "ListItem.h"
#include "Entry.h"

class MListItem: public BStringItem
{
	public:
		MListItem(entry_ref*,const char* name,bool isFolder,int32 spiaz,int32 size);
		BMessage*	getMessage();
	private:
		BMessage	*msg;
}; 
