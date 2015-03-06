#include "MyDrop.h"
#include "stdio.h"
#include "Message.h"

//BRect quadro(0,0,20,20);

#define DRAG_X 1978

MyDrop::MyDrop(BRect r):BView(r,"",B_FOLLOW_NONE,B_WILL_DRAW)
{
	k=100;
}

void
MyDrop::Draw(BRect r)
{
	SetHighColor(k,k,k);
	FillRect(r);
}
void
MyDrop::MessageReceived(BMessage *msg)
{
	if(msg->what==DRAG_X){
	k=0;
	Invalidate();
	}

}
void
MyDrop::MouseMoved(BPoint,uint32 mode,const BMessage *msg)
{
	if(mode==B_ENTERED_VIEW && msg!=NULL)
	{
		k=0;
		Invalidate();
	}
	else
	if(mode==B_EXITED_VIEW && msg!=NULL)
	{
		k=100;
		Invalidate();
	}
}
	

