#include "MyDrag.h"

#include "Message.h"
BRect quadro(0,0,20,20);

#define DRAG_X 1978

MyDrag::MyDrag(BRect r):BView(r,"",B_FOLLOW_NONE,B_WILL_DRAW)
{
}

void
MyDrag::Draw(BRect r)
{
	SetHighColor(0,0,0);
	FillRect(r);
}
void
MyDrag::MouseDown(BPoint where){

	BMessage *drag_msg = new BMessage(DRAG_X);
	
	drag_msg->AddPoint("click_location",where);
	
	drag_msg->AddInt16("from",-1);
	drag_msg->AddInt16("to",-1);
	
	DragMessage(drag_msg,quadro,this);
	
	delete drag_msg;	
}

