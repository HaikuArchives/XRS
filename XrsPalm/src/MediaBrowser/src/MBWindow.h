
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
 
#include <Window.h>

class BListItem;

class MBWindow:public BWindow
{



 public:
					MBWindow();
			virtual bool QuitRequested();	
			virtual void MessageReceived(BMessage*);
			void	AddFolder(entry_ref ref);
			
private:
			void ParseDir(entry_ref*,BListItem*);
			void ParseBank(entry_ref *ref,BListItem* folder);
			int64 ReadFixed(int64 *data,BFile* file);
			
					
		
};

#endif