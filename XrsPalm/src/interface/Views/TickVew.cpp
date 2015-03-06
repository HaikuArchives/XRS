// TickView 
//	23-7-00
//

#include "TickView.h"
#include <Window.h>
#include "stdio.h"
#include "GlobalDef.h"


TickView::TickView(BRect frame,int d): BView(frame,":)",B_FOLLOW_BOTTOM,B_WILL_DRAW|B_FRAME_EVENTS)
{
	xinc=23;
	tick=-1;
	space=10;
	num_notes=16;
	
}
void 
TickView::SetNumberNotes(int d)
{
	
	/*if(d>NUM_NOTES) num_notes=d;
	else	num_notes=NUM_NOTES;*/
	
	num_notes=d;
	
	ResizeTo(23*num_notes+2,Frame().Height());
		
	Invalidate();
}
void
TickView::Draw(BRect b)
{
	
		
		for(int i=0;i<num_notes;i++)
	{
		
		SetPenSize(2);
		StrokeEllipse(BPoint(space+i*xinc,9),5,5);
		SetPenSize(1);
	
	}
}

void
TickView::SetTick(int c,int a,int b)
{
	
	if(c<0){
		Invalidate();
		return;
	}
	
	rgb_color old_c=HighColor();
	
	tick=c;
	
	
	
	if(Window()->Lock())
	{
		
		if(a==b)
			SetHighColor(255,10,0,255); 
		   else
		   	SetHighColor(155,10,156,255); 
		   	
		FillEllipse(BRect(space+tick*xinc-4,5,space+tick*xinc+4,13));
		SetHighColor(ViewColor()); 
		if(tick!=0)
		{  
			FillEllipse(BRect(space+(tick-1)*xinc-4,5,space+(tick-1)*xinc+4,13));
		}
		
		else
		
		{
			FillEllipse(BRect(space+(num_notes-1)*xinc-4,5,space+(num_notes-1)*xinc+4,13));
		}
		
		Window()->Unlock();
		}
	
	
	SetHighColor(old_c);
	
}

void
TickView::AttachedToWindow()
{
	if(Parent())
			SetViewColor(123,162,173,255);
	//	SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

