/*

	VSTSoundConfigure.cpp

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTSOUND_CONFIGURE_H_
#include "VSTSoundConfigure.h"
#endif


#ifndef _VSTFILTER_H_
#include "VSTInstrument.h"
#endif

#include <Window.h>
#include <Messenger.h>
#include <MessageRunner.h>
#include <StringView.h>
#include <TextControl.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <MenuBar.h>
#include <MidiRoster.h>
#include <MidiProducer.h>

#include "PlugWindow.h"

#include "stdio.h"

#define	X_WIN_ZOOM		'xwzo'

const float kConfigHeight = 5;



VSTConfigureView::VSTConfigureView (VSTInstrumentPlugin * plugin) :
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
		{
			window->SetFlags (window->Flags () | B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_NOT_RESIZABLE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS);
			window->SetType(B_FLOATING_WINDOW);
		}
	
	BMessenger messenger (this);
	fMessageRunner = new BMessageRunner (messenger, new BMessage ('Idle'), 10000, -1);
	SetViewColor (Parent ()->ViewColor ());
	
	char	program[32];
	
	fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, program, 0);
	
	
	
	BMenu* popup = new BMenu("Program");
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
	
	popup->SetRadioMode(true);
	
	((PlugWindow*)window)->SetPrograms(fPlugin->name.String (),popup);
	
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
	//else if (msg->what == 'Name')
	//	fEffect->dispatcher (fEffect, effSetProgramName, 0, 0, (char*) fNameControl->Text (), 0);
	else if (msg->what == 'Prog')
	{
		int32	index;
		if (msg->FindInt32 ("program", &index) == B_OK)
		{
			fEffect->dispatcher (fEffect, effSetProgram, 0, index, 0, 0.f);
			char	name[32];
			fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, name, 0);
			//fNameControl->SetText (name);
			index = 0;
			BView * child;
			while ((child = ChildAt (index++)) != 0)
			{
				ParameterSlider * slider = dynamic_cast<ParameterSlider*> (child);
				if (slider)
					{
						//mda JX10 Wrap!
						if(index!=4)
						slider->LoadParameter ();
						else
						slider->SetValue(0);
					}
			}
			
			fPlugin->StopAll();
		}
	}
	else if (msg->what == 'Midi')
	{
		/*int32	id;
		if (msg->FindInt32 ("id", &id) == B_OK)
			fPlugin->SetMidiProducer (id);*/
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
const float 	range = 10;

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
