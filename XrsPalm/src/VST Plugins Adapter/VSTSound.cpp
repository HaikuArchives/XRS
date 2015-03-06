/*

	VSTSound.cpp

	by Georges-Edouard Berenger
	Copyright © 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#include <List.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <String.h>
#include <Alert.h>
#include <View.h>
#include <ListView.h>
#include <Button.h>
#include <ScrollView.h>
#include <FilePanel.h>
#include <Path.h>
#include <FindDirectory.h>
#include <File.h>
#include <StringView.h>
#include <Mime.h>

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif

#ifndef _VSTSOUND_H_
#include "VSTSound.h"
#endif

#ifndef _VSTFILTER_H_
#include "VSTFilter.h"
#endif

#ifndef _VSTCOPYRIGHT_WINDOW_H_
#include "VSTCopyrightWindow.h"
#endif

#include "TemplateList.h"

// Name of the preference file
const char * kPrefFileName = "VST Plugins Folders List";

const char * kVersionName = "VST Plugins Adapter 1.0";
const char * kVSTAdapterListName = "VST Plugins Adapter";

// Local utility functions
static void load_plug_ins (const char *path, BList& list);
static bool ReadVSTAdapterSettings (TList<BString> & folders);
static bool WriteVSTAdapterSettings (const TList<BString> & folders);

/*
	FolderListView class for the VST Plugins Folders List selection panel
	Note that this class is fairly self contained an should be directly
	reusable in some other program in any standard BWindow.
	Only needs ReadVSTAdapterSettings & WriteVSTAdapterSettings
*/
class FolderListView : public BListView {
public:
				FolderListView (BRect frame);
				~FolderListView ();
virtual	void	AttachedToWindow ();
virtual void	SelectionChanged ();
virtual	void	KeyDown (const char *bytes, int32 numBytes);
virtual	void	MessageReceived (BMessage *msg);
		void	RemoveSelection ();
private:
	BButton *	fRemove;
	BButton *	fChange;
	BFilePanel * fFilePanel;
};

/*
	ConfigureVSTPluginFactory class
	When for some reason, no VST plugin could be loaded, then you have no way
	to set where the VST plugins should be looked for.
	This fake plugin is made for that case: It will will used *only* if
	no VST plugin has been found and will allow the user to tell where the
	VST plugins are located. This plugin can not be intanciated as a filter.
*/
class ConfigureVSTPluginFactory : public VSTPluginFactory {
public:
						ConfigureVSTPluginFactory ();
	virtual Plugin* 	CreatePlugin (const char *name,
								const char *header,
								uint32 size,
								plugin_info *pluginfo);
};

ConfigureVSTPluginFactory::ConfigureVSTPluginFactory () :
	VSTPluginFactory (NULL, B_EMPTY_STRING, B_EMPTY_STRING, B_EMPTY_STRING)
{
	fID = kVSTAdapterListName;
	fAboutText = "This fake plugin allows you to configure where the VST plugins should be looked for.\n\n"
	             "It only allows you to access the configure panel.";
	fName = kVSTAdapterListName;
}


