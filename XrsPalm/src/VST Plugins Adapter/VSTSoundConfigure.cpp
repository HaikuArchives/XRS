/*

	VSTSoundConfigure.cpp

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTSOUND_CONFIGURE_H_
#include "VSTSoundConfigure.h"
#endif

#ifndef _VSTSOUND_H_
#include "VSTSound.h"
#endif

#ifndef _VSTFILTER_H_
#include "VSTFilter.h"
#endif

#include <Window.h>
#include <Messenger.h>
#include <MessageRunner.h>
#include <StringView.h>
#include <TextControl.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <MidiRoster.h>
#include <MidiProducer.h>

const float kConfigHeight = 35;

VSTConfigureView::VSTConfigureView (VSTFilterPlugin * plugin) :
	BView (BRect (0, 0, 10, 10), B_EMPTY_STRING, 0, B_WILL_DRAW),
	fMidiSelect (0), fPlugin (plugin), fMessageRunner (0), fEffect (plugin->fEffect)
{
	
	fEffect->dispatcher (fEffect, effOpen, 0, 0, 0, 0.);
	short *rect = 0;
	fEffect->dispatcher (fEffect, effEditGetRect, 0, 0, &rect, 0);
	if (rect)
	{
		ResizeTo (float (rect[3] - 1), float (rect[2] - 1) + kConfigHeight);
		BRect frame (Bounds ());
		frame.top += kConfigHeight;
		BView * host = new BView (frame, 0, 0, 0);
		AddChild (host);
		fEffect->dispatcher (fEffect, effEditOpen, 0, 0, host, 0);
	}
	else
	{
		int count = fEffect->numParams;
		const float kParameterWidth = 275;
		const float kParameterHeight = 40;
		ResizeTo (kParameterWidth, kParameterHeight * count + kConfigHeight);
		BRect frame (0, kConfigHeight, kParameterWidth, kConfigHeight + 39);
		for (int p = 0; p < count; p++)
		{
			ParameterSlider *  ps = new ParameterSlider (frame, fEffect, p);
			AddChild (ps);
			frame.top += kParameterHeight;
			frame.bottom += kParameterHeight;
		}
	}
}

void VSTConfigureView::AttachedToWindow ()
{
	BWindow * window = Window ();
	if (window)
		window->SetFlags (window->Flags () | B_NOT_ZOOMABLE | B_NOT_RESIZABLE);
	BMessenger messenger (this);
	fMessageRunner = new BMessageRunner (messenger, new BMessage ('Idle'), 10000, -1);
	SetViewColor (Parent ()->ViewColor ());
	BRect	frame (3, 7, 150, 30);
	char	program[32];
	fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, program, 0);
	fNameControl = new BTextControl (frame, B_EMPTY_STRING, "Program:", program, new BMessage ('Name'));
	fNameControl->SetDivider (45);
	fNameControl->TextView ()->SetMaxBytes (32);
	AddChild (fNameControl);
	fNameControl->SetTarget (this);
	if (fEffect->flags & effFlagsIsSynth)
	{
		const char * port = fPlugin->GetMidiProducerName ();
		if (!port || !*port)
			port = "Select";
		BPopUpMenu * menu = new BPopUpMenu (port);
		int32	id = 0;
		BMidiProducer * producer;
		while ((producer = BMidiRoster::NextProducer (&id)) != 0)
		{
			BMessage * message = new BMessage ('Midi');
			message->AddInt32 ("id", id);
			BMenuItem * item = new BMenuItem (producer->Name (), message);
			if (strcmp (producer->Name (), port) == 0)
				item->SetMarked (true);
			item->SetTarget (this);
			menu->AddItem (item);
			producer->Release ();
		}
		frame.OffsetBy (frame.right + 20, -1);
		frame.right += 30;
		fMidiSelect = new BMenuField (frame, B_EMPTY_STRING, "MIDI source:", menu);
		fMidiSelect->SetDivider (62);
		AddChild (fMidiSelect);
	}
}

void VSTConfigureView::DetachedFromWindow ()
{
	fEffect->dispatcher (fEffect, effEditClose, 0, 0, 0, 0.f);
	if (fMessageRunner)
		delete fMessageRunner;
}

void VSTConfigureView::MessageReceived (BMessage *msg)
{
	if (msg->what == 'Idle')
		fEffect->dispatcher (fEffect, effEditIdle, 0, 0, 0, 0.f);
	else if (msg->what == 'Name')
		fEffect->dispatcher (fEffect, effSetProgramName, 0, 0, (char*) fNameControl->Text (), 0);
	else if (msg->what == 'Prog')
	{
		int32	index;
		if (msg->FindInt32 ("program", &index) == B_OK)
		{
			fEffect->dispatcher (fEffect, effSetProgram, 0, index, 0, 0.f);
			char	name[32];
			fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, name, 0);
			fNameControl->SetText (name);
			index = 0;
			BView * child;
			while ((child = ChildAt (index++)) != 0)
			{
				ParameterSlider * slider = dynamic_cast<ParameterSlider*> (child);
				if (slider)
					slider->LoadParameter ();
			}
		}
	}
	else if (msg->what == 'Midi')
	{
		int32	id;
		if (msg->FindInt32 ("id", &id) == B_OK)
			fPlugin->SetMidiProducer (id);
	}
	else
		BView::MessageReceived (msg);
}

void VSTConfigureView::WindowActivated (bool state)
{
	if (state)
		fEffect->dispatcher (fEffect, effEditTop, 0, 0, 0, 0.f);
	else
		fEffect->dispatcher (fEffect, effEditSleep, 0, 0, 0, 0.f);
}

const float	clic_x = 161;
const float	clic_y = 13;
const float range = 10;

void VSTConfigureView::MouseDown (BPoint where)
{
	if (where.x < clic_x - range || where.x > clic_x + range)
		return; 
	if (where.y < clic_y - range || where.y > clic_y + range)
		return; 
	BPopUpMenu * popup = new BPopUpMenu ("Popup", false, false);
	popup->SetFont(be_plain_font);
	BMenuItem* item;
	
	int32 currentProgram = fEffect->dispatcher (fEffect, effGetProgram, 0, 0, 0, 0.f);
	int		progCount = fEffect->numPrograms;
	char	name[32];
	for (int prog = 0; prog < progCount; prog++)
	{
		fEffect->dispatcher (fEffect, effSetProgram, 0, prog, 0, 0.f);
		fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, name, 0);
		BMessage * message = new BMessage ('Prog');
		message->AddInt32 ("program", prog);
		item = new BMenuItem (name, message);
		item->SetTarget (this);
		if (currentProgram == prog)
			item->SetMarked (true);
		popup->AddItem (item);
	}
	fEffect->dispatcher (fEffect, effSetProgram, 0, currentProgram, 0, 0.f);

	BRect r (where.x - 10, where.y - 10, where.x + 10, where.y + 10);
	where += BPoint (-10, -10);
	popup->Go (ConvertToScreen (where), true, true, r, true);
}

void VSTConfigureView::Draw (BRect updateRect)
{
	SetHighColor (32, 32, 32);
	StrokeLine (BPoint (clic_x - 3, clic_y), BPoint (clic_x + 3, clic_y));
	StrokeLine (BPoint (clic_x - 2, clic_y + 1), BPoint (clic_x + 2, clic_y + 1));
	StrokeLine (BPoint (clic_x - 1, clic_y + 2), BPoint (clic_x + 1, clic_y + 2));
	StrokeLine (BPoint (clic_x - 0, clic_y + 3), BPoint (clic_x + 0, clic_y + 3));
}

void clean_string (char* d)
{	// removes leading & ending spaces & tabs
	char* f = d;
	char* s = d;
	while (*s == ' ' || *s == '\t')
		s++;
	while (*s)
		*d++ = *s++;
	d--;
	while (d >= f && (*d == ' ' || *d == '\t'))
		d--;
	d++;
	*d = 0;
}

// Generic Parameter
ParameterSlider::ParameterSlider (BRect frame, AEffect * effect, int index)
	:BSlider (frame, B_EMPTY_STRING, "", NULL, 0, 10000), fEffect (effect), fIndex (index), fStore (false)
{
	frame.OffsetTo (B_ORIGIN);
	frame.left = (frame.right + frame.left) / 2;
	frame.bottom = frame.top + 13;
	fEffect->dispatcher (fEffect, effGetParamLabel, fIndex, 0, fUnit, 0.f);
	clean_string (fUnit);
	fDisplay = new BStringView (frame, NULL, fUnit);
	AddChild (fDisplay);
	fDisplay->SetAlignment (B_ALIGN_RIGHT);
}
					
void ParameterSlider::SetValue (int32 v)
{
	BSlider::SetValue (v);
	char	display[64];
	float	value = float (v) / 10000.f;
	if (fStore)
		fEffect->setParameter (fEffect, fIndex, value);
	fEffect->dispatcher (fEffect, effGetParamDisplay, fIndex, 0, display, 0.f);
	clean_string (display);
	char	complete[128];
	sprintf (complete, "%s %s", display, fUnit);
	if (strcmp (complete, fDisplay->Text ()) != 0)
		fDisplay->SetText (complete);
}

void ParameterSlider::AttachedToWindow ()
{
	BSlider::AttachedToWindow ();
	LoadParameter ();
	char	name[64];
	fEffect->dispatcher (fEffect, effGetParamName, fIndex, 0, name, 0.f);
	clean_string (name);
	SetLabel (name);
}

void ParameterSlider::LoadParameter ()
{
	fStore = false;
	SetValue (int32 (fEffect->getParameter (fEffect, fIndex) * 10000.f));
	fStore = true;
}
