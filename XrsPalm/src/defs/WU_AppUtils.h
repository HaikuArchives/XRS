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

#ifndef _WU_AppUtils
#define _WU_AppUtils

#include <AppKit.h>
#include <StorageKit.h>

// -------------------------------------------------------------------
void
BroadcastToClients(const char* mimeAppStr, BMessage* theMesg);
// Broadcasts 'theMesg' to all running apps which have 'mimeAppStr'
// as A PART of their application's signature.
// -------------------------------------------------------------------
// Sets the path to the BApplication's directory. The results may be
// tested by thePath->InitCheck().
void
GetAppDirectoryPath(BPath* thePath);
// -------------------------------------------------------------------
// Sets the path to the BApplication. The results may be
// tested by thePath->InitCheck().
void
GetAppPath(BPath* thePath);
// -------------------------------------------------------------------
// Creates and returns a pointer to a bitmap reconstructed from a 
// resource in the app's file. Returns NULL if it can't be found.
// The caller is responsible for deleting the bitmap.
BBitmap*
GetAppBitmap(uint32 resID);
// -------------------------------------------------------------------
// Creates and returns a pointer to a BPicture reconstructed from a 
// resource in the app's file. Returns NULL if it can't be found.
// The caller is responsible for deleting the BPicture.
BPicture*
GetAppPicture(uint32 resID);

BBitmap*
GetAppJpeg(uint32 resID);
// -------------------------------------------------------------------
#endif
