
// JFileManager v0.0.0.0.0.1


#include "JFileManager.h"


#include "stdio.h"
#include <Entry.h>

#include <CheckBox.h>

#include <Message.h>
#include <Messenger.h>

#include "filemaker_msg.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <FilePanel.h>
#include <Roster.h>
#include <Application.h>
#include <Path.h>

#include "Alert.h"
#include "Button.h"
#include "MediaTrack.h"
#include "MediaFile.h"

#include "Juice.h"

#include "SJFilter.h"


#include  "Song.h"
#include "Pattern.h"
#include "Note.h"

#include "VSTWindow.h"
//#include "BankManager.h"
//#include "ExternalManager.h"

#include "Xed_Utils.h"
#include "TextView.h"

#include "Sequence.h"

#include "TrackManager.h"

#define 	CLEAR_TRS		'cls'
#define	DEL_TRS		'del'
#define 	DEL_TRACK 		'delt'

#define	 FRAMES_NUM	1024

BFilePanel*	filepanel;
BFilePanel*	openpanel;


SJFilter 	*filtro;
BRefFilter	*cippo;
BDirectory 	*dir;

BCheckBox	*chek;



JFileManager::JFileManager()
{
	
	save_type=0;  	// 0 -- normal
				// 1 -- render the wave
	open_type=0;
	filtro=new SJFilter();
	
	GetXRSDirectoryEntry(&sl,"Samples");
}

void
JFileManager::Init(Juice* j,TrackManager* t)
{
	succo=j;
	tm=t;
}

Song*
JFileManager::EmptySong()
{
	Song *i= new Song();
	i->Init();
	return i;
}

void
JFileManager::CloseSong(Song* s,bool bkm)
{
	//if(bkm!=NULL) delete s->getBKM();
	
	tm->Restart();
	succo->CloseVst();
	delete s;
}
JFileManager::~JFileManager()
{
	if(filepanel!=NULL) delete filepanel;
	
	if(openpanel!=NULL) delete openpanel;
	
	if(filtro!=NULL) delete filtro;
}
void
JFileManager::SaveReq(entry_ref ref, const char* file_name,Song* s)
{
	BEntry e(&ref);
	BPath p(&e);
	
	BEntry* zip;
	
	entry_ref rif;
	//bool p_r;
	
	status_t er;
	
	
	p.Append(file_name);
	
	er=get_ref_for_path(p.Path(),&rif);
	
	printf("Do you want to load : %s  status : %s \n",p.Path(),strerror(er));
	
	
	
	if(save_type==1)
		RenderAsWave(rif);
	else
		{
			
			SaveFile(rif,s);
			if(s->getEntry()==NULL) s->setEntry(new BEntry(&rif));
			else {
				zip=s->getEntry();
				zip->SetTo(&rif);
				s->setEntry(zip);
			   }
			s->setModified(false);
		}
		
	if(filepanel!=NULL) delete filepanel;
}

