#include "MyList.h"
#include "Window.h"
#include "stdio.h"
#include "VSTWindow.h"
#include "MyItem.h"

#define DRAG_X 1978

MyList::MyList(BRect r):BListView(r,"")
{
	y=-1;
}

bool
MyList::InitiateDrag(BPoint p ,int32 in ,bool sel)
{
	//printf("Moved  -> \n");
	if(CountItems()<=0 ) return false;
	
	float  yn=(float)((int)(p.y/h))*(float)h;
		
	if(p.y>CountItems()*h) yn=CountItems()*h;
		
		
	int n=(int)(yn/h);
	
	if(ItemAt(n)==NULL) return false;
	
	BMessage *drag_msg=new BMessage(DRAG_X);
	
	drag_msg->AddPoint("click_location",p);
	drag_msg->AddInt16("from",n);
	drag_msg->AddInt16("to",-1);
	drag_msg->AddString("name",((BStringItem*)ItemAt(n))->Text());
	
	
	
	DragMessage(drag_msg,BRect(0,p.y,w,p.y+h));
	return true;
}
void
MyList::Init(float hx)
{
	h=hx;
	w=75;
}
void
MyList::MouseDown(BPoint p)
{
	if(p.x<20){
	int k=(int)(p.y/h);
	if(k<CountItems() && k>=0)
	{
		((MyItem*)ItemAt(k))->ChangeStatus();
		InvalidateItem(k);
		((VSTWindow*)Window())->SetVstState(k,((MyItem*)ItemAt(k))->GetStatus());
	}
	
	}
	else
	BListView::MouseDown(p);
}
void
MyList::MouseMoved(BPoint p,uint32 mode,const BMessage *msg)
{
	
	if(msg!=NULL)
	{

		//printf("Moved  -> \n");
		if(mode==B_EXITED_VIEW)	yn=-1;
		else
			 yn=(float)((int)(p.y/h))*(float)h;
		
		if(p.y>CountItems()*h) yn=CountItems()*h;
		
			
		if(yn!=y){ y=yn;  Invalidate(); }
	}
		
		
}
void
MyList::Draw(BRect r)
{
	DeselectAll();
	BListView::Draw(r);
	if(y>0){
	SetHighColor(0,0,0);
	StrokeLine(BPoint(r.left,y),BPoint(r.right,y));
	StrokeLine(BPoint(r.left,y+1),BPoint(r.right,y+1));
	
	}
}
void
MyList::MessageReceived(BMessage *msg)
{
	if(msg->what==DRAG_X){
	
	BPoint p;
	BListItem *from;
	msg->FindPoint("click_location",&p);
	int k=msg->FindInt16("from");
	
	//printf("Moved  -> %d\n",k);
	int n=(int)(y/h);
	//int n=y;
	if(k>=0)
	{
		from=ItemAt(k);
				
		from->Deselect();
		
		if(k>=n)
		{
			RemoveItem(from);
			AddItem(from,n);
			//printf("MovedUp  %d -> %d\n",k,n);
			((VSTWindow*)Window())->MoveVst(k,n);
		}
		else
		{
			AddItem(from,n);
			RemoveItem(from);
			//printf("Moved Down  %d -> %d\n",k,n-1);
			((VSTWindow*)Window())->MoveVst(k,n-1);
			
		}
	}
	
	else
	{
		sprintf(mea,msg->FindString("name"));
		AddItem(new MyItem(mea),n);
		//printf("Added  in list 0 as %d\n",n);
		((VSTWindow*)Window())->AddVst(msg->FindString("name"),0,n);
	}
	y=-1;
	
	Invalidate();
	
	}

	else
	
		BListView::MessageReceived(msg);

}