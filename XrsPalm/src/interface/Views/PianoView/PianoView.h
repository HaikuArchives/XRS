// PianoView.h
// * PURPOSE
//   Display an editable text field in a simple pop-up window
//   (which must automatically close when the user hits 'enter'
//   or the window loses focus).
// * TO DO +++++
//   escape key -> cancel
//
// * HISTORY
//   e.moon		23aug99		Begun

#ifndef __PianoView_H__
#define __PianoView_H__

#include <Messenger.h>
#include <Window.h>

class BView;
class Pattern;
//#include "cortex_defs.h"



class PianoView :
	public	BWindow {
	typedef	BWindow _inherited;
	
public:												// dtor/ctors
	//virtual ~PianoView();

	PianoView(
		BRect							frame,
		const BMessenger&					target,
		BView*						keypass
		);

public:												// BWindow
	virtual void WindowActivated(bool 	activated);
	virtual bool QuitRequested();
	
		 void	Reset(Pattern* p,bool extra);	
public:												// BHandler
	virtual void MessageReceived(BMessage*	message);
		
private:
	BView*				m_control;
	BView*				extra_control;
	BStringView*				m_text;
	BMessenger				m_target;	
	//const BMessage*			m_message;
	BView*				m_keypass;
	BScrollBar*				sb;
	
	bool					m_sentUpdate;
};


#endif /*__PianoView_H__*/