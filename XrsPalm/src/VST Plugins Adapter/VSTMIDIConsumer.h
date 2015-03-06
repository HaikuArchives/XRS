/*

	VSTMIDIConsumer.h

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTMIDICONSUMER_H_
#define _VSTMIDICONSUMER_H_

#include <MidiConsumer.h>
#include <String.h>

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif

bool first_producer_name (BString & port_name);

class VSTFilterPlugin;

class MIDI_consumer : public BMidiLocalConsumer {
public:
					MIDI_consumer (VSTFilterPlugin * plugin);
			bool	SetSource (const char* port_name);
			bool	SetSource (BMidiProducer * producer);
			sem_id	Finish ();
	virtual	void	NoteOff(uchar channel, uchar note, uchar velocity, bigtime_t time);
	virtual	void	NoteOn(uchar channel, uchar note, uchar velocity, bigtime_t time);
	virtual	void	KeyPressure(uchar channel, uchar note, uchar pressure, bigtime_t time);
	virtual	void	ControlChange(uchar channel, uchar controlNumber, uchar controlValue, bigtime_t time);
	virtual	void	ProgramChange(uchar channel, uchar programNumber, bigtime_t time);
	virtual	void	ChannelPressure(uchar channel, uchar pressure, bigtime_t time);
	virtual	void	PitchBend(uchar channel, uchar lsb, uchar msb, bigtime_t time);
	virtual	void	SystemCommon(uchar statusByte, uchar data1, uchar data2, bigtime_t time);
	virtual	void	SystemRealTime(uchar statusByte, bigtime_t time);
	virtual	void	TempoChange(int32 bpm, bigtime_t time);
	virtual void	AllNotesOff(bool justChannel, bigtime_t time);
private:
					~MIDI_consumer ();
	VSTFilterPlugin * fPlugin;
	AEffect *		fEffect;
	BMidiProducer *	fMidiProducer;
	BString			fProducerName;
	VstEvents		fEvents;
	VstMidiEvent	fEvent;
	sem_id			fFinish;
};

inline uint32 uint32_system_time () { return system_time () & 0xffffffff; }

#endif // _VSTMIDICONSUMER_H_