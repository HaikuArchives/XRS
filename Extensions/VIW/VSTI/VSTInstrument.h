/*

	VSTInstrument.h

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTINSTRUMENT_H_
#define _VSTINSTRUMENT_H_

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif

/*#ifndef _VSTSOUND_H_
#include "VSTSound.h"
#endif*/

/*#ifndef _PLUGIN_PROTO_H
#include "pluginproto.h"
#endif*/

#include "string.h"


#include "KernelKit.h"



#include <Locker.h>
#include "String.h"

class BListView;

class VSTInstrumentPlugin;

typedef struct {
	AEffect	*			effect;
	VSTInstrumentPlugin *	plugin;
} effectPluginPair;

class VSTInstrumentPlugin  //  : public FilterPlugin
{
public:
						VSTInstrumentPlugin (const char*);
	virtual 			~VSTInstrumentPlugin ();

	// Says if the plugin was properly loaded. If not, delete it!
	status_t			InitCheck () { return fImage > 0 ? B_OK : B_ERROR; }

	// SoundPlay communicates with the plugin with these calls:
	// Apply the audio effect on the following buffer
		
	
	void	SendNote(long,float);
	void	StopAll();
	
	
	
	virtual status_t	FilterFloat (float **input, float **output, int32 framecount, void *info);
		// Show your configuration view
	virtual BView *		Configure ();
		// Save & restore the plugins' settings
	virtual void		SetConfig (BMessage *config);
	virtual void		GetConfig (BMessage *config);
	
	// The VST plugin makes request to the host by calling the "audioMaster" call.
	// It might want to transmit the request to one the following methods.
	long				NeedIdle ();
	void				IdleLoop ();
	long				WantMidi ();

	// "audiomaster" callback needs to identify an AEffect. Hence this register
	static void			Register (VSTInstrumentPlugin * factory, AEffect * effect);
	static void			Unregister (AEffect * effect);
	static VSTInstrumentPlugin *	Identify (AEffect * effect);
	
	static void load_plug_ins (const char *path, BListView*,int);

private: // global data members for the register
	static effectPluginPair *	fPairs;
	static int			fPairsCount;
	static BLocker		fLock;
	
public:
	void				SetMidiProducer (int32 id);
	const char *			GetMidiProducerName ();
	int32				GetSampleRate () { return fSampleRate; }
	int32				GetBlockSize () { return fBlockSize; }
	uint32				GetLastFilterTime () { return fLastFilter; }

	// Grant public access to these for simplicity
	//VSTPluginFactory *	fFactory;
	AEffect *			fEffect;
	BString				name;
	
	void setBPM(int);
	
private:
	int32				fSampleRate;
	int32				fBlockSize;
	uint32				fLastFilter;
	//MIDI_consumer *		fMidiConsumer;
	//BString	*			fMidiPortName;
	image_id			fImage;
	thread_id			fIdleThread;
	sem_id				fIdleSem;
	
	
	// VSTEVENTS
	
	VstEvents	*we;
	VstMidiEvent* wme;	//start note
	VstMidiEvent* wms;	//stop nte
	
};

// VST plugins can make requests to the host through this call
long audioMaster (AEffect *eff, long opcode, long index, long value, void *ptr, float opt);

void certify_effect (AEffect * effect);



#define VST_INSTRUMENTS 1
#define VST_INSTRUMENTS_MAX_OUTPUT 16

#endif // _VSTINSTRUMENT_H_
