#include "MySource.h"
#include "stdio.h"

#define DRAG_X 1978

MySource::MySource(BRect r):BListView(r,"")
{}

bool
MySource::InitiateDrag(BPoint p ,int32 in ,bool sel)
{		
	float  yn=(float)((int)(p.y/h))*(float)h;
	if(p.y>CountItems()*h) yn=CountItems()*h;
	int n=(int)(yn/h);
	if(ItemAt(n)==NULL) return false;
	
	BMessage *drag_msg=new BMessage(DRAG_X);
	
	drag_msg->AddPoint("click_location",p);
	drag_msg->AddInt16("from",-1);
	drag_msg->AddString("name",((BStringItem*)ItemAt(n))->Text());
	drag_msg->AddInt16("pat",n+1);
	
	DragMessage(drag_msg,BRect(0,p.y,w,p.y+h));
	
	return true;
}
void
MySource::Init()
{
	h=ItemFrame(0).bottom-ItemFrame(0).top+1;
	w=ItemFrame(0).right-ItemFrame(0).left;
}
