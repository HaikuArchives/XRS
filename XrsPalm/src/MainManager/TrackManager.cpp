#include "TrackManager.h"

#include "Track.h"
#include "JTrack.h"
#include "Xed_Utils.h"
#include "Application.h"
#include "MBWindow.h"
#include "Configurator.h"
// non dovrebbero essere qui, poiche non e' lei che salva!
#include "File.h"
#include "filemaker_msg.h"
#include "Note.h"
#include "Message.h"

#include "Entry.h"
#include "Xed_Utils.h"
#include "Path.h"
#include "TrackBoost.h"
#include "GfxMsg.h"
#include "Menu.h"
#include "MenuItem.h"

#include "Song.h"
#include "TracksPanel.h"

image_id image_ar[MAX_PLUG];

TrackManager::TrackManager()
{
	curSong=NULL;
	curPanel=NULL;
	for(int i=0;i<MAX_PLUG;i++)
	{
		 image_ar[i]=0;
	}
}
TrackManager::~TrackManager()
{
	for(int i=0;i<MAX_PLUG;i++)
	{
		 if(image_ar[i]!=0) unload_add_on(image_ar[i]);
	}
	
}
void
TrackManager::LoadPlugin(const char *name,int i)
{
	entry_ref hope;
	GetXRSDirectoryEntry(&hope,"Extensions");
	BPath sup(name); 
	BPath  *p=new BPath(new  BEntry(&hope) );
	p->Append(sup.Leaf());	
	//get_ref_for_path(p->Path(),&hope);
	
	image_ar[i]= load_add_on (p->Path());
	
	if (image_ar[i] > 0)
	{
		// the file is indeed an addon, but is it a VST plugin?
		//printf ("OK! VST Plugin?... ");
		TrackBoost * tb;
		TrackBoost * (*main_plugin) ();
		if (get_image_symbol (image_ar[i], "main_plugin", B_SYMBOL_TYPE_TEXT, (void**) &main_plugin) == B_OK)
		{	// Chances are now that this is a VST plugin, but is it supported?...
									//printf ("Yes!\n");
		tb = (*main_plugin) ();
		printf ("Found XRS-Extension... %10s (id: %2d)\n",tb->name,tb->id);
		list[tb->id]=tb;
		tb->Init(this);
		
		BMessage *m=new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) return;
		
		if(tb->id<2)
			myMenu->AddItem(new BMenuItem(tb->name,m,65+tb->id));
		else
			myMenu->AddItem(new BMenuItem(tb->name,m,66+tb->id));
		
		
		
		}
	}
}
Track*
TrackManager::getTrack(int p)
{
	if(list[p]==NULL)
	{
		char x[103];
		sprintf(x,"getTrack() with id  %2d\0",t);
		ErrorMsg("Strange error appear!! PLEASE report this bug! (oxygenum@tiscali.it)",x);
		return NULL;
	}	
	return list[p]->getTrack();
}


bool
TrackManager::isBoosterValid(int pos)
{
	if(list[pos]==NULL) return false;
	
	return true;
}
void
TrackManager::Save(Track *cur,int t,BFile* file,int mea)
{
	list[t]->Save(cur,t,file,mea);
}
bool
TrackManager::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	bool ret=false;
	
	if(list[t]==NULL)
	{
		char x[103];
		sprintf(x,"tm->Load with id  %2d\0",t);
		ErrorMsg("Strange error appears!! PLEASE report this bug! (oxygenum@tiscali.it)",x);
			
		return false;
	}	
	ret=list[t]->Load(cur,t,file,va,rt);
	return ret;
}

Track*
TrackManager::SendRef(entry_ref ref, int id,BMessage *m)
{
	if(list[id]==NULL) 	return NULL;
	
	Track* t=list[id]->getTrack();
	
	if(list[id]->RefReceived(ref,t,m)!=B_OK) return NULL;
	
	return t;
}

JTrack*
TrackManager::MakeJTrack(Track* p,BRect rect,int16 n,BView* tv)
{
	return (list[p->getModel()]->getJTrack(rect,n,tv));
}

