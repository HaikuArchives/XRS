#include "TracksPanel.h"
#include "TranslationUtils.h"
#include "InterfaceKit.h"
#include"GraphicsDef.h"
#include "Xed_Utils.h"
#include "GfxMsg.h"
#include "Entry.h"
#include "Track.h"
#include "Song.h"
#include "stdio.h"
#include "TrackManager.h"
#include "JTrack.h"
#include "JMessages.h"
#include "TrackManager.h"


#include "ColorsDef.h"

#define	XRS_BANK_SAMPLE	'xbks'

TracksPanel::TracksPanel(BRect rect,BView* tv): BView(rect,"_xthis",B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP)
,w(tv)
{
	xnv=new BList(1);
	SetViewColor(bkColor);
}


status_t
TracksPanel::Init(TrackManager* t)
{
	tm=t;
	tm->curPanel=this;
	 return B_OK;
}
JTrack*
TracksPanel::getJTrackAt(int h)
{
	return (JTrack*)xnv->ItemAt(h);
}
int
TracksPanel::getNumberJTrack()
{
	return xnv->CountItems();
}




Track*
TracksPanel::getCurrentTrack()
{
	return tm->getCurrentTrack() ; //selTrack;
	
}


void
TracksPanel::RemoveTrack(int h)
{
	
	JTrack *t=(JTrack*)xnv->ItemAt(h);
	
	if(Window()->Lock()){
	
		xnv->RemoveItem(h);
		RemoveChild(t);
		delete t;
		Window()->Unlock();
	}
}
void
TracksPanel::AddTrack(int h)
{
	JTrack* t;
		
	if(Window()->Lock()){
	
	
		xnv->AddItem(t=tm->MakeJTrack(curSong->getTrackAt(h), BRect(BUTTONS_X_START,BUTTONS_Y_START+h*30-40,WINDOW_XL-18,BUTTONS_Y_START+JTRACK_LY+h*30-40),h,w),h);
		t->RControl();
		AddChild(t);
		
		BMessage *msg=new BMessage(TRACK_SET);
		msg->AddInt16("id",h);
		t->Init(this,msg);
		t->Reset(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h));
		
		
		Window()->Unlock();
	}
	
	
	SelectTrack(h);
}
void
TracksPanel::Reset(Song* s,int p)
{
	curSong=s;
	selPattern=p;
	//selTrack=-1;
	JTrack* 	jtr;
	Track*	tr;
	
	
	
	
	//if(curJTrack!=NULL) curJTrack->Deselect();
	//curJTrack=NULL;
	
	tm->Reset(s);
	
	int max;
	
	if(xnv->CountItems()<curSong->getNumberTrack()) 
	
				max=curSong->getNumberTrack();
	
			else 
				max=xnv->CountItems();
				
	//printf("Loopz! %d\n",max);
	for(int h=0;h<max;h++)
	{
		jtr=(JTrack*)(xnv->ItemAt(h));
		tr=curSong->getTrackAt(h);
		if(tr!=NULL)
			{
				if(jtr!=NULL && tr->getModel()!=jtr->getModel())
				{
					
					RemoveTrack(h);
					AddTrack(h);
					jtr=(JTrack*)xnv->ItemAt(h);

				}
				else
				if(jtr==NULL)
				{
					//printf("No graphics but song\n");
					AddTrack(h);
					jtr=(JTrack*)xnv->ItemAt(h);
				}
				
				//printf("callling Rezet..\n");
				jtr->Reset(tr->getPatternAt(selPattern),tr);
				//SingleBank(h);
				
			}
	
		for(int h=0;h<xnv->CountItems()-curSong->getNumberTrack();h++)	
		{
				//printf("graphics remove\n");
				RemoveTrack(xnv->CountItems()-1);
		}
	}
	
	
	for(int h=0;h<xnv->CountItems();h++)
	{
		jtr=(JTrack*)(xnv->ItemAt(h));
		jtr->Reset(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h));
	}
	
	if(curSong->getNumberTrack()>0) SelectTrack(0);
}
void
TracksPanel::MessageReceived(BMessage* message)
{
	int id;
	entry_ref ref;
	
	if(message->what>1000 && message->what<2000)
	{
		curSong->setModified(true);
		Window()->PostMessage(new BMessage(GENERIC));
	}
		
	switch(message->what)
	{

	case TRACK_SET:
	
		id=message->FindInt16("id");
		SelectTrack(id);
	
	break;
	
		
	/*case TRACK_SAMP:
		id=message->FindInt16("id");
		samp_id=message->FindInt16("sample");
		SelectTrack(id);
		curSong->getTrackAt(id)->setSample(curSong->getBKM()->getSampleAt(samp_id));
		getJTrackAt(id)->SetName(curSong->getBKM()->getSampleAt(samp_id)->name);
		
	break;*/
	case TRACK_SAMP_EXT:
		/*
		id=message->FindInt16("id");
		samp_id=message->FindInt16("sample");
		SelectTrack(id);
		curSong->getTrackAt(id)->setSample(curSong->getEXTM()->getSampleAt(samp_id));
		getJTrackAt(id)->SetName(curSong->getEXTM()->getSampleAt(samp_id)->name);
		*/
	break;
	
	case XRS_BANK_SAMPLE:
		
	case B_SIMPLE_DATA:
		
		//printf("Sample droppd\n");
				
		int i;
			i=0;
		while( message->FindRef("refs",i, &ref) == B_OK )
   		{
   					
   			Track*	newTrack=tm->SendRef(ref,0,message);  //Create a new track from a user dropped file.
   			
   			if (newTrack!=NULL){
				// aggiungiamo una traccia
				curSong->AddTrack(newTrack);
				AddTrack(curSong->getNumberTrack()-1); 
				SelectTrack(curSong->getNumberTrack()-1);
				}
   			   	
		
   			i++;		
   		}
   		
   		   		
   	break;
   			
	break;
		
	default:
		
		BView::MessageReceived(message);
	break;
	}
} 


