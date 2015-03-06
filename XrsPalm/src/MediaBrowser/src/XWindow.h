
#ifndef X_WINDOW_H
#define X_WINDOW_H
 
#include <Window.h>
#include "XWindowTitle.h"

class XWindow:public BWindow
{



 public:
					XWindow(BRect r,const char *txt);
			virtual bool QuitRequested();	
			virtual void MessageReceived(BMessage*);
			
private:
			XWindowTitle *title;
		
		
};

#endif