void
TrackManager::Reset(Song* s)
{
	if(curJTrack!=NULL) curJTrack->Deselect();
	curJTrack=NULL;
	for(int i=0;i<MAX_PLUG;i++)
	{
		if(list[i]!=NULL) list[i]->Reset();
	}
	
	curSong=s;
}

void
TrackManager::Restart()
{
	
	if(curJTrack!=NULL) curJTrack->Deselect();
	curJTrack=NULL;
	
	for(int i=0;i<MAX_PLUG;i++)
	{
		if(list[i]!=NULL) list[i]->Restart();
	}	
	curSong=NULL;
	
}
void
TrackManager::Close()
{
	
	PutFloatKey("mediabrowser_Xpos",mb->Frame().left);
	PutFloatKey("mediabrowser_Ypos",mb->Frame().top);
	
	if(mb->Lock()) mb->Quit();
	
	for(int i=0;i<MAX_PLUG;i++)
	{
		if(list[i]!=NULL)  delete list[i];
	}
}

void
TrackManager::Init()
{
	curJTrack=NULL;
	
	myMenu=new BMenu("Extensions");
	for(int i=0;i<MAX_PLUG;i++) list[i]=NULL;
	
	LoadPlugin("Sampler",0);
	LoadPlugin("Tn306",1);
	LoadPlugin("MidiOut",2);
	LoadPlugin("3Osc",3);
	LoadPlugin("VIW",4);
	LoadPlugin("Analiser",5);
	
	entry_ref s;
	::GetXRSDirectoryEntry(&s,"Samples");
	
	
	mb=new MBWindow();
	mb->AddFolder(s);
	::GetXRSDirectoryEntry(&s,"Banks");
	mb->AddFolder(s);

	mb->MoveTo(FloatKey("mediabrowser_Xpos",250),FloatKey("mediabrowser_Ypos",30));
	mb->Show();
}

status_t
TrackManager::SelectTrack(JTrack* x)
{
	if(curJTrack==x ) return B_ERROR;
	
	//if(x==NULL) curJTrack==NULL;
	
	if(curJTrack!=NULL) curJTrack->Deselect();
	curJTrack=x;
	if(curJTrack!=NULL) 	curJTrack->Select();
	
	
	return B_OK;
}
Track*
TrackManager::getCurrentTrack()
{
	if(curJTrack==NULL) return NULL;
	return curJTrack->getTrack();
}
void
TrackManager::GetXRSDirectoryEntry(entry_ref * ref,const char* folder)
{	
	::GetXRSDirectoryEntry(ref,folder);
}
void
TrackManager::ErrorMsg(const char *msg,const char *txt)
{
	char pippo[strlen(msg)+strlen(txt)+2];
	
	strcpy(pippo,msg);
	strcat(pippo,"\n");
	strcat(pippo,txt); 
	(new BAlert("XRS ", pippo, "Ok!",NULL,NULL,B_WIDTH_AS_USUAL,B_WARNING_ALERT))->Go();	
}
void
TrackManager::getAllMyTrack(BList* l, int id)
{
	if(curSong==NULL) return ;
	
	for(int i=0;i<curSong->getNumberTrack();i++)
	{
		if(curSong->getTrackAt(i)->getModel()==id)
		l->AddItem((void*)curSong->getTrackAt(i));
	}
}
void
TrackManager::getAllMyJTrack(BList* l, int id)
{
	if(curPanel==NULL) return ;
	/*
	for(int i=0;i<curPanel->getNumberJTrack();i++)
	{
		if(curPanel->getJTrackAt(i)->getModel()==id)
		l->AddItem((void*)curPanel->getJTrackAt(i));
	}*/
	curPanel->RefreshGraphics();
}
void TrackManager::RefreshSelected()
{
	if(curPanel==NULL) return ;
	curPanel->RefreshSelected();
	curJTrack->Select();
}
void TrackManager::Refresh(JTrack* t)
{
	if(curPanel==NULL) return ;
	curPanel->Refresh(t);
	curJTrack->Select();
}
