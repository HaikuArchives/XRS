/***************************************
/
/	SimpleJuice beta 02
/
/   Something like a DrumMachine.
/	Original idea by Andrea Anzani
/	
/	I hope to find someone that can help me in get
/	this app better!
/	Please write two lines to me.
/
/	andrea.anzani@lib.disco.unimib.it
/
*****************************************/

#include "SampleView.h"
#include <stdio.h>
#include <stdlib.h>

//#include "Message.h"
//#include "Window.h"

SampleView::SampleView(BRect vframe): BView(vframe,"x",0,B_WILL_DRAW|B_FRAME_EVENTS)
{
	SetViewColor(0,0,0);
	SetLowColor(0,0,0);
	SetHighColor(255,255,255);
	SetFontSize(8);
	
	data[0]=NULL;
	data[1]=NULL;
	clip=false;
	sampleCount=1024;
	//SetLowColor(255,255,255);
	SetFontSize(30);
		
}
//void
//SampleView::Pulse(){
//		BMessage* kill=new BMessage('mod');
//		kill->AddInt16("pos",2);
//			printf("Puzzza %d\n",rand());	
//			Window()->PostMessage(kill);
//			}
			
void
SampleView::Init(float** buffer,int32 samples_num,int s)
{
	data[0]=buffer[0];
	
	sampleCount=samples_num;
	spiaz=s;
	//clip=c;
	
	Invalidate();
}

void
SampleView::Draw(BRect b)
{
	char t[100];
	if(data[0]!=NULL)
	sprintf(t,"%f",data[0][spiaz]);
	DrawString(t,BPoint(30,30));
}
/*
void
SampleView::Draw(BRect b)
{
	int32 i; 
	int64 incy;
	
	
	int incx=0;
	float zero=(Bounds().Height())/2;
 	int32 rtn;
 	
 	float y,y2;
 	
 	//incy=(float)32767/zero;
 	
 	//StrokeLine(BPoint(40,0),BPoint(40,Bounds().bottom));
	//StrokeLine(BPoint(0,zero),BPoint(80,zero));

	MovePenTo(0,zero);
	
	//sampleCount=1;
	//sampleCount=x_synth->GetAudio(left,right,sampleCount);
	if(data==NULL)
	{
		StrokeLine(BPoint(0,zero),BPoint(Bounds().Width(),zero));
	}
	
	else
	
	{
	
	incx=(int)(sampleCount/Bounds().Width());
		
	if(sampleCount>0)
	{
		
		for(i=0;i<Bounds().IntegerWidth();i++)
		{
		
		
		 y=zero-(float)((float)zero/((float)data[0][spiaz+i]));
	 	//y2=(float)((zero)+(left[i*incx]/incy));
	 	
	 	//printf("float %f\n",y);
	       
     		SetHighColor(10,200,10);
     		StrokeLine(BPoint(i,zero),BPoint(i,y)); 
     		
     		
	 	//SetHighColor(200,10,10);
	 	//StrokeLine(BPoint(i+41,zero),BPoint(i+41,y2));
		}
	
	}
	}

}*/