status_t
JFileManager::SaveFile(entry_ref rif,Song* song)
{
	
	status_t error;
	int i,j,k;
	
	file=new BFile();
	
    	char	title[10]="andrew hi";
    	
    	error=file->SetTo(&rif,B_CREATE_FILE|B_READ_WRITE);
    		if(error) return error;
    	error=file->InitCheck();
    		if(error) return error;

    	
    	
    	WriteFixed(FILE_VERSION,2);
    	
    	
    	WriteFixed(SONG_START,0);
    	WriteVar(SONG_TITLE,title,10);
    	WriteVar(SONG_DESC,(void*)song->getDescription(),song->getSizeDescription());
    
    	if(song->popupdesc)
    		WriteFixed(SONG_POPUP_DESC,1);
    		else
    		WriteFixed(SONG_POPUP_DESC,0);
	    	
    	/*if(song->getBKM()!=NULL)
    		WriteVar(BANK_NAME,song->getBKM()->the_ref.name,strlen(song->getBKM()->the_ref.name));    	 
   	   		
   	
   	WriteFixed(EXT_LIST_START,0);
   	for(j=0;j<song->getEXTM()->CountItems();j++)
   	{
   		WriteVar(EXT_SAMPLE_PATH,song->getEXTM()->getSampleAt(j)->path_name,strlen(song->getEXTM()->getSampleAt(j)->path_name));
   	}
   	WriteFixed(EXT_LIST_STOP,0);
   	*/
   	   	
    	Track	*cur;
    	Pattern* pat;
    	int64	ap;
    	
    	WriteFixed(SONG_TIME,song->getTempo());
    	if(song->all)
    		WriteFixed(SONG_PLAY_ALL,song->getTempo());
    	
    	//filter
    	WriteFixed(SONG_CUTOFF,song->getCutoff());
    	WriteFixed(SONG_RESONANCE,song->getResonance());
    	
    	if(song->isFilterOn()) 
    			WriteFixed(SONG_FILTER_ON,1);
    		else
    			WriteFixed(SONG_FILTER_ON,0);
    	
    	
    	//VST
    	VSTWindow *vw=succo->vstw;
    	
    	WriteFixed(SONG_VST_START,0);
    	
    	for(int i=0;i<3;i++)
	{
    		char n[100];
      		if(i<2) WriteFixed(SONG_VST_LINE,i);
      		else  WriteFixed(SONG_VST_GLOBAL,i);
      		
      		for(int j=0;j<vw->CountVst(i);j++)
      		{
      			vw->getVstName(n,i,j);
      			WriteVar(SONG_VST_NAME,n,strlen(n));
      		
      			printf("VST on Line %2d and pos %2d! (%s) \n",i,j,n);
      			BMessage *rt=succo->getVSTData(i,j);
      		
      			if(rt!=NULL) {
      		
      				int32 y;
      			
      				WriteFixed(SONG_VST_DATA,1);
      				error=rt->Flatten(file,&y);
      				//rt->PrintToStream();
      				if(error!=B_OK) printf("..errore nel mettere il msg!\n");
      				else       				
      			 		printf("	WrittenVSTData : %ld (%ld)\n",y,rt->FlattenedSize());
		}
      			}
      		      		
   	}
   	
   	
   	WriteFixed(SONG_VST_STOP,0);
    	
    	
    	int curType;    	
    	
    	int32 n=(int32)song->getNumberNotes();
    	WriteFixed(SONG_NUM_NOTES,n);
    	    	
    	    	
    	   // qui si deve salvare le opzioni per tutti boostTrack
    	   
    	   //attualmente solo per il sampler..
    	 //  WriteFixed(SONG_TRACKBOOST,0);
    	  // tm->Save(NULL,0,file,0);
    	
    	for(int i=0;i<MAX_PLUG;i++)
	{
		if(tm->isBoosterValid(i))
			{
				 WriteFixed(SONG_TRACKBOOST,i);
				 tm->Save(NULL,i,file,0);
			}
	}	
    	for(j=0;j<song->getNumberTrack();j++)
    	{
    	
    		// Track Saving
    		// Depending on Track Type
    		// in this version  ONLY if track != 0 ask to TrackManager..
    		
    		cur=song->getTrackAt(j);
    		curType=cur->getModel();
    		
    		   		    		
    		 WriteFixed(TRACK_START,curType);
    		 
    		 //if(curType==0){
    		 
    		 
    		// 
    		 WriteFixed(TRACK_VOL,cur->getVolume()*100);
    		 WriteFixed(TRACK_PAN,cur->getPan()*100);	    			//SAMPLE_NAME_L
    		 WriteVar(TRACK_NAME,(void*)cur->getName(),strlen(cur->getName()));
    		 if(cur->isNameLocked()) ap=1; else ap=0;
    		 WriteFixed(TRACK_NAME_SET,ap);
	
		 // qui facciamo la distinzione tra i vari tipi di traccia (^?^)
    			//Ask the TrackManager to save that particular TrackType.
     			
     		// prima di spostare questo da qui (nota che ci deve essre Prima Track type di track_sample!)
     		// bisogna finire la migrazione dei BankManager ed externalBankmanager su una BoostTrack.
	
    		if(curType==0){
    		
    		WriteFixed(TRACK_TYPE,(int32)cur->getType());
    		
    		// if(cur->getType()!=UNSET_TYPE)
    		 //WriteVar(TRACK_SAMPLE,cur->getSample()->name,strlen(cur->getSample()->name));
    		}
    		
    		tm->Save(cur,curType,file,song->getNumberMeasure());	
    		
    		 if(cur->isOn()) ap=1; else ap=0; //:)
    		 WriteFixed(TRACK_MUTE,ap);
    		WriteFixed(TRACK_FX1_LINE,cur->getFX1Line());
    		// if(cur->isReversed()) ap=1; else ap=0; //:)
    		// WriteFixed(TRACK_REVERSED,ap);
    		
    		 if(cur->isFX1On()) ap=1; else ap=0; //:-)
    		 WriteFixed(TRACK_FX1,ap);
    		
    	
    		 for(i=0;i<song->getNumberMeasure();i++)
    		 {
    	
    			 WriteFixed(PATTERN_START,0);
    			
    			 pat=cur->getPatternAt(i); 
    			
    			 for(k=0;k<pat->getNumberNotes();k++)
    			 {
    			    	WriteFixed(NOTE_START,0);
    			    	
    			    		if(pat->getNoteAt(k)->getValue())
    			    			WriteFixed(NOTE_ON,1);
    			    		else
    			    			WriteFixed(NOTE_ON,0);
    			    	
    			    		WriteFixed(NOTE_VOLUME,pat->getNoteAt(k)->getVolume()*100);
    			    		//WriteFixed(NOTE_OCT,pat->getNoteAt(k)->getOct());
    			    		//WriteFixed(NOTE_NOTE,pat->getNoteAt(k)->getNote());
    			    		WriteFixed(NOTE_NEWNOTE,pat->getNoteAt(k)->getNote());
    			    	WriteFixed(NOTE_STOP,0);		
    			}		
    			
    			 WriteFixed(PATTERN_STOP,0);
    		  			
    		 }
    		 
    		
      		
      		
      		
      	WriteFixed(TRACK_STOP,curType);
	} 
    
    	WriteFixed(PL_START,1);
    	
    	//printf("MaxPat: %d",song->getSequence()->getMaxPat());
    	
    	    	
    	for(int p=0;p<song->getSequence()->getMaxSeq()+1;p++)
    	{
    		int y=song->getSequence()->ItemAt(p);
    		if(y>=0)
    		{
    			WriteFixed(PL_POS_START,p);
    			WriteFixed(PL_POS_PAT,y);
    			WriteFixed(PL_POS_STOP,0);
    		}
    	}
    	
    	WriteFixed(PL_STOP,1);
    		
    	WriteFixed(SONG_STOP,0);
    	
    	AddMime(file);
    	
    	delete file;
	
	
	
	printf("File Saved!\n");
	
}

