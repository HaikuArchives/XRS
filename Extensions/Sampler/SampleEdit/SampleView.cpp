
#include "SampleView.h"
#include <stdio.h>
#include <stdlib.h>



SampleView::SampleView(BRect vframe): BView(vframe,"x",0,B_WILL_DRAW|B_FRAME_EVENTS)
{
	SetViewColor(0,0,0);
	SetLowColor(0,0,0);
	SetHighColor(255,255,255);
	SetFontSize(8);
	
	data=NULL;
	rev=false;
	sampleCount=0;
	left_p=right_p=incx=0;
	
		
}
		
void
SampleView::Init(void *dat,int32 count,bool c)
{
	data=dat;
	sampleCount=count;
	rev=c;
	incx=(int)(sampleCount/Bounds().Width());
	Invalidate();
}


void
SampleView::Draw(BRect b)
{
	
	
	float y;
 	
	//MovePenTo(0,zero);
	
	if(data==NULL || sampleCount==0){
		StrokeLine(BPoint(b.left,zero),BPoint(b.right,zero));
		return;
	}
	
		
	SetHighColor(10,200,10);	
	
	for(int i=0;i<Bounds().IntegerWidth();i++)
	{
		if(!rev)
			memcpy(&right,(char*)data+(i*incx)*2,2);
		else
			memcpy(&right,(char*)data+(sampleCount*2-(i*incx)*2),2);
		//memcpy(&left,(char*)data+(i*incx)*2+2,2);
		right=right*amp;
		y=(float)((zero)+(right/incy));
	 	StrokeLine(BPoint(i-1,right_p),BPoint(i,zero));
     		StrokeLine(BPoint(i,zero),BPoint(i,y));
     		right_p=y;
     		
 	}
 }
 void
 SampleView::AttachedToWindow()
 {
 	zero=(Bounds().Height())/2;	
 	incy=-32767./zero;
 }