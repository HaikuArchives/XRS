#include "JSamplerTrack.h"
#include "GraphicsDef.h"
#include "Pattern.h"
#include "SamplerTrack.h"
#include "GfxMsg.h"
#include "stdio.h"
#include "SamplerTrackBoost.h"

#define	XRS_BANK_SAMPLE	'xbks'

JSamplerTrack::JSamplerTrack(BRect rect,int16 n,BView* v,SamplerTrackBoost *b):JTrack(rect,n,v,JTRACK_VOLUME|JTRACK_NOTE|JTRACK_MENU|JTRACK_VST)
{
	boost=b;
}


void
JSamplerTrack::Reset(Pattern* p ,Track* tr)
{		
	JTrack::Reset(p ,tr);
	mySamplerTrack=dynamic_cast<SamplerTrack *>(myTrack);

}
void
JSamplerTrack::Select()
{
	JTrack::Select();
	boost->SetTrack(mySamplerTrack);
	
}
void
JSamplerTrack::Deselect()
{
	JTrack::Deselect();
	boost->SetTrack(NULL);
}
void
JSamplerTrack::MessageReceived(BMessage* message)
{	entry_ref	ref;	
	switch(message->what)
	{

	case POPUPMENU:
		{
			BPoint	point(99,0);
			ConvertToScreen(&point);
			boost->ShowMenu(point,this);
		}
	break;
	case TRACK_SET:
			
			Window()->PostMessage(message,Parent());
			if(message->FindInt32("mouse") & B_SECONDARY_MOUSE_BUTTON) 
			{
				boost->ShowPanel();
			}
	break;
	case XRS_BANK_SAMPLE:	
	
		message->FindRef("refs",&ref);
		boost->RefReceived(ref,getTrack(),message);
		Reset(getTrack()->getCurrentPattern(),getTrack());
	
	break;
		
	case XRS_SIMPLE_DATA:
		message->FindRef("refs",&ref);
		boost->RefReceived(ref,getTrack());
		Reset(getTrack()->getCurrentPattern(),getTrack());
	
	break;
	
		
	default:
		
		JTrack::MessageReceived(message);
		
		break;
	}

}