#include "SamplerTrackBoost.h"
#include "Application.h"
#include "SamplerTrack.h"
#include "JSamplerTrack.h"
#include "filemaker_msg.h"
#include "Message.h"
#include "File.h"
#include "GfxMsg.h"
#include "FxWindow.h"
#include "Path.h"
// new session
#include "ExternalManager.h"
#include "BankManager.h"
#include "PopUpMenu.h"
#include "MenuItem.h"
#include "Sample.h"

#include "NodeInfo.h"
#include "TrackManager.h"
#include "Alert.h"
#include "FilePanel.h"

#define	XRS_BANK_SAMPLE	'xbks'

#include	"BitmapMenuItem.h"
//#include	"Xed_Utils.h"

// DINAMIC CAST!!!!!!!! //


SamplerTrackBoost::SamplerTrackBoost():TrackBoost()
{
	name[0]='S';
	name[1]='a';
	name[2]='m';
	name[3]='p';
	name[4]='l';
	name[5]='e';
	name[6]='r';
	name[7]='\0';
	id=0;
	
	popup = new BPopUpMenu("");
	popup->SetLabelFromMarked(true);
	
	fw=new FxWindow(this);
	fw->Show();
	
	extm=new ExternalManager();
	
	banker=NULL;
	tm->GetXRSDirectoryEntry(&sample_ref,"Samples");
	openpanel=NULL;
	
}

SamplerTrackBoost::~SamplerTrackBoost()
{

	if(fw->Lock()) fw->Quit();
	printf("Quit SamplerTrackBoost\n");
}
void
SamplerTrackBoost::Reset()
{

	//	ricreare il menu
	//	altro?
	MakeMenu();
}

void SamplerTrackBoost::RemoveAll()
{
	BAlert *sam;
	int32 sam_say;

	sam=new BAlert("Remove ","Remove All Samples?","Remove","No");
	sam_say=sam->Go();
	
	if(sam_say!=0) return; 
	
	
	BList *l=new BList(0);
	
	tm->getAllMyTrack(l,0);
	
	extm->Empty();
	
	if(banker!=NULL)
			banker->getSampleList()->MakeEmpty();
	if(l!=NULL)
		{
			for(int i=0;i<l->CountItems();i++)
			{
				SamplerTrack* tr=(SamplerTrack*)l->ItemAt(i);
				tr->setType(UNSET_TYPE);
				tr->setName(tr->getPreferredName());
				tr->setSample(NULL);
			}
		}
	MakeMenu();
	//tm->getAllMyJTrack(NULL,0);
	RefreshGraphics();
}
void
SamplerTrackBoost::Restart()
{
	//	cancellare i samples dall memoria.
	//	cancellare il menu
	//	ricreare il menu
	//	altro?
	
	printf("TrackBoost Restart?\n");
	
	extm->Empty();
	if(banker!=NULL)
	{
		banker->getSampleList()->MakeEmpty();
		delete banker;
		banker=NULL;
	}

	if(fw->Lock()){
			if(fw->IsHidden()) 
				fw->Show();
					
				fw->Unlock();
			}
}

void
SamplerTrackBoost::Init(TrackManager* t)
{
	TrackBoost::Init(t);
}


void
SamplerTrackBoost::SetTrack(SamplerTrack* tr)
{
	fw->SetTrack(tr);
}
Track*
SamplerTrackBoost::getTrack()
{
	return dynamic_cast<Track *>(new SamplerTrack());
}
JTrack*
SamplerTrackBoost::getJTrack(BRect r,int16 n,BView* tv)
{
	
	JSamplerTrack *t=new JSamplerTrack(r,n,tv,this);
	t->target=fw;
 	return (t);
 }

