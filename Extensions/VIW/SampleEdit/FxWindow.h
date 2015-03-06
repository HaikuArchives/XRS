
#ifndef S_WINDOW_H
#define S_WINDOW_H
 
#include <Window.h>



class StringView;

class Sample;

class FxWindow:public BWindow
{



 public:
				FxWindow();
	virtual void		MessageReceived(BMessage* message);
	virtual bool		QuitRequested();	
	void			SetJSample(Sample *samp);
	void			ResetJSample();
	void			AddFx();
private:

	
	Sample				*my_sample;
	
	char txt[30];
	BStringView * text_info;
	
};

#endif