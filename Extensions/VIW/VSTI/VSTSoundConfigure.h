/*

	VSTSoundConfigure.h

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTSOUND_CONFIGURE_H_
#define _VSTSOUND_CONFIGURE_H_

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif

#include <Slider.h>
#include <String.h>

class BTextControl;
class BStringView;
class BMenuField;
class VSTInstrumentPlugin;

class VSTConfigureView : public BView {
public:
					VSTConfigureView (VSTInstrumentPlugin * plugin);
	virtual	void 	AttachedToWindow ();
	virtual	void 	DetachedFromWindow ();
	virtual	void	MessageReceived (BMessage *msg);
	virtual	void	WindowActivated (bool state);
	BMenuField	*	fMidiSelect;
	VSTInstrumentPlugin *	fPlugin;
	BMessageRunner *	fMessageRunner;
	AEffect *		fEffect;
		
};

class ParameterSlider : public BSlider {
public:
					ParameterSlider (BRect frame, AEffect * effect, int index);
	void		 	AttachedToWindow ();
	void			SetValue (int32 v);
	void			LoadParameter ();

private:
	BStringView *	fDisplay;
	AEffect *		fEffect;
	int				fIndex;
	char			fUnit[64];
	bool			fStore;
};

#endif // _VSTSOUND_CONFIGURE_H_