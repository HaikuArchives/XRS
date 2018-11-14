
#include "MBWindow.h"
#include "Application.h"
#include "stdio.h"
#include "StorageKit.h"
#include "MListView.h"
#include "ScrollView.h"
#include "SimpleGameSound.h"
#include "MListItem.h"
#include "XWindowTitle.h"
#include "WU_AppUtils.h"
#include "string.h"

BOutlineListView *list;
BSimpleGameSound *sound;

#define SELECTED_MSG 'slm'
#define INVOCATION_MSG 'inv'

#define	SAMPLE_START	400
#define	SAMPLE_STOP	401
#define	FILE_STOP		402

#define 	FILE_VERSION 	800
#define	FILE_NOTE		801
#define	SAMPLE_NAME	802
#define	SAMPLE_DATA	804
#define	XRS_BANK_SAMPLE	'xbks'


#define	X_WINDOW_ZOOM	'xwzc'
#define	XRS_SIG	"application/x-vnd.xeD.XRS"
status_t
CheckIsBank(entry_ref *ref);

XWindowTitle *title;
//B_BORDERED_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL
MBWindow::MBWindow() :
	BWindow(BRect(450,30,680,264),NULL,B_BORDERED_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL,B_WILL_ACCEPT_FIRST_CLICK |B_ASYNCHRONOUS_CONTROLS)
{
	list=new MListView(BRect(21,0,Bounds().Width()-15,Bounds().Height()-15));
	list->SetInvocationMessage(new BMessage(INVOCATION_MSG));
	list->SetSelectionMessage(new BMessage(SELECTED_MSG));
	
	AddChild(title=new XWindowTitle(BRect(0,0,20,Bounds().Height()),"MediaBrowser"));
	
	
	
	AddChild( new BScrollView("name",list,B_FOLLOW_ALL_SIDES,0,true,true) );
	sound=NULL;
	
}
void
MBWindow::AddFolder(entry_ref ref)
{
	BListItem* samples;
	list->AddItem(samples = new MListItem(&ref,ref.name,true));
	samples->SetExpanded(true);
	ParseDir(&ref,samples);
}
void
MBWindow::ParseDir(entry_ref *ref,BListItem* folder)
{
	BDirectory dir(ref);
	BEntry e;
	entry_ref rif;
	MListItem* samples;
	
	//int j=0;

	while(dir.GetNextEntry(&e,true)==B_OK)
	{
		e.InitCheck();
		
		
		if(e.IsFile())
			{
				e.GetRef(&rif);
				if(CheckIsBank(&rif)!=B_OK)
				{
					list->AddUnder(new MListItem(&rif,rif.name),folder);
					//j++;
				}
				else
				{
					list->AddUnder(samples = new MListItem(&rif,rif.name,true),folder);
					samples->SetExpanded(false);
					ParseBank(&rif,samples);
				}
			}
			
		else
		
		if(e.IsDirectory())
			{
				e.GetRef(&rif);
				list->AddUnder(samples = new MListItem(&rif,rif.name,true),folder);
				samples->SetExpanded(false);
				ParseDir(&rif,samples);
			}
	}
}

int64
MBWindow::ReadFixed(int64 *data,BFile* file)
{
	int64 code;
	file->Read(&code,sizeof(int64));
	file->Read(data,sizeof(int64));
	return code;	
}

void
MBWindow::ParseBank(entry_ref *ref,BListItem* folder)
{
	//MListItem* samples;
	
	
	char	title[10];
	char 	space=' ';
	char 	name[35];
	int32 	poz=-1;
	int32 	size=-1;
	int64	 msg,val;
	
	BFile* file=new BFile();
    	
    	file->SetTo(ref,B_READ_ONLY);	
    	file->InitCheck();	    		
	file->Seek(0,SEEK_SET);

	

	msg=ReadFixed(&val,file);
	
	if(msg==FILE_VERSION){
    	file->Read(title,10);
	}
	
	while(msg!=FILE_STOP)
	{
		msg=ReadFixed(&val,file);
		
		switch(msg){
		
		case SAMPLE_START:
			
			while(msg!=SAMPLE_STOP)
			{
				msg=ReadFixed(&val,file);
				switch(msg){
				case SAMPLE_NAME:
					memset(name,space,35);
					file->Read(name,val);
					//memset(name,space,35);
					//memcpy(name,title,val);
					name[val]='\0';
					break;
				case SAMPLE_DATA:
					
					size=val;
					poz=file->Position();
					file->Seek(val,SEEK_CUR);
					break;
					
				default:
					if(msg>=800)
					{
						//printf("Unknown code :%lld\n",msg);
						file->Seek(val,SEEK_CUR);
					}
				break;
				}
			}
		
		//get_ref_for_path(name,ref);
		//printf("name: %s\n",ref->name);
		list->AddUnder(new MListItem(ref,name,false,poz,size),folder);
		poz=-1;
		size=-1;
		break;
		default:
			if(msg>=800)
					{
						//printf("Unknown code :%lld\n",msg);
						file->Seek(val,SEEK_CUR);
					}
		break;
		}
	}
	
	//printf("Bank loaded!\n");

	
	file->Unset();
	delete file;
	

}
bool
MBWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
void
MBWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case  SELECTED_MSG:
			Activate(true);
		break;
		
		case  INVOCATION_MSG:
			{
			
			if(Lock())
			{
			
			int32 n=list->CurrentSelection(0);
			MListItem *s=(MListItem*)list->ItemAt(n);
			
			entry_ref ref;
			BMessage*	to=s->getMessage();
			
			if(to==NULL){
			 Unlock();
			 return;
			}
			
			to->FindRef("refs",&ref);
			BPath p(&ref);
			
			
			if(to->what==B_SIMPLE_DATA)
			{	
				if(sound!=NULL) {
			
					sound->StopPlaying();
					delete sound;
					sound=NULL;			
				} 
				sound=new BSimpleGameSound(&ref);
				sound->StartPlaying();			
			}
			else
			if(to->what==XRS_BANK_SAMPLE)
			{
				//int32 spiaz=to->FindInt32("spiaz");
				//We are loading a bank...
			}
			Unlock();
			}
			}
		break;
		case X_WINDOW_ZOOM:
			if(Bounds().Width()==19)
			ResizeTo(680-450,Bounds().Height());
			else
			ResizeTo(19,Bounds().Height());
		break;
		default:
		BWindow::MessageReceived(message);
	}
	
}
