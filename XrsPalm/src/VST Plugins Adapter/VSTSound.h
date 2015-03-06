/*

	VSTSound.h

	by Georges-Edouard Berenger
	Copyright © 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTSOUND_H_
#define _VSTSOUND_H_

/*#ifndef _CPLUGPLUGINAPI_H
#include "CPlusAPI.h"
#endif*/

#ifndef _VSTMIDICONSUMER_H_
#include "VSTMIDIConsumer.h"
#endif

#include <image.h>
#include <String.h>

class BList;

class VSTPluginFactory : public PluginFactory
{
public:
						VSTPluginFactory (AEffect * plugin, const char* folder,
								const char* addonpath, const char* name);
	virtual const char*	ID ();
	virtual uint32		Flags () { return PLUGIN_IS_FILTER; }
	virtual const char*	Name ();
	virtual const char*	AboutString ();
	virtual void		About ();
	virtual BView*		Configure (BMessage *config);
	virtual Plugin* 	CreatePlugin (const char *name,
								const char *header,
								uint32 size,
								plugin_info *pluginfo);
	
	// information about the plugin
	BString				fFolderPath;
	BString				fAddonPath;
	BString				fName;
	BString				fAboutText;
	BString				fID;
};

#endif // _VSTSOUND_H_