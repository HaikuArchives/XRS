
#include "MatrixWindow.h"

#include "stdio.h"
#include "stdlib.h"
#include "strings.h"


#include "XMatrix.h"
#include "XMName.h"
#include "ScrollView.h"
#include "DropView.h"
#include "Message.h"
#include "XMPoz.h"
#include "Alert.h"
#include "Xed_Utils.h"


#include "JMessages.h"
#include "TextControlFloater.h"


#define 	XBOX		18
#define	Y_COUNT	NUM_PATTERN
#include "maxcount.h"



#include "Box.h"

#include "GfxMsg.h"
#include "Button.h"
#include "Sequence.h"
//#include "TextControlFloater.h"

XMPoz *tt;

MatrixWindow::MatrixWindow(BWindow* w) :
	BWindow(BRect(250,30,250+452,30+Y_COUNT*XBOX+XBOX+14),"PlayList ", B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS)
,target(w)
{

	
	BBox *bb;
	  
	AddChild(bb=new BBox( BRect(0,Y_COUNT*XBOX,452,Y_COUNT*XBOX+XBOX+XBOX ),NULL,B_FOLLOW_BOTTOM|B_FOLLOW_LEFT_RIGHT));
	
	
	BButton *bu;

	bb->AddChild(bu=new BButton(BRect(2,2,50,15),"Add","Add",new BMessage(ADD_PATTERN)));
	bb->AddChild(bu=new BButton(BRect(52,2,100,15),"Add","Del",new BMessage(REMOVE_PATTERN)));
	
	
	
	//new TextControlFloater(BRect(100,2,200,15),B_ALIGN_LEFT,be_plain_font,"Untitled",this,NULL,NULL);

	the_n=new XMName(BRect(0,18,100,Y_COUNT*XBOX-1));
	AddChild(the_n);
	
	AddChild(new BBox( BRect(0,0,100,17 ),NULL,B_FOLLOW_NONE));
	AddChild(tt=new XMPoz(BRect(101,0,452-14,17)));
	
	AddChild(new BBox( BRect(452-13,0,452,17),NULL,B_FOLLOW_RIGHT));
	the_m=new XMatrix(BRect(101,18,452-14,Y_COUNT*XBOX-1),the_n,tt);
	
	
	AddChild(scroll=new BScrollView("w2",the_m,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,false,true));
	bb->AddChild(scroll_bar=new BScrollBar(BRect(120,5,400,20),"",the_m,0,X_COUNT,B_HORIZONTAL));
	
	the_m->SetViewBitmap(MakeBitmapSkin("Morbido"));
	
	sel=0;
	
	the_n->Select(sel);
	the_m->Select(sel);
	
	the_msg=new BMessage(CURPAT_MSG);
	the_msg->AddInt16("pos",0);
	
	the_m->MakeFocus(true);	
	
}
void
MatrixWindow::FrameResized(float,float)
{
	scroll_bar->SetRange(0,X_COUNT*XBOX-the_m->Bounds().Width());
	
	//scroll_bar->SetRange(0,99*XBOX/scroll->Frame().Width());
	//scroll_bar->SetSteps(1,1);
	//scroll_bar->SetProportion(XBOX/scroll->Frame().Width());
}
XMatrix*
MatrixWindow::getMatrix()
{
	return the_m;
}

int
MatrixWindow::nextPattern(int d)
{
	return the_m->nextPattern(d);
}
void
MatrixWindow::step()
{
	the_m->step();
}
int
MatrixWindow::getStep()
{
	return the_m->getStep();
}
int
MatrixWindow::getPattern()
{
	return the_m->getPattern();
}
void
MatrixWindow::MessageReceived(BMessage *m)
{
	int y;
	
	if(m->what>1000 && m->what<2000)
	{
		target->PostMessage(GENERIC);	
	}
	switch(m->what)
	{
		case ADD_PATTERN:
		case REMOVE_PATTERN:
			m->AddInt16("measure",sel);
			target->PostMessage(m);
		break;
	
		case  CURPAT_MSG:
			
			 y=m->FindInt16("pos")-1;
			
			if(sel!=y){
			//drop[sel]->Deselect();
			sel=y;
			the_n->Select(sel);
			//drop[sel]->Select();
			the_m->Select(sel);}
			
		break;
		
		case FALSEPAT_MSG:
		
			 y=m->FindInt16("pos")-1;
			 
			if(sel!=y){
			//drop[sel]->Deselect();
			sel=y;
			the_n->Select(sel);
			the_m->Select(sel);
			
						 
			target->PostMessage(m);
			 
			}
		
		break;
		case 'ttty':
		{
		
		BPoint f,t;
		
		m->FindPoint("from", &f);
		m->FindPoint("to", &t);
				
		/*  Disabling KeyBoard for MainWindow */
		if(target->Lock()){
		target->CurrentFocus()->SetEventMask(0);
		
		target->Unlock();
		}
				BMessage *mx=new BMessage(JMSG_NAME_SET);
				mx->AddInt16("pos",m->FindInt16("pos"));
				
					
				new TextControlFloater(BRect(f,t),B_ALIGN_LEFT,be_plain_font,m->FindString("name"),this,mx,new BMessage(JMSG_NAME_NOTSET));
		
		}
		break;
		case JMSG_NAME_SET:
			
			const char* name;
			m->FindString("_value", &name);
			
			//printf("Changed to %s\n",name);
			
			seq->SetMeasureName(name,m->FindInt16("pos"));
			
			if(target->Lock()){
			target->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
			target->Unlock();
			}
		break;
		
		case JMSG_NAME_NOTSET:
			if(target->Lock()){
			target->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
			target->Unlock();
			}
		break;
						
		default:
			BWindow::MessageReceived(m); 
		break;
	}

}
int
MatrixWindow::getSelectedPattern()
{
	return sel;
}
void
MatrixWindow::setSelectedPattern(int y)
{	if(Lock()){
		if(sel!=y){
			//drop[sel]->Deselect();
			sel=y;
			the_n->Select(sel);
			the_m->Select(sel);
			
						 
			//target->PostMessage(m);
			 
			}	
		Unlock();
		   }
	
}
void
MatrixWindow::Reset(Sequence* s)
{
	if(Lock()){
			//drop[sel]->Deselect();
			sel=0;
			//drop[sel]->Select();
		Unlock();
		   }
	
	seq=s;
	the_n->Reset(s);
	the_m->Reset(s);
	tt->Reset(s);
	
	//scroll_bar->SetRange(0,99*XBOX-the_m->Bounds().Width());
}

bool
MatrixWindow::QuitRequested()
{
	Hide();
	return false;
}
void
MatrixWindow::setPosition(int p)
{
	if(Lock())
	{
		the_m->setPosition(p);
		Unlock();  
	}
}

