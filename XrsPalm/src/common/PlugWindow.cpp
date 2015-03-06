#include 	"PlugWindow.h"
#include 	"Box.h"
#include 	"Menu.h"
#include 	"MenuBar.h"
#include	"MenuItem.h"
#include	"ScrollBar.h"

#include "stdio.h"
#define	X_WIN_ZOOM		'xwzo'

PlugWindow::PlugWindow(const char*name=NULL) :
	BWindow(BRect(250,30,280,60),name, B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE),
	big(true)
{}

bool
PlugWindow::QuitRequested()
{
	Hide();
	return false;
}
void
PlugWindow::SetControls(BView* conf,bool scr)
{if(Lock()){
		bar=new BMenuBar(BRect(0,0,30,20),"Bar");
		AddChild(bar);
		
		BBox* box=new BBox(BRect(0,bar->Frame().Height()+1,conf->Bounds().Width(),bar->Frame().Height()+conf->Bounds().Height()),"Boxer",B_FOLLOW_NONE);
		ResizeTo(conf->Bounds().Width(),conf->Bounds().Height()+bar->Frame().Height());
		
		
		
			
			box->AddChild(conf);
			if(scr){
				
				ResizeTo(conf->Bounds().Width()+B_V_SCROLL_BAR_WIDTH+1,300);
				BScrollBar *scroll=new BScrollBar(BRect(conf->Bounds().Width()+1,bar->Frame().Height(),conf->Bounds().Width()+1+B_V_SCROLL_BAR_WIDTH,300),"scoller",conf,0,conf->Bounds().Height()+bar->Frame().Height()-300,B_VERTICAL);	
				AddChild(scroll);
			}
			
			
			AddChild(box);
			Unlock();
			
			bigH=Frame().Height();
			bigW=Frame().Width();
		}
			
}

void
PlugWindow::SetPrograms(const char *name,BMenu* prog)
{
	Lock();
	bar->AddItem(nameMenu=new BMenuItem(name,new BMessage(X_WIN_ZOOM)));
	nameMenu->SetTarget(this);	
	if(prog!=NULL) bar->AddItem(prog);
	Unlock();
}
void
PlugWindow::SetState(bool s,float force=-1)
{
	if(!s)	{
				smallH=bar->Frame().Height();
				
				if(force<0) smallW=nameMenu->Frame().Width();
				else
					smallW=force;
					
				ResizeTo(smallW,smallH);
				big=false;
		}
		else{
				ResizeTo(bigW,bigH);
				big=true;
		}
}
void
PlugWindow::MessageReceived(BMessage* msg)
{
	if(msg->what=='xwzo')
	{
		SetState(!big);
		
	}
	else BWindow::MessageReceived (msg);
	
}
void
PlugWindow::SavePref(BMessage* m)
{
	m->AddFloat("Xpos",Frame().left);
	m->AddFloat("Ypos",Frame().top);
	m->AddFloat("Ysize",nameMenu->Frame().Width());
	m->AddBool("big",GetState());
}
void
PlugWindow::LoadPref(BMessage* msg)
{
		float x;
		if(msg->FindFloat("Xpos",&x)==B_OK)
		{
			if(Lock());{
			MoveTo(x,msg->FindFloat("Ypos"));
			msg->FindFloat("Ysize",&smallW);
			SetState(msg->FindBool("big"),smallW);
			Unlock();
			}
		}
}
