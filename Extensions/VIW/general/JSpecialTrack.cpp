#include "JSpecialTrack.h"
#include "PlugWindow.h"
#include "VSTInstrument.h"
#include "SpecialTrack.h"
#include "Box.h"
#include "GfxMsg.h"

JSpecialTrack::JSpecialTrack(BRect rect,int16 n,BView* v): JTrack(rect,n,v,JTRACK_VOLUME|JTRACK_NOTE|JTRACK_VST)
{

	instrument=NULL;
}

JSpecialTrack::~JSpecialTrack()
{
	if(((SpecialTrack*)myTrack)->win!=NULL)
	if(((SpecialTrack*)myTrack)->win->Lock())
		((SpecialTrack*)myTrack)->win->Quit();
			
}

void
JSpecialTrack::Reset(Pattern* p ,Track* tr)
{
	JTrack::Reset(p,tr);
	SetName("[mda JX10]");
		
	if(((SpecialTrack*)tr)->viw!=instrument)
	{
						
		((SpecialTrack*)tr)->win=new PlugWindow("[mda JX10]");
		((SpecialTrack*)tr)->win->SetControls((((SpecialTrack*)tr)->viw)->Configure(),true);
		((SpecialTrack*)tr)->win->Show();
		if(((SpecialTrack*)tr)->settings!=NULL) ((SpecialTrack*)tr)->win->LoadPref(((SpecialTrack*)tr)->settings);
		instrument=((SpecialTrack*)tr)->viw;
	}
	
	
}

void
JSpecialTrack::SetName(const char* t)
{
	JTrack::SetName(t);
	if(((SpecialTrack*)myTrack)->win!=NULL) ((SpecialTrack*)myTrack)->win->SetTitle(t);	
}
void
JSpecialTrack::MessageReceived(BMessage *message)
{
	BWindow *win;
	
	switch(message->what)
	{
		case TRACK_SET:
		if(((SpecialTrack*)myTrack)->win!=NULL) win=((SpecialTrack*)myTrack)->win;
			else	return;
			
			win->Activate();
			Window()->PostMessage(message,Parent());
			if(message->FindInt32("mouse") & B_SECONDARY_MOUSE_BUTTON) 
			{
				if(win->Lock()){
				
					if(win->IsHidden()) 
						win->Show();
						else
						win->Hide();
					win->Unlock();
				}
			}
		break;
		default:
		JTrack::MessageReceived(message);
		break;
	}
}