bool
SamplerTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(cur);
	
	bool ret=true;
	int32 trenta;
	switch(va){
	
	case TRACK_RESAMPLE_EN:
	if(rt==0)
		tr->setResampleEnable(false);
		else
		tr->setResampleEnable(true);
		
	break;
	case TRACK_AMPBOOST:
		tr->amp=(float)rt;
	break;
	case TRACK_RESAMPLE:
		tr->setResample((int)rt);
	break;
	case TRACK_REVERSED:
		if(rt==0)
			tr->setReversed(false);
					else
			tr->setReversed(true);
	break;
	
	case	SONG_TRACKBOOST:
		//printf("Got a track boooooost..\n");
		//file->Seek(rt,SEEK_CUR);	
		{
			BMessage *newdata=new BMessage();
			BString		path;
			newdata->Unflatten(file);
			int i=0;
			while(newdata->FindString("Sample_path",i,&path)==B_OK)
			{
				printf("Sample %d %s\n",i,path.String());
				i++;
				{// da rendere una funzione!!
				
				entry_ref ref;			
				status_t error=get_ref_for_path(path.String(),&ref);
				if(error==B_OK)
					error=extm->AddSample(ref);
							
					if(error!=B_OK)
							{
								entry_ref hope;
								tm->GetXRSDirectoryEntry(&hope,"Samples");
								BPath sup(path.String()); 
								BPath  *p=new BPath(new  BEntry(&hope) );
								p->Append(sup.Leaf());	
								error=get_ref_for_path(p->Path(),&hope);
								if(error==B_OK)
								{
									error=extm->AddSample(hope);
									if(error!=B_OK) tm->ErrorMsg("Could not find SAMPLE: \n",p->Path());
								}			
								
							}
				}// da rendere una funzione!
			}
			
			float xp,yp;
			
			if( newdata->FindFloat("win_Xpos",&xp)==B_OK && (newdata->FindFloat("win_Ypos",&yp)==B_OK))
				fw->MoveTo(xp,yp);
			bool hidden;
			if( newdata->FindBool("win_hidden",&hidden)==B_OK) if(hidden) ShowPanel();
			
		}
	
	break;
	
	case BANK_NAME:
		{
		char	title[100];
		file->Read(title,rt);
		title[rt]='\0';
		//printf("%ld - Old Bank name : %s  (len %ld)  \n",va,title,rt);
		status_t s;
		banker=new BankManager(title);
		if(banker->InitCheck()!=B_OK) 
			{
				tm->ErrorMsg("Could not find Bsnk: \n",title);
				return true;
			}
		extm->AddSampleList(banker->getSampleList());
		//MakeMenu();
		}
	
	break;
	case EXT_LIST_START:
	{
	
		//printf("%ld - Old Sample  List start\n");
		
		char	path[B_PATH_NAME_LENGTH];
		entry_ref	ref;
		
		
		while(va!=EXT_LIST_STOP)
		{
			int64 rt2;
			va=ReadFixed(&rt2,file);
			
			rt=(int32)rt2;
			switch(va){
		
			case EXT_SAMPLE_PATH:{
			
				file->Read(path,rt);
				path[rt]='\0';
				//printf("EXTERNAL Sample : %s \n",path);
				status_t error=get_ref_for_path(path,&ref);
				if(error==B_OK)
					//printf("EXTERNAL Sample ERROR ! : %s \n",strerror(error));
					
					error=extm->AddSample(ref);
							
					if(error!=B_OK)
						{
						entry_ref hope;
						tm->GetXRSDirectoryEntry(&hope,"Samples");
						BPath sup(path); 
						BPath  *p=new BPath(new  BEntry(&hope) );
						p->Append(sup.Leaf());	
									
						error=get_ref_for_path(p->Path(),&hope);
						if(error==B_OK){
								error=extm->AddSample(hope);
								if(error!=B_OK) tm->ErrorMsg("Could not find SAMPLE: \n",p->Path());
								
								}			
								
					}}
				break;
				default:
						if(va>=2000) file->Seek(rt,SEEK_CUR);
				break;
			}
		}
	}
		
	break;
	
	case TRACK_SAMPLE:
	{			
		char	base[SAMPLE_NAME_L];
		file->Read(base,rt);
		base[rt]='\0';
		
		printf("%ld - Old Track Sample : %s  (len %ld) for %s\n",va,base,rt,tr->getName());
		Sample *s=FindSample(base);
		if(s!=NULL)
		{
			cur->setType(EXT_SAMPLE_TYPE);
			tr->setSample(s);
			tr->setName(tr->getSample()->name);
			locateEXTMSample(tr);
		}
		
		
	}					
		break;				
		
			default:
				//printf("..%ld..",va);
			ret=false;
			break;
		}	
		return ret;
}
void
SamplerTrackBoost::Save(Track *cur,int t,BFile* file,int mea)
{
	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(cur);
	
	if(cur==NULL)
	{
			   		
   		BMessage* ext=new BMessage();
   		
   		for(int j=0;j<getEXTM()->CountItems();j++)
   		{
   			ext->AddString("Sample_path",getEXTM()->getSampleAt(j)->path_name);
   			ext->AddFloat("win_Xpos",fw->Frame().left);
   			ext->AddFloat("win_Ypos",fw->Frame().top);
   			if(fw->Lock()){
   			ext->AddBool("win_hidden",fw->IsHidden());
   			fw->Unlock();
   			}
   		}
   		
   		int32 y;
      		status_t error=ext->Flatten(file,&y);
      		return;
	}	
	
	bool ap;
	
	if(tr->getType()!=UNSET_TYPE)
    	WriteVar(TRACK_SAMPLE,tr->getSample()->name,strlen(tr->getSample()->name),file);
	
	if(tr->isReversed()) ap=1; else ap=0;
    	WriteFixed(TRACK_REVERSED,ap,file);
    	
    	if(tr->isResampleEnable()) ap=1; else ap=0;
    	WriteFixed(TRACK_RESAMPLE_EN,ap,file);
    	
    	WriteFixed(TRACK_RESAMPLE,(int32)tr->getResample(),file);
    	
    	WriteFixed(TRACK_AMPBOOST,(int32)tr->amp,file);
    	
}
bool
SamplerTrackBoost::NeedMessage(){return true;}

