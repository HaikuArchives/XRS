
#include "TheApp.h"
#include "FilePanel.h"
#include "GfxMsg.h"

#include "JFileManager.h"
//#include "ExternalManager.h"
#include "Configurator.h"

#include "Alert.h"
#include "Font.h"
#include "TextView.h"

#include "Pattern.h"
#include "Track.h"
#include "Note.h"

#include "Xed_Utils.h"
#include "AboutBox.h"
#include "TrackManager.h"


TheApp::TheApp(const char * signature) :
	BApplication(signature)
{
	AboutBox *ab=new AboutBox(false);
	ab->Show();
	
	cf_Init("XRSConfig");	
	
	
	entry_ref ref;
	
	//clipboard
	
	clipboard=new Pattern(NUM_NOTES);
	
	dinamic_clip=new BList(1);
	
	//clipboard
	
	ab->setText("loading..Extensions");
	tm=new TrackManager();
	tm->Init();
	//the_trackmanager=new TrackManager();
	
	jfm=new JFileManager();
	
	ab->setText("loading..vst");
	S=new MainWindow();
	S->Init(jfm,tm);
	
	ab->setText("..default song..");
	
	status_t err=::GetDefaultSongRef(&ref);
	
	
	currentSong=NULL;
	
	
	LoadRequest(ref);
	
	
	if(err==B_OK ) {
	
		delete currentSong->getEntry();
		currentSong->setEntry(NULL);
		
	
	}
		
		
	if(currentSong==NULL)
		{
		  currentSong=jfm->EmptySong();
		 
		}
	
	ab->setText("..ready..");
	 S->Reset(currentSong);
	look=S->PreferredHandler() ;//getHandler();
	S->Show();
	
	if(ab->Lock()) ab->Quit();	
	
	if(currentSong->getSizeDescription()>0 && currentSong->popupdesc==true)
			(new BAlert("XRS ",currentSong->getDescription(), "Ok",NULL,NULL,B_WIDTH_AS_USUAL,B_EMPTY_ALERT))->Go(NULL);
	
				
}
void
TheApp::EmptySong()
{
	status_t	ask;
	ask=jfm->AskForClose(currentSong);
		
	if(ask==B_OK){
		
	 jfm->CloseSong(currentSong,true);
	currentSong=jfm->EmptySong();
	//currentSong->setDefaultBKM();
	S->Reset(currentSong);
	}
}
void
TheApp::DefaultSong()
{
	entry_ref ref;
	status_t	ask;
	
	ask=jfm->AskForClose(currentSong);
		
	if(ask==B_OK){
		
	 jfm->CloseSong(currentSong,true);
	status_t err=::GetDefaultSongRef(&ref);
	
	
	currentSong=NULL;
	LoadRequest(ref);
	
	
	if(err==B_OK ) {
	
		delete currentSong->getEntry();
		currentSong->setEntry(NULL);
		
	
	}
		
		
	if(currentSong==NULL)	  currentSong=jfm->EmptySong();
	
	 S->Reset(currentSong);
	
	}
}
void
TheApp::MessageReceived(BMessage* message)
{
	entry_ref ref;
	
	status_t	ask;
	//int id;
	//int32 old;
	
	switch(message->what)
	{
	case MENU_NEW_EMPTY:
	
		EmptySong();
		
		break;
		
	case MENU_NEW_DEFAULT:
	
		DefaultSong();
		
		break;
		
	case MENU_SAVE:
		
		//get_ref_for_path("/boot/glaxxa",&ref);
		jfm->Save(currentSong,false);
	
		break;
	case MENU_SAVEAS:
		
		
		jfm->Save(currentSong,true);
	
		break;	
	case MENU_OPEN:
		
		
		//get_ref_for_path("/boot/glaxxa",&ref);
		
		ask=jfm->AskForClose(currentSong);
		
		if(ask==B_OK){
		
			jfm->Load();
		}
		break;
	case MENU_EXPORT:
		S->PostMessage(message);
		break;
		
	case B_SAVE_REQUESTED:
		if(message->FindRef("directory",&ref)==B_OK)
		{
			jfm->SaveReq(ref,message->FindString("name"),currentSong);
			// cosan brutta! :
			if(jfm->save_type==0) S->Saved();
		}
	break;
	
	/*case EXT_SAMPLE:
		id=message->FindInt16("id");
		printf("LoadSample for track %d\n",id);
		if(message->FindRef("refs",&ref)==B_OK)
		{
		ask=currentSong->getEXTM()->AddSample(ref);
		
		// MANCA CONTROLLO FILE GIUSTO 
		
		if (ask==B_OK){
		currentSong->getTrackAt(id)->setType(EXT_SAMPLE_TYPE);
		currentSong->locateEXTMSample(id,ref.name);
		S->RefreshPopUp(-1);
		message->what=TRACK_SET;
		S->PostMessage(message);
		jfm->setRef(ref);
		}
		}
	break;
	
	case B_CANCEL:
		if(message->FindInt32("old_what",0,&old)==B_OK && old==EXT_SAMPLE)
		S->RefreshPopUp(-1);
	break;
	*/
	case MENU_COPY:
		if(S->Lock())
		{	
		
			clipboard->setNumberNotes(currentSong->getNumberNotes());
			CopyPattern((S->getCurrentTrack())->getPatternAt(S->getCurrentPattern()),clipboard);
			S->Unlock();
		}
		S->PostMessage(message);
	break;
	case MENU_CUT:
		
		if(S->Lock())
		{
			clipboard->setNumberNotes(currentSong->getNumberNotes());
			CopyPattern((S->getCurrentTrack())->getPatternAt(S->getCurrentPattern()),clipboard);
			ClearPattern((S->getCurrentTrack())->getPatternAt(S->getCurrentPattern()));
			S->Unlock();
		}
		S->PostMessage(message);
	break;
	case MENU_PASTE:
		
		if(S->Lock())
		{
			clipboard->setNumberNotes(currentSong->getNumberNotes());
			CopyPattern(clipboard,(S->getCurrentTrack())->getPatternAt(S->getCurrentPattern()));
			S->Unlock();
		}
		S->PostMessage(message);
	break;
	case MENU_MEA_COPY:
		
		if(S->Lock())
		{
			for(int i=0;i<dinamic_clip->CountItems();i++)
			{
				delete dinamic_clip->ItemAt(i);	
			}
			
			dinamic_clip->MakeEmpty();
						
			for(int i=0;i<currentSong->getNumberTrack();i++)
			{
				Pattern *r=new Pattern(currentSong->getNumberNotes());
				r->setNumberNotes(currentSong->getNumberNotes());
				dinamic_clip->AddItem(r);
				CopyPattern(currentSong->getTrackAt(i)->getPatternAt(S->getCurrentPattern()),r);
											
			}
						
			S->Unlock();
		}
		S->PostMessage(message);
	break;
	case MENU_MEA_CUT:
		
		if(S->Lock())
		{
			for(int i=0;i<dinamic_clip->CountItems();i++)
			{
				delete dinamic_clip->ItemAt(i);	
			}
			
			dinamic_clip->MakeEmpty();
						
			for(int i=0;i<currentSong->getNumberTrack();i++)
			{
				Pattern *r=new Pattern(currentSong->getNumberNotes());
				r->setNumberNotes(currentSong->getNumberNotes());
				dinamic_clip->AddItem(r);
				CopyPattern(currentSong->getTrackAt(i)->getPatternAt(S->getCurrentPattern()),r);
											
			}
			for(int i=0;i<currentSong->getNumberTrack();i++)
			{
				ClearPattern(currentSong->getTrackAt(i)->getPatternAt(S->getCurrentPattern()));
			}
			S->Unlock();
			
		}
		S->PostMessage(message);
	break;
	case MENU_MEA_PASTE:
		
		if(dinamic_clip->CountItems() > 0) 
		{
			 if(S->Lock())
	 		{
	 			
				for(int i=0;i<dinamic_clip->CountItems();i++)
				{
					if(i<currentSong->getNumberTrack())
					{
						Pattern *r=(Pattern*)dinamic_clip->ItemAt(i);
						r->setNumberNotes(currentSong->getNumberNotes());
						CopyPattern(r,currentSong->getTrackAt(i)->getPatternAt(S->getCurrentPattern()));
					}
				}
				S->Unlock();
			}
			S->PostMessage(message);
		}
	break;
	case SONG_RESET:
	S->Reset(currentSong,false);
	break;
	case ADD_TN306:
	case ADD_TRACK:
	case REMOVE_TRACK:
	S->PostMessage(message);
	break;
	default:
		
		BApplication::MessageReceived(message);
		break;
	}
}
void
TheApp::CopyPattern(Pattern* from,Pattern* to)
{
	for(int i=0;i<from->getNumberNotes();i++)
	{
		to->getNoteAt(i)->setValue(from->getNoteAt(i)->getValue());
		to->getNoteAt(i)->setNote(from->getNoteAt(i)->getNote());
		to->getNoteAt(i)->setOct(from->getNoteAt(i)->getOct());
		to->getNoteAt(i)->setVolume(from->getNoteAt(i)->getVolume());	
	}
}
void
TheApp::ClearPattern(Pattern* to)
{
	for(int i=0;i<to->getNumberNotes();i++)
	{
		to->getNoteAt(i)->setValue(false);	
		to->getNoteAt(i)->setNote(0);	
		to->getNoteAt(i)->setOct(0);	
		to->getNoteAt(i)->setVolume(0.8);	
	}
}
bool
TheApp::QuitRequested()
{
	jfm->CloseSong(currentSong,true);
	
	//delete S;		//da definire meglio..
	//delete jfm;		//da definire meglio..
	
	tm->Close();
	delete tm;
	
	
	cf_Close(); 		//Configuration/Preferences		
	printf("Quit\n");
	snooze(4000);		// fam fiadà!
	
	return BApplication::QuitRequested();
}
void
TheApp::RefsReceived(BMessage *message)
{
	entry_ref ref;
	
	if(message->FindRef("refs",&ref)==B_OK)
		{			
			LoadRequest(ref);
			
			if(currentSong!=NULL) 
			{
				if(currentSong->getSizeDescription()>0 && currentSong->popupdesc==true)
				(new BAlert("XRS ",currentSong->getDescription(), "Ok",NULL,NULL,B_WIDTH_AS_USUAL,B_EMPTY_ALERT))->Go(NULL);
				S->Reset(currentSong);
			}
			
		}
	
}

status_t
TheApp::LoadRequest(entry_ref ref)
{
	Song* new_song;
	new_song=jfm->EmptySong();
	
	jfm->CloseSong(currentSong,false);
	 
		 
	jfm->LoadFile(ref,new_song);
	currentSong=new_song;
	
	return B_OK;
}
void
TheApp::AboutRequested()
{
	
	AboutBox *ab=new AboutBox(true);
	ab->Show();
		
	printf("Written by Anzani Andrea\nSend comments to : oxygenum@tiscali.it\nFreeware\n");
}