status_t
JFileManager::Load()
{
	open_type=0;
	entry_ref sl;
	GetXRSDirectoryEntry(&sl,"Songs");
	openpanel=new BFilePanel(B_OPEN_PANEL,&be_app_messenger,&sl,0,false,NULL,NULL,true,true);	
	//filtro=new SJFilter();
	openpanel->SetRefFilter(filtro);
	openpanel->Show();
	return B_OK;
}

//status_t
//JFileManager::LoadSample(BMessage* msg)
//{
//	open_type=1;
//
//	openpanel=new BFilePanel(B_OPEN_PANEL,&be_app_messenger,&sl,0,false,msg,NULL,true,true);	
//	
////	filtro=new SJFilter();
////	openpanel->SetRefFilter(filtro);
//	
//	openpanel->Show();
//	openpanel->GetPanelDirectory(&sl);
//	
//	
//	return B_OK;
//}

//void
//JFileManager::setRef(entry_ref ref)
//{
//	BPath p(&ref);
//	if(p.GetParent(&p)==B_OK)	get_ref_for_path(p.Path(),&sl);	
//	printf("current : %s\n",sl.name);
//}

status_t
JFileManager::LoadFile(entry_ref rif,Song* song)
{
	
	status_t error;
	char	title[100];
	char	base[SAMPLE_NAME_L];
	//char	path[B_PATH_NAME_LENGTH];
	entry_ref ref;
	bool	have_seq=false;
	
	
	succo->setSong(song);
	//BankManager *new_bank;
	
	//BEntry controllo_esiste(&rif);
	//if(controllo_esiste.Exists()==false) return B_ERROR;
	
	
    	file=new BFile();
    	
    	error=file->SetTo(&rif,B_READ_ONLY);
    		if(error) return error;
    	error=file->InitCheck();
    		if(error) return error;
	
	//error=file->GetSize(&file_size);
    	//	if(error) return error;
	
	
	file->Seek(0,SEEK_SET);

			
	int64 rt,va;


	if(FILE_VERSION!=ReadFixed(&rt)) return B_ERROR;
	//printf("file version:: %lld\n",rt);
	if(SONG_START!=ReadFixed(&rt)) return B_ERROR;
	
	int track_c=0;
	Track*	track_o;
	int pat_c=0;
	int cur_pos=0;
	int note_c=0;
	int vst_num=0;
	int track_type=BANK_SAMPLE_TYPE;
	int track_id=0;
	
	//ssize_t temp;
	
	while(va!=SONG_STOP)
	{
		va=ReadFixed(&rt);
		switch(va){
		
			case 	SONG_NUM_NOTES:
				song->setNumberNotes(rt);
			break;
			case	SONG_POPUP_DESC:
				if(rt==0) song->popupdesc=false; 
					else 	song->popupdesc=true;
			break;
			case SONG_TITLE:
				file->Read(title,rt);
				title[rt]='\0';
				//printf("Song title : %s (len %lld)\n",title,rt);
			break;
			case SONG_DESC:
				{
				char ptr[rt+1];
				//ptr=malloc(rt);
				file->Read(ptr,rt);
				ptr[rt]='\0';
				song->setDescription((const char*)ptr);
				//free(ptr);
				}
			break;
			case BANK_NAME:
				
				//printf("Found a bank! %d\n",rt);
				tm->Load(NULL,0,file,va,rt)	;			
			break;
			case EXT_LIST_START:
				//printf("Found an external list of samples! \n");
				tm->Load(NULL,0,file,va,rt)	;
			break;
			case PL_START:
				have_seq=true;
				while(va!=PL_STOP)
				{
					va=ReadFixed(&rt);
					switch(va){
						case PL_POS_START:
							
							cur_pos=rt;
							while(va!=PL_POS_STOP)
							{
								va=ReadFixed(&rt);
								switch(va){
									case PL_POS_PAT:
									song->getSequence()->setItemAt(cur_pos,rt,true);
									break;
								default:
									if(va>=2000) file->Seek(rt,SEEK_CUR);
								break;
								}
							}
						break;
						default:
							if(va>=2000) file->Seek(rt,SEEK_CUR);
						break;
					}
				}
			break;
			case SONG_VST_START:
			{
				printf("Song is VST compatible!\n");
				
				VSTWindow *vw=succo->vstw;
				
				int line=-1;
				int pos=0;
				bool global=false;
			
				while(va!=SONG_VST_STOP){
				
					va=ReadFixed(&rt);
				
					switch(va){
						
						case SONG_VST_NAME:
							file->Read(title,rt);
							title[rt]='\0';
							printf("VST used : %s\n",title);
							break;
							
						case SONG_VST_DATA:
							{
							//printf("VSTDATA : %d \n",rt);
							BMessage *newdata=new BMessage();
							newdata->Unflatten(file);
							//newdata->PrintToStream();
							newdata->AddString("xrsname",title);
							
													
							if(line<0){
								bool k=vw->AddVst(newdata,vst_num,0);
								vst_num++;
								if(!k)
									ErrorMsg("Could not find Vst-PlugIn:",title);
							}
							else
								{
								bool k=vw->AddVst(newdata,line,pos,global);
								if(!k)
									ErrorMsg("*Could not find Vst-PlugIn:",title);
							}
							}
						break;
						
							case SONG_VST_LINE:
								line=rt;
								pos=0;
								global=false;
							break;
							case SONG_VST_GLOBAL:
								line=rt;
								pos=0;
								global=true;
							break;
						default:
							if(va>=2000) file->Seek(rt,SEEK_CUR);
						break;
					}
				}
				
				//printf("VST Ok!\n");
			}		
			break;
			case SONG_TIME:
				song->setTempo((int)rt);
				//printf("Song Tempo : %d  \n",(int)rt);
				//printf("Song Tempo : %d  \n",song->getTempo());
			break;
			case SONG_PLAY_ALL:
				song->all=true;
				break;
			case SONG_CUTOFF:
				song->setCutoff(rt);
				break;
			case SONG_RESONANCE:
				song->setResonance(rt);
				break;
			case SONG_FILTER_ON:
				if(rt==1) 
					song->setFilterIsOn(true);
				     else
				     	song->setFilterIsOn(false);
				break;	     	
				
			case	SONG_TRACKBOOST:
				tm->Load(NULL,rt,file,va,rt);
			break;
			
			case TRACK_START:
				
				track_o=tm->getTrack(rt);
				track_id=rt;
				track_c=song->MeltTrack(track_o);
				
				while(va!=TRACK_STOP)
				{
					va=ReadFixed(&rt);
					switch(va){
					case TRACK_TYPE:
						track_type=(int)rt;
						//printf("--> type : %d",track_type);
					break;
					
					case TRACK_NAME:	//Note : this is set ONLY if different from a sample_name
						file->Read(base,rt);
						base[rt]='\0';
						song->getTrackAt(track_c)->setName(base);
						//printf("On load .. Track_Name : %s\n",base);
					break;
					
					case TRACK_NAME_SET:
						if(rt)
						song->getTrackAt(track_c)->LockName(true);
						else
						song->getTrackAt(track_c)->LockName(false);
						
						//printf("Track_Name_Set ? : %s\n",rt);
					break;
										
						
					case TRACK_VOL:
						song->getTrackAt(track_c)->setVolume((float)rt/(float)100);
						//printf("Track %d : Volume \n",track_c);
					break;
					case TRACK_PAN:
						song->getTrackAt(track_c)->setPan((float)rt/(float)100);
						//printf("Track %d : Volume \n",track_c);
					break;
					case TRACK_MUTE:
						if(rt==0)
						song->getTrackAt(track_c)->setOn(false);
							else
						song->getTrackAt(track_c)->setOn(true);
						
						//printf("Track %d :  MUTED?\n",track_c);
					break;
					/*case TRACK_REVERSED:
						if(rt==0)
						song->getTrackAt(track_c)->setReversed(false);
							else
						song->getTrackAt(track_c)->setReversed(true);
						
						
					break;*/
					case TRACK_FX1:
						if(rt==0)
						song->getTrackAt(track_c)->setFX1On(false);
							else
						song->getTrackAt(track_c)->setFX1On(true);
						
						
					break;
					case TRACK_FX1_LINE:
						song->getTrackAt(track_c)->setFX1Line(rt);
					break;
					case PATTERN_START:
						//printf("Pattern started! %d\n",pat_c);
						//
						song->AddMeasure();
						
						while(va!=PATTERN_STOP)
						{
							va=ReadFixed(&rt);
							switch(va){
							
							case NOTE_START:
								while(va!=NOTE_STOP)
								{
									va=ReadFixed(&rt);
									switch(va){
									
										case NOTE_ON:
										if(rt==0)
										{
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setValue(false);
										// printf("-");
										}
										else
										{
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setValue(true);
										//printf("*");
										}
										break;
										case NOTE_VOLUME:
										
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setVolume((float)rt/100.);

										break;
										case NOTE_OCT:
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setOct(rt);
										break;
										
										case NOTE_NOTE:
										// song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setNote(rt);
										 //old message :
										 // 1) pulire i primi due bit di oct
										 // 2) ricalcolare in base MIDI
										 {
										 int oc=song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->getOct();
										 
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setOct(oc&8);
										 
										 oc=oc&7;
										 
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setNote(rt+oc*12+60);
										
										}
										break;
										case NOTE_NEWNOTE:
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setNote(rt);
										break;
										default:
										//printf("3 Unknown message! %lld\n",va);
										if(va>=2000) file->Seek(rt,SEEK_CUR);
										break;																			
									}
								}
								note_c++;
							break;
							default:
							//printf("2 Unknown message! %lld\n",va);
							if(va>=2000) file->Seek(rt,SEEK_CUR);
							break;
							}
						}
						note_c=0;
						pat_c++;
					break;
					default:
					//printf("1 Unknown message! %lld\n",va);
					//printf("DEBUG: track id %d\n",track_id);
					bool ing=tm->Load(track_o,track_id,file,va,rt);
					if(va>=2000 && ing==false) file->Seek(rt,SEEK_CUR);
					
					break;
					}
				}
				//track_c++;
				song->setNumberMeasure(pat_c);
				pat_c=0;
			break;
			
			default:
			if(va>=2000) file->Seek(rt,SEEK_CUR);
			break;
	}
	
}		

	if(!have_seq)	{
		//printf("Song without a sequnece!\n");
		song->getSequence()->Reset();
		song->all=true;
		}
	//printf("Song is Ok? %s\n",song->getBKM()->the_ref.name);
	song->setEntry(new BEntry(&rif));
	song->setModified(false);
	printf("Song loaded.\n");	
	delete file;
	
	return B_OK;
	
}

