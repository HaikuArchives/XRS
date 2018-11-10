
#ifndef PLUG_WINDOW_H
#define PLUG_WINDOW_H
 
#include "Window.h"

class BMenu;
class BMenuItem;



class PlugWindow:public BWindow
{



	 public:
					PlugWindow(const char*name);
			virtual bool 	QuitRequested();
			void		SetControls(BView*,bool);
			void		SetPrograms(const char *name,BMenu*);
			void		SetState(bool,float force);
			bool		GetState() { return big;}
			
			void		SavePref(BMessage*);
			void		LoadPref(BMessage*);		
			virtual void	MessageReceived(BMessage*);
				
	private:
			BMenuBar *bar;
			BMenuItem* nameMenu;
			bool	big;
			float	smallW,smallH;
			float	bigW,bigH;
};

#endif
