#include "XMName.h"
#include "math.h"
#include "stdio.h"
#include "Window.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "Message.h"
#include "JMessages.h"
#include "GfxMsg.h"


#include "String.h"



#define 	XBOX		18
#include "maxcount.h"


/*

cose da fare:

1) init con la canzone!


*/

char nu[4];


XMName::XMName(BRect r):BView(r,"",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW)
{

}

void
XMName::Reset(Sequence* s)
{
	sequence=s;
	
				
	curPat=0;
	sel=0;
	y_count=s->getMaxPat();
	MaxPat=0;
	
	
	
	if(Window()->Lock())
	{
		//setMaxs(sequence->getMaxSeq()+1,0);
		Invalidate();
		Window()->Unlock();
	}
}
void
XMName::AttachedToWindow()
{
	SetViewColor(200,200,220);
	SetLowColor(200,200,220);
	SetFontSize(12);
}


int
XMName::getStep()
{
	return curPat;	
}
void
XMName::Select(int y)
{
	Invalidate(BRect(0,XBOX*sel,Bounds().right-Bounds().left,XBOX*(sel+1)));
	sel=y;
	Invalidate(BRect(0,XBOX*sel,Bounds().right-Bounds().left,XBOX*(sel+1)));

}

void
XMName::Draw(BRect r)
{
	if(sequence==NULL) return;
	
	int	ax1=(int)floor(r.left/XBOX)-1;
	int	ay1=(int)floor(r.top/XBOX)-1;
	int	ax2=(int)ceil(r.right/XBOX)+1;
	int	ay2=(int)ceil(r.bottom/XBOX)+1;
	
	for(int y=ay1;y<ay2;y++)
	
	{
		if(y<y_count)
		{
			if(y==sel)
			{
				SetHighColor(255,227,153);	
			FillRect(BRect(ax1*XBOX,sel*XBOX,ax2*XBOX+XBOX,sel*XBOX+XBOX-1));
			SetLowColor(255,227,153);
		}
		else SetLowColor(200,200,220);
		SetHighColor(0,0,0);
		
		//DrawString("Measure  ",BPoint(6,y*XBOX+12));
		BString* s=sequence->GetMeasureName(y);
		if(s!=NULL){
		sprintf(nu,"%2d",y+1);
		DrawString(nu,BPoint(3,y*XBOX+12));
		DrawString(": ");
		DrawString(s->String());
		
		} 
	
	}
 
	}
}
void
XMName::MouseDown(BPoint p)
{
	if(sequence==NULL) return;
	
	int	ay1=(int)floor(p.y/XBOX);
	
	int32 key;
	
	BString* text=sequence->GetMeasureName(ay1);
	
	BMessage *m=Window()->CurrentMessage();
	 m->FindInt32("modifiers",&key);
	 
	
	if( ( key & B_CONTROL_KEY)  && m!=NULL && text!=NULL) {
	BRect n(ConvertToScreen(BRect( Bounds().left,ay1*XBOX,Bounds().right,(ay1+1)*XBOX)));
	BMessage* m=new BMessage('ttty');
	m->AddPoint("from",BPoint(n.left,n.top));
	m->AddPoint("to",BPoint(n.right,n.bottom));
	m->AddInt16("pos",ay1);
	m->AddString("name",text->String());
	
	Looper()->PostMessage(m,Parent());
	
	return;
	
	}
	
	if(ay1!=sel &&  ay1<y_count)
	{
		BMessage* notify=new BMessage(FALSEPAT_MSG);
		notify->AddInt16("pos",ay1+1);
		Window()->PostMessage(notify);
	}
	
}

void
XMName::setPosition(int val)
{
	curPat=val;
	Invalidate();
}

