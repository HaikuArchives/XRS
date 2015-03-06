#include "MyScroll.h"
#include "MyList.h"
#include "stdio.h"
#define DRAG_X 1978

MyScroll::MyScroll(MyList* j):BScrollView("_scroolView",j,B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER)
{
	m=j;
	j->Init();
	m->y=-1;
}


void
MyScroll::MouseMoved(BPoint p,uint32 mode,const BMessage *msg)
{
	
	
	if(msg!=NULL)
	{

		printf("w\n");	
		
		if(mode==B_EXITED_VIEW)	m->yn=-1;
		else
			m-> yn=(float)((int)(p.y/m->h))*(float)m->h;
			
		printf("%f %f \n",m->y,m->h);	
			
		if(m->yn!=m->y){ m->y=m->yn;  Invalidate(); }
	}
		
		
}