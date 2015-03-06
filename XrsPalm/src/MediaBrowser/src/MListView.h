#include "OutlineListView.h"
#include "Entry.h"

class MListView : public BOutlineListView
{
	public:
		MListView(BRect r);
	virtual bool InitiateDrag(BPoint,int32,bool);
	
	
	private:
		entry_ref myref;
		
};