void
SamplerTrackBoost::ShowMenu(BPoint point,JSamplerTrack* t)
{
	lastTrack=t;

	locateEXTMSample(dynamic_cast<SamplerTrack *>(t->getTrack()));
	popup->Go(point,true,true,true);
}
ExternalManager*
SamplerTrackBoost::getEXTM()
{
	return extm;
}
status_t
SamplerTrackBoost::RefReceived(entry_ref ref,Track* t,BMessage *m=NULL)
{
	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(t);
	
	status_t r;
	
	if(m!=NULL) 
	{
		if(m->what==XRS_BANK_SAMPLE)
		{
			//printf("il msg e' un banko s%s \n",ref.name);
			
			if(CheckIsBank(&ref)!=B_OK) return B_ERROR;
			
			r=getEXTM()->AddBankSample(m);
			if(r==B_OK){
	
				tr->setSample(getEXTM()->getSampleAt(getEXTM()->CountItems()-1));
				tr->setType(EXT_SAMPLE_TYPE);
				tr->setName(tr->getSample()->name);
				MakeMenu();
				return B_OK;
			}
		else
			return B_ERROR;
		}
	}
	
	// siamo sicuri che e' un B_SAMPLE_DATA???
	// cmq, NON dovrebbe essere necessario..
		
	r=getEXTM()->AddSample(ref);
		if(r==B_OK){
	
				tr->setSample(getEXTM()->getSampleAt(getEXTM()->CountItems()-1));
				tr->setType(EXT_SAMPLE_TYPE);
				tr->setName(tr->getSample()->name);
				//printf("name %s \n",tr->getSample()->name);
				MakeMenu();
				return B_OK;
			}
		else
			return B_ERROR;
	
	
}
void
SamplerTrackBoost::ShowPanel()
{
if(fw->Lock()){
			if(fw->IsHidden()) 
				fw->Show();
					else
					fw->Hide();
				fw->Unlock();
			}
}
void
SamplerTrackBoost::RemoveSample(Sample* s)
{
	if(s==NULL) return;
	
	BAlert *sam;
	int32 sam_say;
	char   name[SAMPLE_NAME_L+100];
	
	sprintf(name,"Remove Sample '%s' \nfrom ALL SamplerTracks that use it?\0",s->name);
	
	sam=new BAlert("Remove ",name,"Remove","No");
	sam_say=sam->Go();
	
	if(sam_say!=0) return; 
	
	
	BList *l=new BList(0);
	
	tm->getAllMyTrack(l,0);
	
	//Sample* fs=NULL;
	BList *sl=extm->getSampleList();
	
	if(sl==NULL) return;
	
	for(int i=0;i<sl->CountItems();i++)
	{
		Sample* fs=(Sample*)sl->ItemAt(i);
		if( fs==s)
			{
				sl->RemoveItem(i);
				delete fs;
				break;
			}
	}
	
	if(l!=NULL)
		{
			for(int i=0;i<l->CountItems();i++)
			{
				SamplerTrack* tr=(SamplerTrack*)l->ItemAt(i);
				Sample* ss=tr->getSample();
				if(ss==s)
				{
					tr->setType(UNSET_TYPE);
					tr->setName(tr->getPreferredName());
					tr->setSample(NULL);
					//Refresh((JTrack*)tr);
				}
			}
		}
	MakeMenu();
	RefreshGraphics();
}
void
SamplerTrackBoost::MakeMenu()
{
	
	BMenuItem	*t;
	BMenuItem * item;
	Sample*        kikko;
	int32 		max;
	BMessage *info;
	

		max=popup->CountItems();
		
		for(int j=0;j<max;j++){
		
		t=popup->ItemAt(0);
		if(t==NULL) break;
		popup->RemoveItem(t);
		delete t;
		}
		
		
				
		for(int zip=0;zip<getEXTM()->CountItems();zip++)
		{
			info=new BMessage(TRACK_SAMP_EXT);
			info->AddInt16("sample",zip);
			kikko=getEXTM()->getSampleAt(zip);
			item=new BMenuItem(kikko->name, info);
			item->SetTarget(fw);
			popup->AddItem(item);
			
		}
		
		/*menu->AddSeparatorItem();
		info=new BMessage(EXT_SAMPLE);
		info->AddInt16("id",h);
		item=new BMenuItem("Load External Sample...", info);
		item->SetTarget(Window());
		menu->AddItem(item);*/
		
		//popup->AddItem(new BitmapMenuItem("","TrackModePianoOff",NULL));
		
}
void
SamplerTrackBoost::ChangeSample(int id)
{
	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(lastTrack->getTrack());
	
	if(tr==NULL || id >= getEXTM()->CountItems()) return;
		
		
	
	tr->setSample(getEXTM()->getSampleAt(id));
	tr->setType(EXT_SAMPLE_TYPE);	
	if(tr->isNameLocked()==false) tr->ResetName();
	lastTrack->SetName(tr->getName());
	
	tm->SelectTrack(lastTrack);
	lastTrack=NULL;
		
}
status_t
SamplerTrackBoost::locateEXTMSample(SamplerTrack* t)
{
	BMenuItem*	item=NULL;
	
	if(t->getType()==UNSET_TYPE) 
			{
				item=popup->FindMarked();
				if(item!=NULL) item->SetMarked(false);
				return B_ERROR;
			}
		
	item=popup->FindItem(t->getSample()->name);
			
	if(item==NULL)  return B_ERROR;	
	
	item->SetMarked(true);
				
	entry_ref ref;			
	tm->GetXRSDirectoryEntry(&ref,"Folder");
	
	return B_OK;
	
}


