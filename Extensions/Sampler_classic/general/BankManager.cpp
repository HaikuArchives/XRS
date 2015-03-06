
#include "BankManager.h"
#include <File.h>
#include "Sample.h"
#include "Entry.h"
#include "File.h"

#include "Xed_Utils.h"
#include "file_msg.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

BankManager::BankManager(const char* bankname)
{
	
	samples_list=new BList(1);
	lastStatus=B_OK;
	::GetBankRef(bankname,&the_ref);
	lastStatus=LoadDefaultBank(bankname,samples_list);
	
}
BankManager::~BankManager()
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
BankManager::InitCheck()
{
	return lastStatus;
}


status_t
BankManager::LoadDefaultBank(const char *bankname,BList *samples_list)
{	
	entry_ref ref;//"DefaultBank"
	::GetBankRef(bankname,&ref);
	char space=' ';
	status_t error;
	char	title[10];
		
	BEntry controllo_esiste(&ref);
	if(controllo_esiste.Exists()==false) return B_ERROR;
	
	BFile* file=new BFile();
    	
    	error=file->SetTo(&ref,B_READ_ONLY);
    		if(error!=B_OK) return error;
    	error=file->InitCheck();
    		if(error!=B_OK) return error;
	
	file->Seek(0,SEEK_SET);

	int64 msg,val;

	msg=ReadFixed(&val,file);
	
	if(msg==FILE_VERSION){
    	file->Read(title,10);
    	//printf("song name: %s -- %s\n",title,ref.name);
	}
	Sample* samp;
	
	while(msg!=FILE_STOP)
	{
		msg=ReadFixed(&val,file);
		switch(msg){
		case SAMPLE_START:
			samp=new Sample();
			while(msg!=SAMPLE_STOP)
			{
				msg=ReadFixed(&val,file);
				switch(msg){
				case SAMPLE_NAME:
					file->Read(title,val);
					memset(samp->name,space,30);
					memcpy(samp->name,title,val);
					samp->name[val+1]='\0';
					//printf("Sample name :%s (size %lld)\n",samp->name,val);
					break;
				case SAMPLE_DATA:
					samp->data=malloc(val);
					samp->totalbytes=val;
					samp->type=BANK_SAMPLE_TYPE;
					file->Read(samp->data,val);
					break;
				default:
					if(msg>=800)
					{
						printf("Unknown code :%lld\n",msg);
						file->Seek(val,SEEK_CUR);
					}
				break;
				}
			}
			samples_list->AddItem(samp);
		break;
		default:
			if(msg>=800)
					{
						printf("Unknown code :%lld\n",msg);
						file->Seek(val,SEEK_CUR);
					}
		break;
		}
	}
	
	printf("Bank loaded!\n");

	
	file->Unset();
	delete file;
	
	return B_OK;
}

int64
BankManager::ReadFixed(int64 *data,BFile* file)
{
	int64 code;
	file->Read(&code,sizeof(int64));
	file->Read(data,sizeof(int64));
	return code;	
}


Sample*
BankManager::getSampleAt(int val)
{
	return (Sample*)samples_list->ItemAt(val);
}

BList*
BankManager::getSampleList()
{
	return samples_list;
}
int32
BankManager::CountItems()
{
	return samples_list->CountItems();
}