#include "XMPoz.h"
#include "stdio.h"
#include "Window.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "Message.h"
#include "GfxMsg.h"


#define 	XBOX		18
#include "maxcount.h"


/*

cose da fare:

1) init con la canzone!


*/

char na[3];


XMPoz::XMPoz(BRect r):BView(r,"",B_FOLLOW_TOP|B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW)
{
	
	
}

void
XMPoz::Reset(Sequence* s)
{
	sequence=s;
	curPat=0;
	
	if(Window()->Lock())
	{
		Invalidate();
		Window()->Unlock();
	}
}
void
XMPoz::AttachedToWindow()
{
	SetViewColor(200,200,220);
	SetLowColor(200,200,220);
	SetFontSize(7);
}





void
XMPoz::Draw(BRect r)
{
	if(sequence==NULL) return;
	
		
	int	ax1=(int)floor(r.left/XBOX)-1;
	int	ay1=0;
	int	ax2=(int)ceil(r.right/XBOX)+1;
	int	ay2=18;
	
	int y_count=0;
	
	SetHighColor(0,0,0);
	if (ax2>X_COUNT) ax2=X_COUNT;
	
	int maxseq=sequence->getMaxSeq();
	
	 for(int x=ax1;x<ax2;x++)
	{
	
			
				
				if(x==maxseq && maxseq>=0)
					{
						SetHighColor(0,0,255);
						FillRect(BRect(x*XBOX+1,12,x*XBOX+XBOX,15));
						SetHighColor(0,0,0);	
						
						
					}
						
					
				if(x==curPat)
					{
						SetHighColor(255,0,0);
						FillRect(BRect(x*XBOX+1,8,x*XBOX+XBOX,11));
						SetHighColor(0,0,0);	
						
					}
						
				
				StrokeLine(BPoint(x*XBOX,0),BPoint(x*XBOX,XBOX));
				sprintf(na,"%2d",x+1);
				DrawString(na,BPoint(x*XBOX+3,8));
				
			

		
	}
		
}
	
	
	
	
/*
void
XMPoz::MouseDown(BPoint p)
{
	if(sequence==NULL) return;
	
	int	ay1=(int)floor(p.y/XBOX);
	
	//printf("ay1 %d , sel %d , y_count %d \n",ay1,sel,y_count);
	
	if(ay1!=sel &&  ay1<y_count)
	{
		BMessage* notify=new BMessage(FALSEPAT_MSG);
		notify->AddInt16("pos",ay1+1);
		Window()->PostMessage(notify);
	}
	
}*/

void
XMPoz::setMaxs(int seq ,int pat)
{
	int k;
	k=sequence->getMaxSeq();
	
	//printf("k : %d -  \n",k);
	
	//Invalidate(BRect(sequence->getMaxSeq()*XBOX,y_count*XBOX,sequence->getMaxSeq()*XBOX+XBOX,y_count*XBOX+XBOX));
	if(Window()->Lock())
	 {
	
		if(seq>sequence->getMaxSeq()) sequence->setMaxSeq(seq);
		else
			if(seq==sequence->getMaxSeq() && sequence->ItemAt(seq)==-1)
					{
						//k=sequence->getMaxSeq();
						sequence->setMaxSeq(-1);
						for(int f=k-1;f>-1;f--)
						{
							
							if(sequence->ItemAt(f)!=-1) {
								sequence->setMaxSeq(f);
								break;
							}
						}
						
					}
								
					
		// Il MaxPat non e' stato ancora pensato ne implementato..
		// Non e' forse meglio che il calcolo si faccia 
		// SoLo durante il Save??
		// ?
		
		
		
	 	Window()->Unlock();			
	}
	
	Invalidate();
}

void
XMPoz::setPosition(int val)
{
	curPat=val;
	Invalidate();
}

