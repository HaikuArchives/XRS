#include "ListItem.h"
#include "Entry.h"

class MListItem: public BStringItem
{
	public:
		MListItem(entry_ref*,const char* name,bool isFolder=false,int32 spiaz=-1,int32 size=-1);
		BMessage*	getMessage();
	private:
		BMessage	*msg;
}; 