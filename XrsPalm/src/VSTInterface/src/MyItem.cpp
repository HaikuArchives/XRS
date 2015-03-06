#include "MyItem.h"
#include "stdio.h"
#include "Message.h"

//BRect quadro(0,0,20,20);

#define DRAG_X 1978

MyItem::MyItem(const char* n):BStringItem(n)
{
	active=true;
}

void
MyItem::ChangeStatus(){
	
	active=!active;
	//InvalidateItem();

}
void
MyItem::DrawItem(BView* w ,BRect r,bool)
{
	if(IsSelected()) {
		w->SetHighColor(222,219,222);
		
	}
	else w->SetHighColor(255,255,255);
	w->FillRect(r);
	w->SetLowColor(tint_color(w->HighColor(),B_DARKEN_1_TINT));	
	w->SetHighColor(0,0,0);
	
	w->DrawString(Text(),BPoint(r.left+30,r.top+10));
	if(active)
		w->SetHighColor(0,255,0);
		else
		w->SetHighColor(255,0,0);
	w->FillRect(BRect(r.left,r.top,r.left+20,r.top+10));
}

	