void
JFileManager::Save(Song* s,bool saveas)
{

	entry_ref ref;

	if(s->getEntry()==NULL || saveas==true )
		
		{
			save_type=0;
			entry_ref sl;
			GetXRSDirectoryEntry(&sl,"Songs");
			filepanel=new BFilePanel(B_SAVE_PANEL,NULL,&sl,0,false,NULL,NULL,true);
			filepanel->SetRefFilter(filtro);
			filepanel->Show();
			
	 	}
	 else
	 	{
	 		//printf("Second case!\n");
	 		s->getEntry()->GetRef(&ref);
	 		SaveFile(ref,s);
	 		s->setModified(false);
	 	}
	
}


void
JFileManager::WriteFixed(int64 code ,int64 data)
{
	file->Write(&code,sizeof(int64));
	file->Write(&data,sizeof(int64));
	return;
}

void
JFileManager::WriteVar(int64 code ,void* data,int64 size)
{
	WriteFixed(code,size);
	file->Write(data,size);		
}
int64
JFileManager::ReadFixed(int64 *data)
{
	int64 code;
	file->Read(&code,sizeof(int64));
	file->Read(data,sizeof(int64));
	return code;	
}
int64
JFileManager::ReadVar(void *data)
{
	int64 code;
	int64 size;
	code=ReadFixed(&size);
	
	file->Read(data,size);
	
	return code;	
}
void JFileManager::ExportWave(bool all,int po)
{
	allpat=all;
	
	if(allpat) poz=0; else poz=po;
	
	//succo=suc;
	save_type=1;

	filepanel=new BFilePanel(B_SAVE_PANEL,NULL,NULL,0,false,NULL,NULL,true);
	filepanel->Show();

}


