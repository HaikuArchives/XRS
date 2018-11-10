#include "XMatrix.h"
#include "math.h"
#include "stdio.h"
#include "Window.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "Message.h"
#include "GfxMsg.h"
#include "XMPoz.h"
#include "maxcount.h"
#define 	XBOX		18


/*

cose da fare:

1) init con la canzone!


*/

char ni[3];


XMatrix::XMatrix(BRect r,BView* n,XMPoz* k):BView(r,"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW)
{
	the_n=n;
	the_s=k;
	y_pos=0;	
	x_pos=0;
	
}

void
XMatrix::ScrollTo(BPoint p)
{
	
	BView::ScrollTo(p);
	
	the_n->ScrollBy(0,p.y-y_pos);
	y_pos=p.y;
	
	the_s->ScrollBy(p.x-x_pos,0);
	x_pos=p.x;
}

void
XMatrix::Reset(Sequence* s)
{
	sequence=s;
	
				
	curPat=0;
	sel=0;
	y_count=s->getMaxPat();
	MaxPat=0;
	
		
	if(Window()->Lock())
	{
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
		the_s->setPosition(curPat);
		if(curPat>sequence->getMaxSeq()) 
		{
			curPat=0;
			the_s->setPosition(curPat);
		}
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
	Invalidate(BRect(0,XBOX*sel,XBOX*X_COUNT,XBOX*(sel+1)));
	sel=y;
	Invalidate(BRect(0,XBOX*sel,XBOX*X_COUNT,XBOX*(sel+1)));

}

void
XMatrix::Draw(BRect r)
{
	if(sequence==NULL) return;
	
	int	ax1=(int)floor(r.left/XBOX)-1;
	int	ay1=(int)floor(r.top/XBOX)-1;
	int	ax2=(int)ceil(r.right/XBOX)+1;
	int	ay2=(int)ceil(r.bottom/XBOX)+1;
	
	if(ax2>X_COUNT) ax2=X_COUNT;
	
	if(ay2>y_count) 
	{
		SetHighColor(0,0,0);
		StrokeLine(BPoint(ax1*XBOX,y_count*XBOX),BPoint(ax2*XBOX+XBOX,y_count*XBOX));//  _
		ay2=y_count;
	}
	
	
	
	for(int y=ay1;y<ay2;y++)
	
	{
		if(y==sel)
		{
			SetHighColor(255,227,153);	
			FillRect(BRect(ax1*XBOX,sel*XBOX,ax2*XBOX+XBOX,sel*XBOX+XBOX-1));
		}
			
		
	 
	 for(int x=ax1;x<ax2;x++)
		
			
			
			{
				
			if(y!=sequence->ItemAt(x)){
			
				
								
				SetHighColor(0,0,0);
				StrokeLine(BPoint(x*XBOX,y*XBOX),BPoint(x*XBOX,y*XBOX+XBOX)); // |
				StrokeLine(BPoint(x*XBOX,y*XBOX),BPoint(x*XBOX+XBOX,y*XBOX));//  _
				}
			else
				{	
					SetHighColor(0,0,0);
					StrokeLine(BPoint(x*XBOX,y*XBOX),BPoint(x*XBOX,y*XBOX+XBOX)); // |
					StrokeLine(BPoint(x*XBOX,y*XBOX),BPoint(x*XBOX+XBOX,y*XBOX));//  _
					SetHighColor(y*30,150-y*20,150);
					FillEllipse(BRect(x*XBOX+1,y*XBOX+1,x*XBOX+XBOX-1,y*XBOX+XBOX-1));
					SetHighColor(tint_color(HighColor(),B_LIGHTEN_2_TINT));
					StrokeArc(BRect(x*XBOX+1,y*XBOX+1,x*XBOX+XBOX-1,y*XBOX+XBOX-1),45,180);
					SetHighColor(tint_color(HighColor(),B_DARKEN_3_TINT));
					StrokeArc(BRect(x*XBOX+1,y*XBOX+1,x*XBOX+XBOX-1,y*XBOX+XBOX-1),-135,180);
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
	
	if(ax1<X_COUNT && ay1<y_count)
	{
		if(sequence->ItemAt(ax1)!=ay1)
		
				sequence->setItemAt(ax1,ay1,true);
				
				else
				{
						sequence->setItemAt(ax1,-1, true);
				}
		
		
		if(ax1 >= sequence->getMaxSeq()) setMaxs(ax1,ay1);
		
		Invalidate(BRect((ax1)*XBOX,0*XBOX,(ax1+1)*XBOX,y_count*XBOX));
	}
}
void
XMatrix::setMaxs(int seq,int pat)
{
	the_s->setMaxs(seq,pat);
	Window()->PostMessage(1010);
}

void
XMatrix::setPosition(int val)
{
	curPat=val;
	the_s->setPosition(val);
}

