/* -------------------------------------------------------------------

	Title:
		WU_AppUtils

	Description:
		Miscellaneous application utilities

	Author :
		Stephen Wardlaw, M.D.
		Medical Reserch & Development
		Highrock
		Lyme, Ct 06371
		<swardlaw@connix.com>

	Edit History:
		21 Octomer, 1999
			Assembled from bits & pieces

------------------------------------------------------------------- */

#include "WU_AppUtils.h"

#include <SupportKit.h>
#include <InterfaceKit.h>
#include <strings.h>
#include "TranslationKit.h"
// -------------------------------------------------------------------
void
BroadcastToClients(const char* mimeAppStr, BMessage* theMesg)
{
	// Make a signature string of prospective clients
	// Get a list of all running applications
	BList* appList = new BList();
	be_roster->GetAppList(appList);
	
	// Look through list for matching signatures
	app_info theInfo;
	team_id theTeam;
	int32 numApps = appList->CountItems();
	while(numApps > 0) {
		theTeam = (team_id)(*(int*)appList->ItemAt(numApps - 1));
		status_t result = be_roster->GetRunningAppInfo(theTeam,&theInfo);
		if(result == B_OK) {
			// Compare signature and send message if match
			if(::strstr(theInfo.signature,mimeAppStr) != NULL) {
				BMessenger* theMessenger = new BMessenger(NULL,theTeam);
				if(theMessenger->IsValid()) {
					theMessenger->SendMessage(theMesg);
				}
				delete theMessenger;
			}
		}
		numApps--;
	}
	delete appList;
	delete theMesg;
}
// -------------------------------------------------------------------
void
GetAppDirectoryPath(BPath* thePath)
{
	app_info theInfo;
	BEntry 	theEntry;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath->SetTo(&theDirectory,NULL);
		}
	}  
}
// -------------------------------------------------------------------
void
GetAppPath(BPath* thePath)
{
	app_info theInfo;
	be_app->GetAppInfo(&theInfo);
	GetAppDirectoryPath(thePath);
	if(thePath->InitCheck() == B_OK) {
		thePath->Append(theInfo.ref.name);
	}
}
// -------------------------------------------------------------------
BBitmap*
GetAppJpeg(uint32 resID)
{
	BBitmap* theMap = NULL;
	BMemoryIO *mem;
	size_t length;
	
	BPath thePath;
	GetAppPath(&thePath);
	if(thePath.InitCheck()== B_OK) {
		BFile appFile(thePath.Path(),B_READ_ONLY);
		if(appFile.InitCheck() == B_OK) {
			BResources* theRes = new BResources(&appFile);
			const char* theData = (const char*)theRes->LoadResource(B_RAW_TYPE,resID,&length);
			
			
			mem=new BMemoryIO(theData,length);
			if(mem!=NULL)
			theMap=BTranslationUtils::GetBitmap(mem);
			//delete theRes;
		}
	}  
	// Make sure it's what we have
	
	
	
	
	if(theMap != NULL) {
		return theMap;
	} else {

		delete mem;
		return NULL;
	}
}

// -------------------------------------------------------------------
BPicture*
GetAppPicture(uint32 resID)
{
	size_t length;
	BArchivable* theObj = NULL;
	BPath thePath;
	GetAppPath(&thePath);
	if(thePath.InitCheck()== B_OK) {
		BFile appFile(thePath.Path(),B_READ_ONLY);
		if(appFile.InitCheck() == B_OK) {
			BResources* theRes = new BResources(&appFile);
			const char* theData = (const char*)theRes->LoadResource(B_RAW_TYPE,resID,&length);
			if(theData != NULL) {
				BMessage* theMesg = new BMessage();
				if(theMesg->Unflatten(theData) == B_OK) {
					theObj = instantiate_object(theMesg);
				}
				delete theMesg;
			}
			delete theRes;
		}
	}  
	// Make sure it's what we have
	BPicture* thePic = cast_as(theObj,BPicture);
	if(thePic != NULL) {
		
		return thePic;
	} else {
		
		delete theObj;
		return NULL;
	}
}
// -------------------------------------------------------------------
