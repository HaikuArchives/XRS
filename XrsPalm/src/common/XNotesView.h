
#ifndef _X_NOTESVIEWL_
#define  _X_NOTESVIEWL_

#include "Control.h"
#include "GlobalDef.h"

#include "Pattern.h"

//#include "PianoView.h"
class		PianoView;
static		PianoView	*pw;

class XNotesView : public BControl
{
	public:
	
					XNotesView (BRect,BView*,int16);
					~XNotesView();
	virtual	void		Draw(BRect);
	virtual	void		MouseDown(BPoint);
	virtual	void		MouseUp(BPoint);
	virtual 	void 		MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
			void		Reset(Pattern*,bool);
			status_t	Init();
			void		setType(int);
			int		getType();
			void		ClosePiano();
			void		ShowPiano(bool);
			
	private:

		Pattern*	curPattern;
		BBitmap	*picOn;
		BBitmap	*picOff;
		BBitmap	*picOnB;
		BBitmap	*picOffB;
		int		view_type;
		int		prev;
		bool		set_state;
		int		selected;
};
#endif