status_t
SamplerTrackBoost::CheckIsBank(entry_ref *ref)
{
	BNodeInfo	info;
	BNode		node(ref);
	
	char	t[B_MIME_TYPE_LENGTH];
	
	status_t err;
	
	err=node.InitCheck();
	if(err!=B_OK) return err;
	
	err=info.SetTo(&node);
	if(err!=B_OK) return err;
	
	err=info.GetType(t);
	if(err!=B_OK) return err;
	
	if(strncmp(t,"audio/XRS-Bank",14)!=0) return B_ERROR;
	
	return B_OK;
}
Sample*
SamplerTrackBoost::FindSample(const char* sam)
{
	Sample *k=NULL;
	char	base[SAMPLE_NAME_L];
	
	for(int i=0;i<getEXTM()->CountItems();i++)
	{
		
		strcpy(base,(getEXTM()->getSampleAt(i)->name));
		//printf("     FindSample  #%s# \n",sam);
		//printf("                 #%s# \n",base);
		if(strcmp(base,sam)==0) 
			
				 k=getEXTM()->getSampleAt(i);
				
			
		
	}
	
	return k;
}

void
SamplerTrackBoost::LoadSample()
{
	if(openpanel==NULL){
	BMessenger	*me=new BMessenger(fw);
	openpanel=new BFilePanel(B_OPEN_PANEL,me,&sample_ref,0,false,NULL,NULL,true,true);	
	}
	openpanel->Show();
	openpanel->GetPanelDirectory(&sample_ref);
	//return B_OK;
}