#include "MListView.h"
#include "MListItem.h"
#include "Window.h"
MListView::MListView(BRect r):BOutlineListView(r,"lis",B_SINGLE_SELECTION_LIST,B_FOLLOW_ALL_SIDES)
{
}

bool
MListView::InitiateDrag(BPoint p ,int32 in ,bool sel)
{
	int32 n=CurrentSelection(0);
	
	if(n<0) return false;
	
	MListItem *s=(MListItem*)ItemAt(n);

	BMessage *drag_msg=s->getMessage();
	
	if(drag_msg==NULL) return false ; //No message=FOLDER
	
	DragMessage(drag_msg,BRect(0,p.y,Bounds().Width(),p.y+s->Height()));
	return true;
}