status_t
 JFileManager::RenderAsWave(entry_ref ref)
{
     
     status_t err;
     entry_ref refrec;  
     media_file_format mfi;
     media_format mf,outfm;
      int32 cookie=0;
     char *filetype;
     void *buffer;
 
	BAlert * al;
	BButton *theb;
	al=new BAlert("XRS ","Exporting as Wave ..", "Finish!");
	
	theb=(al->ButtonAt(0));
	theb->Hide();
	
	al->Go(NULL);
	


    BMediaFile *dest;
    BMediaTrack	*rectrack;

	 
	 
    buffer=malloc(FRAMES_NUM*4); /* il buffer da passare a ProcessBuff..*/
		 
  
	 	
	 filetype="wav";
	 	 
	 while((err=get_next_file_format(&cookie,&mfi))==B_OK){
	
	 	if(strcmp(mfi.short_name,filetype)==0)
	 	break;
	 
	 	
	 }
	 
	
	if(err!=B_OK) goto cocco;
	
	
	 dest=new BMediaFile(&ref,&mfi);
	 
	 
	 err=dest->InitCheck();
	 
	
	if(err!=B_OK) goto cocco;
	
		
	media_raw_audio_format *fmt;
	
	fmt=&mf.u.raw_audio;
	mf.type=B_MEDIA_RAW_AUDIO;
	
	fmt->format=media_raw_audio_format::B_AUDIO_SHORT;
	fmt->channel_count=2;
	fmt->frame_rate=44100;
	fmt->byte_order=B_MEDIA_LITTLE_ENDIAN;
	fmt->buffer_size=FRAMES_NUM*4;
	 
	 rectrack=dest->CreateTrack(&mf);
	 	if(err!=B_OK) goto cocco;
	 	 	 
	 dest->AddCopyright("Created with XRS 1.2 by Anzani Andrea");
	 dest->CommitHeader();
	 
	int32 togo;
	
	succo->Reset(allpat,poz);
	succo->process_row(succo);
	
	 printf("EXPORT AS WAVE RESET  %d  %d\n",allpat,poz);
	
	
	while(true)
	{	
	 	succo->ProcessBuffer((void *)succo,buffer,FRAMES_NUM*4);
	 	
	 	 
	 	 if(succo->last_buf_good==0) 
	 	 {
	 		err=rectrack->WriteChunk((char*)buffer,FRAMES_NUM*4);
		 	if(err!=B_OK) goto cocco;
			
		 } 
		 
		 else 
		
		{
			 if(succo->last_buf_good+succo->note_size/4<=FRAMES_NUM)
			 {
				
		  		 err=rectrack->WriteChunk((char*)buffer,succo->last_buf_good*4+succo->note_size);
		  		 if(err!=B_OK) goto cocco;
			 }
		 	
		 	 	else
		 	{
		 		err=rectrack->WriteChunk((char*)buffer,FRAMES_NUM*4);
		 		if(err!=B_OK) goto cocco;
		 		togo=(succo->note_size/4)-(FRAMES_NUM-succo->last_buf_good);
		 		// printf("1-: togo : %ld note_size %ld frame-- &ld\n",togo,succo->note_size/4,FRAMES_NUM-succo->last_buf_good);
		 		while(togo)
		 		{
		 		
		 		succo->ProcessBuffer((void *)succo,buffer,FRAMES_NUM*4);
		 			
		 		 if(togo<=FRAMES_NUM)
		 			{
		 				// printf("11: togo : %ld note_size %ld\n",togo,succo->note_size/4);
		 				err=rectrack->WriteChunk((char*)buffer,togo*4);
		 				if(err!=B_OK) goto cocco;
		 				break;
		 			}
		 		else
		 			{
		 		//	 printf("12: togo : %ld note_size %ld\n",togo,succo->note_size/4);

		 				err=rectrack->WriteChunk((char*)buffer,FRAMES_NUM*4);
		 				if(err!=B_OK) goto cocco;
		 				togo -=FRAMES_NUM;
		 			}
		 		
		 		}
		 	
		 		break;
		 	}
		
		}
		 
		
	}
		
	    rectrack->Flush();
	    dest->ReleaseTrack(rectrack);
	    dest->CloseFile();
	    rectrack=NULL;
	    free(buffer);
	    
	 //   printf("Render finished!\n");
	  
	    
	   if(al->Lock());
	    {
	   	theb->Show();	 
	  	al->Unlock();
	  }
	
	  return B_OK;
	  
	  /* Spaghetti programming! */
cocco:
ErrorMsg("An error occorred while exporting! :",strerror(err));
if(al->Lock()) al->Quit();
return err;
}
void
JFileManager::AddMime(BFile*	 file)
{
	BNodeInfo	info;
	status_t err;
	err=info.SetTo(file);
	err=info.SetType("audio/XRS-File");
}
status_t
JFileManager::AskForClose(Song* s)
{	
	BAlert *sam;
	int32 sam_say;

	if(!s->isModified()) return B_OK;
	
			
	sam=new BAlert("Close","Close current Song (unsaved) ?","Close","No");

	sam_say=sam->Go();
	
	if(sam_say==0) return B_OK;
	else return B_ERROR;
		
}
void
JFileManager::ErrorMsg(const char *msg,const char *txt)
{
	char pippo[strlen(msg)+strlen(txt)+2];
	
	strcpy(pippo,msg);
	strcat(pippo,"\n");
	strcat(pippo,txt); 
	(new BAlert("XRS ", pippo, "Ok!",NULL,NULL,B_WIDTH_AS_USUAL,B_WARNING_ALERT))->Go();	
}

