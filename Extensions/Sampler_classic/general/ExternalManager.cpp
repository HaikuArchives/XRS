
#include "ExternalManager.h"
#include <File.h>
#include "Sample.h"
#include "Entry.h"
#include "File.h"
#include "Path.h"

//#include "file_msg.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "File.h"
ExternalManager::ExternalManager()
{
	
	samples_list=new BList(1);
	lastStatus=B_OK;
	//lastStatus=LoadDefaultBank(bankname,samples_list);
	
}
ExternalManager::~ExternalManager()
{
	Sample	*s;	
	int top=CountItems();
	for(int h=0;h<top;h++)
	{
		s=getSampleAt(h);
		delete s;
	}
	
	samples_list->MakeEmpty();
}
void
ExternalManager::Empty()
{
	Sample	*s;	
	int top=CountItems();
	for(int h=0;h<top;h++)
	{
		s=getSampleAt(h);
		delete s;
	}
	
	samples_list->MakeEmpty();
}

status_t
ExternalManager::InitCheck()
{
	return lastStatus;
}
status_t
ExternalManager::AddSample(entry_ref ref)
{
	lastStatus=LoadFile(ref,samples_list);
	return lastStatus;
}

status_t
ExternalManager::FindSample(const char* path)
{
		
		return B_OK;
}

status_t
ExternalManager::LoadFile(entry_ref ref,BList *samples_list)
{	

	status_t error;
			
	BEntry controllo_esiste(&ref);
	if(controllo_esiste.Exists()==false) return B_FILE_NOT_FOUND;
	

  	media_format		format;
  	media_header 	mh;
  	
  	int64 			buffer_pos=0;
	int64 			z=0;
	int64 			cf=0;
	
	BMediaFile	*m_source;
	BMediaTrack	*m_track;
		
		
	int64						totalbytes;
	int64						totalframes;		
	int8						default_data;
	float						frame_rate;
	uint32						frame_size;
	int32						channelCount;
	uint32						buffer_size;	
	void*						wave_data;
	void*						frame;
  
  
  	  m_source=new BMediaFile(&ref);
    
    
    	m_track=m_source->TrackAt(0);
	 
	if(m_track==NULL) return B_ERROR;
	 
	format.type=B_MEDIA_RAW_AUDIO;
	format.u.raw_audio.format=media_raw_audio_format::B_AUDIO_SHORT;
	format.u.raw_audio.frame_rate=44100;
	
				
	error=m_track->DecodedFormat(&format);
	
	//printf("Current format converted %f\n",format.u.raw_audio.frame_rate);
	
	if(error) return error;
	
	switch (format.u.raw_audio.format) {
	case media_raw_audio_format::B_AUDIO_UCHAR :
		default_data = 0x80;
		frame_size = 1;
		return B_MEDIA_BAD_FORMAT;
		break;
	case media_raw_audio_format::B_AUDIO_SHORT :
		default_data = 0;
		frame_size = 2;
		/* This is the only supported format!*/
		break;
	case media_raw_audio_format::B_AUDIO_INT :
		default_data = 0;
		frame_size = 4;
		return B_MEDIA_BAD_FORMAT;
		break;
	case media_raw_audio_format::B_AUDIO_FLOAT :
		default_data = 0;
		frame_size = 4;
		return B_MEDIA_BAD_FORMAT;
		break;
	default :
		return B_ERROR;
	}
	
	channelCount = format.u.raw_audio.channel_count;
	frame_size *= channelCount;
	buffer_size = format.u.raw_audio.buffer_size;
	frame_rate = format.u.raw_audio.frame_rate;

	
	if(frame_rate!=44100) return B_MEDIA_BAD_FORMAT;
	
	
	m_track->SeekToFrame(&cf);
	
	totalframes=m_track->CountFrames();
	totalbytes=totalframes*frame_size;
	
	frame=malloc(buffer_size); 
	
	int64 real_read=0;
	
	
	if (channelCount==2)
	{	
		
		wave_data=malloc(totalbytes*3);
		while(true){
	
			 error=m_track->ReadFrames((char*)frame,&z,&mh);
				 if(error) break;
		 
			 memcpy((char*)wave_data+buffer_pos,frame,z*frame_size);
				 if(error) break;
		 	buffer_pos+=z*frame_size;
		 	real_read++;
		 }
	}		
	else
		if(channelCount==1)	
		{
			
			wave_data=malloc(totalbytes*6);
				
			while(true){
	
			 error=m_track->ReadFrames((char*)frame,&z,&mh);
				 if(error) break;
		 
		 	
		 
			for(int s=0;s<z;s++)
			{
				 memcpy((char*)wave_data+buffer_pos+s*4,(char*)frame+s*2,frame_size);
				 memcpy((char*)wave_data+buffer_pos+(s*4)+2,(char*)frame+s*2,frame_size);
			} 	
		 			
			
			 buffer_pos+=z*frame_size*2;
		 }
		
			frame_size*=2;
			totalbytes*=2;
			channelCount=2;
			totalframes*=2;
			}
			else
			return B_MEDIA_BAD_FORMAT;
		
	
		
	printf("Duration in microseconds : %lld ; Count frames : %lld ; Real Read : %lld , TOTALYBYTES %lld \n",m_track->Duration(),m_track->CountFrames(),real_read,totalbytes);
	
	
	Sample* samp=new Sample();
	
	memcpy(samp->name,ref.name,30);
	samp->name[30]='\0';		
	samp->data=wave_data;
	samp->totalbytes=totalbytes;
	samp->type=EXT_SAMPLE_TYPE;
	
	 samples_list->AddItem(samp);	 
	
	m_track->SeekToFrame(&cf); 
	m_source->ReleaseTrack(m_track);
	m_track=NULL;
	
	BEntry e(&ref);
	BPath p(&e);
	
	samp->path_name=(char*)malloc(B_PATH_NAME_LENGTH);
	memcpy( samp->path_name, p.Path(), strlen(p.Path() ) );
	samp->path_name[strlen(p.Path() )]='\0';
	
	printf("External File loaded! [%s]\n",samp->path_name);	
	return B_OK;
}