void
TracksPanel::muteTrack(Track*)
{

	//ATTENZIONE! ->Mute() si comporta non come semra..	
	
	//getJTrackAt(h)->Mute(false,false);
}
void
TracksPanel::soloTrack(Track*)
{
	//ATTENZIONE! ->Mute() si comporta non come semra..	
	/*for(int u=0;u<curSong->getNumberTrack();u++)
	{
		if(u!=y)	getJTrackAt(u)->Mute(false,true);
			else    getJTrackAt(u)->Mute(true,true);
	}*/
}
void
TracksPanel::setPattern(int p)
{
	selPattern=p;
	
	for(int h=0;h<curSong->getNumberTrack();h++)		
		getJTrackAt(h)->Refresh(curSong->getTrackAt(h)->getPatternAt(p));
}

void
TracksPanel::MouseDown(BPoint)
{
	if(!Window()->IsActive()) Window()->Activate(true);
}

void
TracksPanel::RemoveTrackAt(int id)
{
	JTrack*	tr;
	
	//printf("Rermoving the track..\n");
	
	//int id=curSong->getIndexOfTrack(tx);	
	
	//printf("Rermoving the track..%d\n",id);
	if(id<0) return;
	
	SelectTrack(-1);		
	
	RemoveTrack(id);
				
	for(int h=id;h<xnv->CountItems();h++){
	
		tr=getJTrackAt(h);
		
		tr->MoveTo(BUTTONS_X_START, (BUTTONS_Y_START+h*30-40));
		
		BMessage *msg=new BMessage(TRACK_SET);
		msg->AddInt16("id",h);
		tr->Init(this,msg);
		tr->Reset(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h));
			
		}
	
	if(curSong->getNumberTrack()>0)
	{
		SelectTrack(0);
	}

}

void
TracksPanel::SelectTrack(int id)
{	if(id<0)
		tm->SelectTrack(NULL);
	else
		tm->SelectTrack(getJTrackAt(id));
}
void
TracksPanel::RefreshGraphics()
{
	if(Window()->Lock()){
	Reset(curSong,selPattern);
	Window()->Unlock();
	}
}	
void
TracksPanel::RefreshSelected()
{
	if(Window()->Lock()){
	tm->getCurrentJTrack()->Reset(tm->getCurrentJTrack()->getTrack()->getPatternAt(selPattern),tm->getCurrentJTrack()->getTrack());
	Window()->Unlock();
	}
}	
void
TracksPanel::Refresh(JTrack* t)
{
	if(t==NULL) return;
	if(Window()->Lock()){
	t->Reset(t->getTrack()->getPatternAt(selPattern),t->getTrack());
	Window()->Unlock();
	}
}	