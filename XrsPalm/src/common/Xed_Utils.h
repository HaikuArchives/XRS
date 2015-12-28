#ifndef _Xed_Utils
#define _Xed_Utils

#include <InterfaceKit.h>
#include <Path.h>

BPicture*	MakePicture(BBitmap*,BView*);
void		GetSkinDirectoryPath(BPath* thePath);
void		GetBankDirectoryPath(BPath* thePath);
void		GetXRSDirectoryEntry(entry_ref *f,const char *dirname);

void		GetBankRef(const char *,entry_ref*);
BPicture*	MakePictureSkin(const char *,BView*);
BBitmap*	MakeBitmapSkin(const char *name);
status_t	CheckIsBank(entry_ref *ref);
status_t	GetDefaultBankRef(entry_ref *rif);
status_t	GetDefaultSongRef(entry_ref *rif);
void		CheckMimeType();
#endif