status_t
ExternalManager::AddBankSample(BMessage* message)
{
	entry_ref	ref;
	
	message->FindRef("refs", &ref);
	
	int32 spiaz=message->FindInt32("spiaz");
	int32 size=message->FindInt32("size");
	
	lastStatus=ExtractSample( ref,spiaz ,size , message->FindString("name"),samples_list);
	return lastStatus;
}

status_t
ExternalManager::ExtractSample(entry_ref ref ,int32 spiaz ,int32 size ,const char* name,BList *samples_list)
{	

	status_t error;
			
	BEntry controllo_esiste(&ref);
	if(controllo_esiste.Exists()==false) return B_FILE_NOT_FOUND;
	
	
	BFile *file=new BFile();
    	
    	error=file->SetTo(&ref,B_READ_ONLY);
    		if(error) return error;
    	error=file->InitCheck();
    		if(error) return error;
	
	//error=file->GetSize(&file_size);
    	//	if(error) return error;
	
	
	file->Seek(spiaz,SEEK_SET);
	
	
	
	
	
	Sample* samp=new Sample();
	samp->data=(void*)malloc(size);
	file->Read(samp->data,size);
	
	memcpy(samp->name,name,30);
	samp->name[30]='\0';		
	//samp->data=wave_data;
	samp->totalbytes=size;
	samp->type=EXT_SAMPLE_TYPE;
	
	 samples_list->AddItem(samp);	 
	
	BEntry e(&ref);
	BPath p(&e);
	
	samp->path_name=(char*)malloc(B_PATH_NAME_LENGTH);
	memcpy( samp->path_name, p.Path(), strlen(p.Path() ) );
	samp->path_name[strlen(p.Path() )]='\0';
	
	printf("External File loaded! [%s]\n",samp->path_name);	
	
	delete file;
	return B_OK;
}

Sample*
ExternalManager::getSampleAt(int val)
{
	return (Sample*)samples_list->ItemAt(val);
}

status_t
ExternalManager::AddSampleList(BList* l)
{
	return samples_list->AddList(l);
}

BList*
ExternalManager::getSampleList()
{
	return samples_list;
}
int32
ExternalManager::CountItems()
{
	return samples_list->CountItems();
}