#include "XMatrix.h"
#include "math.h"
#include "stdio.h"
#include "Window.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "Message.h"
#include "GfxMsg.h"
#include "GlobalDef.h"

#define 	XBOX			18
#define	NAMELEN		6
#define	NAMESPACE		XBOX*NAMELEN
//#define	y_count	NUM_PATTERN
#define	X_COUNT	99


/*

cose da fare:

1) init con la canzone!


*/

char ni[3];


XMatrix::XMatrix(BRect r,BView *n):BView(r,"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW)
{
	the_n=n;
	
}

void
XMatrix::Reset(Sequence* s)
{
	sequence=s;
	
				
	curPat=0;
	sel=0;
	//sequence->setMaxSeq(-1);
	MaxPat=0;
	
	y_count=s->getMaxPat();
	
	the_n->Reset(s);
	
	if(Window()->Lock())
	{
		//setMaxs(sequence->getMaxSeq()+1,0);
		Invalidate();
		Window()->Unlock();
	}
}
void
XMatrix::AttachedToWindow()
{
	SetViewColor(200,200,220);
	SetLowColor(200,200,220);
	SetFontSize(XBOX/2);
}

int
XMatrix::nextPattern(int prev)
{
	int r;
	r=sequence->ItemAt(curPat+1);
	
	if(r==-1) r=0;
				
	return r;
}

int
XMatrix::getPattern()
{
	int r;
	r=sequence->ItemAt(curPat);
	
	if(r==-1 || sequence->getMaxSeq()==-1) r=EMPTY_PATTERN;
		
			
	return r;
}


void
XMatrix::step()
{

	if(Window()->Lock())
	{
		
		Invalidate(BRect(curPat*XBOX,y_count*XBOX,curPat*XBOX+XBOX,y_count*XBOX+XBOX));
		curPat++;
		if(curPat>sequence->getMaxSeq()) curPat=0;
		//printf("curPat : %d\n",curPat);
		Invalidate(BRect(curPat*XBOX,y_count*XBOX,curPat*XBOX+XBOX,y_count*XBOX+XBOX));
		Window()->Unlock();
	}
}
int
XMatrix::getStep()
{
	return curPat;	
}
void
XMatrix::Select(int y)
{
	Invalidate(BRect(0,XBOX*sel,Bounds().right-Bounds().left,XBOX*(sel+1)));
	sel=y;
	Invalidate(BRect(0,XBOX*sel,Bounds().right-Bounds().left,XBOX*(sel+1)));

}

void
XMatrix::Draw(BRect r)
{
	if(sequence==NULL) return;
	
	int	ax1=(int)floor(r.left/XBOX)-1;
	int	ay1=(int)floor(r.top/XBOX)-1;
	int	ax2=(int)ceil(r.right/XBOX)+1;
	int	ay2=(int)ceil(r.bottom/XBOX)+1;
	
	
	
	if(ay2>y_count) 
	{
		SetHighColor(0,0,0);
		StrokeLine(BPoint(ax1*XBOX,y_count*XBOX),BPoint(ax2*XBOX+XBOX,y_count*XBOX));//  _
		 for(int x=ax1;x<ax2;x++)
			{
				if(x-NAMELEN==sequence->getMaxSeq() && sequence->getMaxSeq()>=0)
					{
						SetHighColor(0,0,255);
						FillRect(BRect(x*XBOX,y_count*XBOX+14,x*XBOX+XBOX,y_count*XBOX+17));
						SetHighColor(0,0,0);	
					}
					
				if(x-NAMELEN==curPat)
					{
						SetHighColor(255,0,0);
						FillRect(BRect(x*XBOX,y_count*XBOX+10,x*XBOX+XBOX,y_count*XBOX+13));
						SetHighColor(0,0,0);	
					}
				
				
				sprintf(ni,"%2d",x+1-NAMELEN);
				DrawString(ni,BPoint(x*XBOX+(XBOX/3),y_count*XBOX+(XBOX/2)));
				
			}
		ay2=y_count;
	}
	
	int axS;
	
	for(int y=ay1;y<ay2;y++)
	
	{
		if(y==sel)
		{
			SetHighColor(255,227,153);	
			FillRect(BRect(ax1*XBOX,sel*XBOX,ax2*XBOX+XBOX,sel*XBOX+XBOX-1));
			//SetHighColor(255,227,153);	

		}
			
		if(ax1<NAMELEN)
		{
			
			
			SetHighColor(0,0,0);	
			DrawString("Measure  ",BPoint(5,y*XBOX+10));
			sprintf(ni,"%2d",y+1);
			DrawString(ni);
			axS=NAMELEN;
		}
		
		else axS=ax1;
	 
	 for(int x=axS;x<ax2;x++)
		
			
			
			{
				
			if(y!=sequence->ItemAt(x-NAMELEN)){
			
				
								
				SetHighColor(0,0,0);
				StrokeLine(BPoint(x*XBOX,y*XBOX),BPoint(x*XBOX,y*XBOX+XBOX)); // |
				StrokeLine(BPoint(x*XBOX,y*XBOX),BPoint(x*XBOX+XBOX,y*XBOX));//  _
				}
			else
				{
					SetHighColor(y*30,150-y*20,150);
					FillRect(BRect(x*XBOX,y*XBOX,x*XBOX+XBOX,y*XBOX+XBOX));
				}
			}
	}
 
}
void
XMatrix::MouseDown(BPoint p)
{
	if(sequence==NULL) return;
	
	int	ax1=(int)floor(p.x/XBOX);
	int	ay1=(int)floor(p.y/XBOX);
	
	if(ay1>y_count-1) return;
	if(ax1<NAMELEN)
		{
			//ax1=NAMELEN;
			Select(ay1);
			return;
		}
		
	if(ax1<X_COUNT && ay1<y_count)
	{
		if(sequence->ItemAt(ax1-NAMELEN)!=ay1)
		
				sequence->setItemAt(ax1-NAMELEN,ay1);
				
				else
				{
						sequence->setItemAt(ax1-NAMELEN,-1);
				}
		
		printf("Matrix click %d %d\n",ax1-NAMELEN, sequence->getMaxSeq());
		
		if(ax1 >= sequence->getMaxSeq()) setMaxs(ax1-NAMELEN,ay1);
		
		Invalidate(BRect((ax1)*XBOX,0*XBOX,(ax1+1)*XBOX,y_count*XBOX));
	}
}
void
XMatrix::setMaxs(int seq,int pat)
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
	
	//printf("k : %d - MaxSeq : %d - Seq %d \n",k,sequence->getMaxSeq(),seq);

	Invalidate(BRect((k)*XBOX,y_count*XBOX,(k)*XBOX+XBOX,y_count*XBOX+XBOX));
	Invalidate(BRect(sequence->getMaxSeq()*XBOX,y_count*XBOX,sequence->getMaxSeq()*XBOX+XBOX,y_count*XBOX+XBOX));
	
	Window()->PostMessage(1010);
}

void
XMatrix::setPosition(int val)
{
	curPat=val;
	Invalidate();
}