Plugin* ConfigureVSTPluginFactory::CreatePlugin (const char *name, const char *header, uint32 size, plugin_info *pluginfo)
{
	BAlert* alert = new BAlert (NULL,
		"This is a fake plugin to allow you to set where VST plugins are located!\nYou can't use it as an effect!",
		"OK!", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
	alert->Go();	
	return NULL;
}

/*
	Create the BList of PluginFactory objects (one per plugin)
*/
status_t _EXPORT GetPluginFactories (BList *out_list)
{
	// Always insert the VST Plugins Adapter configuration plugin
	out_list->AddItem (new ConfigureVSTPluginFactory ());
	
	TList<BString> folders;

	ReadVSTAdapterSettings (folders);
	
	// To help identify bad VST Plugins which crash when they are loaded,
	// the loading thread will be renamed. Here, we save the thread's original name.
	thread_info	infos;
	status_t	s;
	s = get_thread_info (find_thread (NULL), &infos);
	// folders now contains the list of VST plugins folders
	// Now load each of these folders
	for (int f = 0; f < folders.CountItems (); f++)
		load_plug_ins (folders.ItemAt (f)->String (), *out_list);

	// If no plugin was really loaded, then use the default configuration plugin
	if (out_list->CountItems () < 1)
		out_list->AddItem (new ConfigureVSTPluginFactory ());
	
	// Restore the thread's name when we are done.
	if (s == B_OK)
		rename_thread (infos.thread, infos.name);
	return B_OK;
}

/*
	This function scans the directory given and adds new plugins to the BList object given.
	If a folder is found, it is scanned recursively.
*/
void load_plug_ins (const char *rootdir, BList& list)
{
	DIR* dir = opendir (rootdir);
	if (dir)
	{
		struct dirent * entry = readdir (dir);
		while (entry)
		{
			if (strcmp (entry->d_name, ".") != 0 && strcmp (entry->d_name, "..") != 0)
			{
				char path[PATH_MAX];
				strcpy (path, rootdir);
				strcat (path, "/");
				strcat (path, entry->d_name);

				struct stat st;
				if (stat (path, &st) == 0)
				{
					if (S_ISREG (st.st_mode))
					{
						BNode	node (path);
						char	type[B_MIME_TYPE_LENGTH];
						if (node.InitCheck () == B_OK
							&& node.ReadAttr ("BEOS:TYPE", B_MIME_STRING_TYPE, 0, type, B_MIME_TYPE_LENGTH) > 0
							&& strcmp (type, B_APP_MIME_TYPE) == 0) {
							// To help debugging if a crash occurs...
							printf ("Loading \"%s\" as an addon... ", path);
							fflush (stdout);
							char	name[B_OS_NAME_LENGTH];
							strncpy (name, entry->d_name, B_OS_NAME_LENGTH);
							rename_thread (find_thread (NULL), name);
							// an executable has been found. Try to load it as a VST plugin
							image_id vstaddon = load_add_on (path);
							if (vstaddon > 0)
							{	// the file is indeed an addon, but is it a VST plugin?
								printf ("OK! VST Plugin?... ");
								fflush (stdout);
								AEffect * effect;
								AEffect * (*main_plugin) (audioMasterCallback audioMaster);
								if (get_image_symbol (vstaddon, "main_plugin", B_SYMBOL_TYPE_TEXT, (void**) &main_plugin) == B_OK)
								{	// Chances are now that this is a VST plugin, but is it supported?...
									printf ("Yes!\n");
									effect = (*main_plugin) (&audioMaster);
									if (effect && effect->magic == kEffectMagic)
									{
										if (
											#if VST_INSTRUMENTS
											(((effect->flags & effFlagsIsSynth) && effect->numOutputs <= VST_INSTRUMENTS_MAX_OUTPUT) || // for VST Instruments
												(effect->numOutputs <= 2))	// "normal" plugins
											#else
											// VST Instrument not allowed
											(effect->flags & effFlagsIsSynth) == 0
											&& effect->numOutputs <= 2
											#endif
											//&& effect->dispatcher (effect, effGetVstVersion, 0, 0, 0, 0) >= 1
											&& effect->numInputs <= 2)
										{
											effect->dispatcher (effect, effOpen, 0, 0, 0, 0.);
											// the VST plugin has been opened. Now we can create the "PluginFactory" object.
											list.AddItem (new VSTPluginFactory (effect, rootdir, path, entry->d_name));
										}
										effect->dispatcher (effect, effClose, 0, 0, 0, 0.);
									}
								} else
									printf ("No!\n");
								unload_add_on (vstaddon);
							} else
								printf ("Not an addon!\n");
						}
					}
					else if (S_ISDIR (st.st_mode))
						load_plug_ins (path, list);
				}
			}
			entry = readdir (dir);
		}
		closedir (dir);
	}
}

/*
	VSTPluginFactory definition.
	This object allows SoundPlay to display the list of plugins which can be instanciated.
	Note that this object needs an instanciated VST plugin to be created, but that after
	creation, it will not ever access that original VST plugin which is considered to be gone.
	When a VST host application is launched, it usualy scan a folder in order to find its plugins.
	In the VST world, each plugin will be instanciated then, tested to see if it is supported and 
	find out its properties (number of channels, etc). The plugins are then immediatly unloaded until
	a plugin is "really" needed for an effect.
*/
VSTPluginFactory::VSTPluginFactory (AEffect * plugin, const char* folder,
	const char* addonpath, const char* name) : fFolderPath (folder), fAddonPath (addonpath)
	
{
	if (plugin)
	{
		char	effectName[32];			// effGetEffectName
		char	effectVendor[64];		// effGetVendorString
		char	effectProduct[64];		// effGetProductString
		long	effectVendorVersion;	// effGetVendorVersion
		if (!plugin->dispatcher (plugin, effGetEffectName, 0, 0, effectName, 0.))
			strncpy (effectName, name, 32);
		if (!plugin->dispatcher (plugin, effGetVendorString, 0, 0, effectVendor, 0.))
			strcpy (effectVendor, "<Unknown vendor>");
		if (!plugin->dispatcher (plugin, effGetProductString, 0, 0, effectProduct, 0.))
			strcpy (effectProduct, "<Unnamed product>");
		effectVendorVersion = plugin->dispatcher (plugin, effGetVendorVersion, 0, 0, effectName, 0.);
	//	printf ("Plugin: %s        Vendor: %s       Product: %s      Version: %d\n", effectName, effectVendor, effectProduct, effectVendorVersion);
		fID << "VST Plugin " << plugin->uniqueID << "v" << effectVendorVersion;
		fName << "VST " << effectName;
		fAboutText << effectProduct << " by " << effectVendor << ".";
	}
}

/*
	Note that none of the BString object is ever modified after the object is created.
	Therefore, the const char* pointers returned by those BString objects can be considered
	constant until the object itself is deleted.
*/
const char * VSTPluginFactory::ID ()
{
	return fID.String ();
}

const char * VSTPluginFactory::Name ()
{
	return fName.String ();
}

const char * VSTPluginFactory::AboutString ()
{
	return fAboutText.String ();
}

void VSTPluginFactory::About ()
{
	new VSTCopyrightWindow (kVersionName);
}

BView* VSTPluginFactory::Configure (BMessage *)
{
	// Select which folders are containing VST plugins
	// Don't use SoundPlay's storage, so that other hosts can share this global setting!
	BRect frame (0, 0, 300, 160);
	BView *back = new BView (frame, B_EMPTY_STRING, B_FOLLOW_ALL, B_WILL_DRAW);
	back->SetViewColor (ui_color (B_PANEL_BACKGROUND_COLOR));
	back->AddChild (new BStringView (BRect (7, 1, 300, 15), B_EMPTY_STRING, "Your VST plugins folders:"));
	frame.top += 25;
	frame.left += 10;
	frame.right -= 100;
	frame.bottom -= 48;
	back->AddChild (new BScrollView (NULL, new FolderListView (frame), B_FOLLOW_ALL, 0, false, true));
 	return back;
}

Plugin* VSTPluginFactory::CreatePlugin (const char *name, const char *header, uint32 size, plugin_info *pluginfo)
{
	VSTFilterPlugin * plugin = new VSTFilterPlugin (this);
	if (plugin->InitCheck () == B_OK)
		return plugin;
	delete plugin;
	return NULL;
}

/*
	Reads the setting file and builds the list of VST plugins folders.
	The setting file is a plain text file containing a list of full pathnames.
	Therefore, it can be edited by hand if necessary!
*/
static bool ReadVSTAdapterSettings (TList<BString> & folders)
{
	BPath path;
	find_directory (B_COMMON_SETTINGS_DIRECTORY, &path, true);
	path.Append (kPrefFileName);
	BFile settings (path.Path (), B_READ_ONLY);
	if (settings.InitCheck () == B_OK)
	{
		off_t	size;
		settings.GetSize (&size);
		if (size > 0)
		{
			char * buf = new char[size];
			if (settings.Read (buf, size) == size)
			{
				char	path[B_PATH_NAME_LENGTH];
				off_t	cursor = 0;
				while (cursor < size)
				{
					int	c = 0;
					while (c < B_PATH_NAME_LENGTH && cursor < size && buf[cursor] != '\n')
						path[c++] = buf[cursor++];
					path[c] = 0;
					while (cursor < size && buf[cursor] == '\n')
						cursor++;
					if (c > 0)
						folders.AddItem (new BString (path));
				}
			}
		}
	}
	// Default folder if file is empty, or if no setting file was found
	if (folders.CountItems () < 1)
		folders.AddItem (new BString ("/boot/home/config/add-ons/VST Plugins"));
	return true;
}

/*
	(Re)creates the setting file based on the list of plugins given.
*/
static bool WriteVSTAdapterSettings (const TList<BString> & folders)
{
	BPath path;
	find_directory (B_COMMON_SETTINGS_DIRECTORY, &path, true);
	path.Append (kPrefFileName);
	BFile settings (path.Path (), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (settings.InitCheck () == B_OK)
	{
		for (int i = 0; i < folders.CountItems (); i++)
		{
			BString * s = folders.ItemAtFast (i);
			settings.Write (s->String (), s->Length ());
			settings.Write ("\n", 1);
		}
	}
	return true;
}

/*
	Implementation of the FolderListView class
*/
FolderListView::FolderListView (BRect frame) :
	BListView (frame, B_EMPTY_STRING, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL),
	fFilePanel (NULL)
{
}

FolderListView::~FolderListView ()
{
	if (fFilePanel)
		delete fFilePanel;
}

void FolderListView::AttachedToWindow ()
{
	BListView::AttachedToWindow ();
	// Read the settings and populate the list
	TList<BString> folders;
	ReadVSTAdapterSettings (folders);
	for (int f = 0; f < folders.CountItems (); f++)
		AddItem (new BStringItem (folders.ItemAt (f)->String ()));
	// Add the buttons for the interface
	BView * parent = Parent ();
	BRect listframe;
	if (parent)
	{
		listframe = parent->Frame ();
		parent = parent->Parent ();
	}
	if (parent)
	{
		BRect frame = parent->Bounds ();
		float	buttonWidth = frame.right - listframe.right - 2 * listframe.left;
		BRect framebutton (listframe.right + listframe.left, listframe.top, 0, 0);
		framebutton.right = framebutton.left + buttonWidth;
		BButton * b = new BButton (framebutton, B_EMPTY_STRING, "Add", new BMessage ('Add '), B_FOLLOW_RIGHT);
		b->SetTarget (this);
		parent->AddChild (b);
		framebutton = b->Frame ();	// button has been resized when attached to the window
		float	buttonHeight = framebutton.bottom - framebutton.top;
		framebutton.OffsetBy (0, buttonHeight + 10);
		fChange = new BButton (framebutton, B_EMPTY_STRING, "Change", new BMessage ('Chan'), B_FOLLOW_RIGHT);
		fChange->SetTarget (this);
		parent->AddChild (fChange);
		framebutton.OffsetBy (0, buttonHeight + 10);
		fRemove = new BButton (framebutton, B_EMPTY_STRING, "Remove", new BMessage ('Remo'), B_FOLLOW_RIGHT);
		fRemove->SetTarget (this);
		parent->AddChild (fRemove);
		framebutton.OffsetTo (framebutton.left, frame.bottom - buttonHeight - 10);
		b = new BButton (framebutton, B_EMPTY_STRING, "Done", new BMessage ('Done'), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
		b->SetTarget (this);
		parent->AddChild (b);
		framebutton.OffsetTo (listframe.left, framebutton.top);
		b = new BButton (framebutton, B_EMPTY_STRING, "Cancel", new BMessage ('Canc'), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
		b->SetTarget (this);
		parent->AddChild (b);
		SelectionChanged ();
	}
}

void FolderListView::SelectionChanged ()
{
	bool enable = CurrentSelection () >= 0;
	fRemove->SetEnabled (enable);
	fChange->SetEnabled (enable);
}

void FolderListView::KeyDown (const char *bytes, int32 numBytes)
{
	if (numBytes == 1 && (*bytes == B_DELETE || *bytes == B_BACKSPACE))
		RemoveSelection ();
	else
		BListView::KeyDown (bytes, numBytes);
}

void FolderListView::RemoveSelection ()
{
	int32	Nselection = CurrentSelection ();
	if (Nselection >= 0) 
	{
		delete (BStringItem *) RemoveItem (Nselection);
		if (Nselection < CountItems ())
			Select (Nselection);
		else if (CountItems() > 0)
			Select (Nselection - 1);
	}
}

void FolderListView::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case 'Add ':
			if (!fFilePanel)
			{
				BMessenger me (this);
				fFilePanel = new BFilePanel (B_OPEN_PANEL, &me, 0, B_DIRECTORY_NODE, false);
				fFilePanel->Show ();
			}
			break;
		case 'Chan':
			if (!fFilePanel)
			{
				entry_ref	ref;
				entry_ref	* refp = NULL;
				BStringItem * item = (BStringItem *) ItemAt (CurrentSelection ());
				if (item)
				{
					BEntry entry (item->Text (), false);
					if (entry.InitCheck () == B_OK && entry.GetParent (&entry) == B_OK)
					{
						entry.GetRef (&ref);
						refp = &ref;
					}
				}
				BMessenger me (this);
				BMessage * m = new BMessage (B_REFS_RECEIVED);
				m->AddString ("source", item->Text ());
				fFilePanel = new BFilePanel (B_OPEN_PANEL, &me, refp, B_DIRECTORY_NODE, false, m);
				fFilePanel->Show ();
			}
			break;
		case B_REFS_RECEIVED:
		{
			delete fFilePanel;
			fFilePanel = NULL;
			entry_ref	ref;
			if (msg->FindRef ("refs", &ref) == B_OK)
			{
				BEntry entry (&ref);
				BPath path (&entry);
				const char * original;
				if (msg->FindString ("source", &original) == B_OK)
				{
					for (int i = 0; i < CountItems (); i++)
					{
						BStringItem * item = (BStringItem *) ItemAt (i);
						if (strcmp (item->Text (), original) == 0)
						{
							item->SetText (path.Path ());
							break;
						}
					}
				}
				else
					AddItem (new BStringItem (path.Path ()));
			}
			break;
		}
		case B_CANCEL:
			delete fFilePanel;
			fFilePanel = NULL;
			break;
		case 'Remo':
			RemoveSelection ();
			break;
		case 'Done':
		{
			TList<BString> folders;
			for (int i = 0; i < CountItems (); i++)
			{
				BStringItem * item = (BStringItem *) ItemAt (i);
				folders.AddItem (new BString (item->Text ()));
			}
			WriteVSTAdapterSettings (folders);
			Window ()->PostMessage (B_QUIT_REQUESTED);
			BAlert* alert = new BAlert (NULL,
				"You must restart SoundPlay for the changes to take effect!",
				"OK!", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
			alert->Go();	
			break;
		}
		case 'Canc':
			Window ()->PostMessage (B_QUIT_REQUESTED);
			break;
		default:
			BListView::MessageReceived(msg);
	}
}
