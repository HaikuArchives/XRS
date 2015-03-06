
#include "Xed_Utils.h"
#include "Application.h"
#include "Roster.h"
#include "StorageKit.h"
#include <InterfaceKit.h>
#include "TranslationUtils.h"
#include "stdio.h"
#include "string.h"


BPicture*
MakePicture(BBitmap* bit ,BView* vie)
{
	BPicture *on;
	//BView *w= new BView(BRect(10,10,20,20));
	//AddChild(w);
	BRect rect(bit->Bounds());
	BView *tempView = new BView( rect, "temp", B_FOLLOW_NONE, B_WILL_DRAW );
	
	vie->AddChild(tempView);
	
	tempView->BeginPicture(new BPicture); 
   	tempView->DrawBitmap(bit);
   	on = tempView->EndPicture();
   	vie->RemoveChild(tempView);
   	delete tempView;
   	
   	return on;
}
void
GetSkinDirectoryPath(BPath* thePath)
{
	app_info theInfo;
	BEntry 	theEntry;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath->SetTo(&theDirectory,"Skin");
			thePath->InitCheck();
			
		}
	}  
}
void
GetBankDirectoryPath(BPath* thePath)
{
	app_info theInfo;
	BEntry 	theEntry;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath->SetTo(&theDirectory,"Banks");
			thePath->InitCheck();
			
		}
	}  
}
void
GetXRSDirectoryEntry(entry_ref *f,const char *dirname)
{
	app_info theInfo;
	BEntry 	theEntry;
	BPath	thePath;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath.SetTo(&theDirectory,dirname);
			thePath.InitCheck();
			get_ref_for_path(thePath.Path(),f);
			
		}
	}  
}
void
GetBankRef(const char* name,entry_ref*rif)
{
	BPath 		thePath;
	
	
	GetBankDirectoryPath(&thePath);
	
	thePath.Append(name);
	
	get_ref_for_path(thePath.Path(),rif);
}
BPicture*	
MakePictureSkin(const char *name,BView *w)
{
	BPath 		thePath;
	BBitmap	 *xxx=NULL;
	BPicture		*pic;
	
	GetSkinDirectoryPath(&thePath);
	
	thePath.Append(name);
	
	//printf("The file : %s \n", thePath.Path());
	
	xxx=BTranslationUtils::GetBitmap(thePath.Path());
	
	pic=MakePicture(xxx,w);
	delete xxx;
	return pic;

}
BBitmap*
MakeBitmapSkin(const char *name)
{
	BPath 		thePath;
	GetSkinDirectoryPath(&thePath);
	
	thePath.Append(name);
	
	//printf("The file : %s \n", thePath.Path());
	
	return BTranslationUtils::GetBitmap(thePath.Path());
}
status_t
CheckIsBank(entry_ref *ref)
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

status_t
GetDefaultBankRef(entry_ref *rif)
{
	GetBankRef("DefaultBank",rif);
	BEntry e(rif,true);
	e.GetRef(rif);
	if(e.Exists()) return B_OK;
			else return B_ERROR;
}
status_t
GetDefaultSongRef(entry_ref *rif)
{
	BPath 		thePath;
	
	
	GetXRSDirectoryEntry(rif,"Songs");
	
	BEntry e(rif,true);
	
	thePath.SetTo(&e);
	
	thePath.Append("DefaultSong");
	
	get_ref_for_path(thePath.Path(),rif);
	
	
	e.SetTo(rif);
	e.GetRef(rif);
	if(e.Exists()) return B_OK;
			else return B_ERROR;
}
void
CheckMimeType()
{
	BMimeType m("audio/XRS-File");
	m.InitCheck();
	if(!m.IsInstalled())
		{
			m.SetPreferredApp("application/x-vnd.xeD.XRS");
			m.Install();
			printf("MimeType installed!\n");
		}
//}
	
}
