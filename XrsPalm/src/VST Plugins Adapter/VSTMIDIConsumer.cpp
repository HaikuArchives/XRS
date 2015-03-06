/*

	VSTMIDIConsumer.cpp

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTMIDICONSUMER_H_
#include "VSTMIDIConsumer.h"
#endif

#ifndef _VSTFILTER_H_
#include "VSTFilter.h"
#endif

#include <MidiRoster.h>
#include <MidiProducer.h>

#include <stdio.h>
#include <OS.h>

enum MidiStatus
{                             // data byte table
	kNoteOff = 0x80,           // note, off velocity
	kNoteOn = 0x90,            // note, on velocity
	kPolyPressure = 0xA0,      // note, pressure
	kController = 0xB0,        // controller, value
	kProgramChange = 0xC0,     // program
	kAfterTouch = 0xD0,        // pressure
	kPitchBend = 0xE0,         // lsb, msb
		
	kSysexStart = 0xF0,        // data byte until sysexend
	kQuarterFrame = 0xF1,      // quarter frame message
	kSongPointer = 0xF2,       // msb, lsb
	kSongSelect = 0xF3,        // number
	kSysexEnd = 0xF7,          // 
	kMidiClock = 0xF8,         // 
	kMidiClockStart = 0xFA,    //
	kMidiClockContinue = 0xFB, //
	kMidiClockStop = 0xFC,     //
	kActiveSensing = 0xFE,     //
	kMidiReset = 0xFF          //
};

// this is the time (in frames) where we are since the last call to VST_filter.
// note the fixed frame rate of 44100...
// I use uint32_system_time, which uses only 32 bits in order to be thread safe in
// reading & writing the time. Capacity is still huge!
#define FRAME long ((float (uint32_system_time () - fPlugin->GetLastFilterTime ()) * 0.044100f))

BMidiProducer * find_producer (const char* port_name)
{
	int32	id = 0;
	BMidiProducer * producer;
	while ((producer = BMidiRoster::NextProducer (&id)) != 0 && strcmp (producer->Name (), port_name) != 0 )
		producer->Release ();
	return producer;
}

bool first_producer_name (BString & port_name)
{
	int32	id = 0;
	BMidiProducer * producer;
	if ((producer = BMidiRoster::NextProducer (&id)) != 0)
	{
		port_name = producer->Name ();
		producer->Release ();
		return true;
	}
	port_name = "";
	return false;
}

MIDI_consumer::MIDI_consumer (VSTFilterPlugin * plugin) : BMidiLocalConsumer (),
	fPlugin (plugin), fEffect (plugin->fEffect), fMidiProducer (0), fFinish (0)
{
	fEvents.numEvents = 1;
	fEvents.reserved = 0;
	fEvents.events[0] = (VstEvent*) &fEvent;
	// Create a unique name in the system (the same VST plugin maybe hosted several times by different apps!)
	BString	NameBase = "SoundPlay's VST Synth plugin ";
	NameBase << plugin->fFactory->fName << " #";
	BString Name;
	int count = 1;
	BMidiConsumer * consumer;
	do
	{
		Name = NameBase;
		Name << count++;
		int32	id = 0;
		consumer = 0;
		do
		{
			if (consumer)
				consumer->Release ();
			consumer = BMidiRoster::NextConsumer (&id);
			if (consumer)
				printf ("Found consumer %s\n", consumer->Name ());
		} while (consumer && strcmp (consumer->Name (), Name.String ()) != 0);
		if (consumer)
			consumer->Release ();
		// if consumer not null here, then we have found a consumer with that name -> try an other name!
	} while (consumer);
	SetName (Name.String ());
	Register ();
	printf ("Registering %s\n", this->Name ());
}

MIDI_consumer::~MIDI_consumer ()
{
	if (fMidiProducer)
	{
		fMidiProducer->Disconnect (this);
		fMidiProducer->Release ();
	}
	if (fFinish)
		delete_sem (fFinish);
}

bool MIDI_consumer::SetSource (const char* port_name)
{
	if (fProducerName == port_name)
		return true;
	if (fMidiProducer)
	{
		fMidiProducer->Disconnect (this);
		fMidiProducer->Release ();
	}
	fMidiProducer = find_producer (port_name);
	if (fMidiProducer)
	{
		status_t r = fMidiProducer->Connect (this);
		fProducerName = port_name;
		return r == B_OK;
	}
	fProducerName = "";
	return false;
}

bool MIDI_consumer::SetSource (BMidiProducer * producer)
{
	if (producer && fMidiProducer && producer->ID () == fMidiProducer->ID ())
		return true;
	if (fMidiProducer)
	{
		fMidiProducer->Disconnect (this);
		fMidiProducer->Release ();
	}
	fMidiProducer = producer;
	if (fMidiProducer)
	{
		status_t r = fMidiProducer->Connect (this);
		fProducerName = fMidiProducer->Name ();
		return r == B_OK;
	}
	fProducerName = "";
	return false;
}

sem_id MIDI_consumer::Finish ()
{
	if (fMidiProducer)
	{
		fMidiProducer->Disconnect (this);
		fMidiProducer->Release ();
		fMidiProducer = 0;
	}
	Unregister ();
	fFinish = create_sem (0, "MIDI Producer finish");
	Release ();
	return fFinish;
}

void MIDI_consumer::NoteOff(uchar channel, uchar note, uchar velocity, bigtime_t time)
{
//	puts ("Noteoff");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kNoteOff | channel;
	fEvent.midiData[1] = note;
	fEvent.midiData[2] = velocity;
	fEvent.noteOffVelocity = velocity;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::NoteOn(uchar channel, uchar note, uchar velocity, bigtime_t time)
{
//	puts ("NoteOn");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kNoteOn | channel;
	fEvent.midiData[1] = note;
	fEvent.midiData[2] = velocity;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::KeyPressure(uchar channel, uchar note, uchar pressure, bigtime_t time)
{
//	puts ("KeyPressure");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kPolyPressure | channel;
	fEvent.midiData[1] = note;
	fEvent.midiData[2] = pressure;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::ControlChange(uchar channel, uchar controlNumber, uchar controlValue, bigtime_t time)
{
//	puts ("ControlChange");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kController | channel;
	fEvent.midiData[1] = controlNumber;
	fEvent.midiData[2] = controlValue;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::ProgramChange(uchar channel, uchar programNumber, bigtime_t time)
{
//	puts ("ProgramChange");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kProgramChange | channel;
	fEvent.midiData[1] = programNumber;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::ChannelPressure(uchar channel, uchar pressure, bigtime_t time)
{
//	puts ("ChannelPressure");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kAfterTouch | channel;
	fEvent.midiData[1] = pressure;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::PitchBend(uchar channel, uchar lsb, uchar msb, bigtime_t time)
{
//	puts ("PitchBend");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = kPitchBend | channel;
	fEvent.midiData[1] = lsb;
	fEvent.midiData[2] = msb;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::SystemCommon(uchar statusByte, uchar data1, uchar data2, bigtime_t time)
{
//	puts ("SystemCommon");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = statusByte;
	fEvent.midiData[1] = data1;
	fEvent.midiData[2] = data2;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::SystemRealTime(uchar statusByte, bigtime_t time)
{
//	puts ("SystemRealTime");
	memset (&fEvent, 0, sizeof (VstMidiEvent));
	fEvent.type = kVstMidiType;
	fEvent.byteSize = 24;
	fEvent.deltaFrames = FRAME;
	fEvent.midiData[0] = statusByte;
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0, &fEvents, 0.f);
}

void MIDI_consumer::TempoChange(int32 bpm, bigtime_t time)
{
//	puts ("TempoChange");
}

void MIDI_consumer::AllNotesOff(bool justChannel, bigtime_t time)
{
//	puts ("AllNotesOff");
}
