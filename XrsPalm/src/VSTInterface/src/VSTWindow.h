
#ifndef VST_WINDOW_H
#define VST_WINDOW_H
 
#include "PlugWindow.h"

class Juice;
class VSTFilterPlugin;
class MyList;
class MySource;

class VSTWindow:public PlugWindow
{

 public:
					VSTWindow();
					~VSTWindow();
				void	Init(Juice* s);
				void	Reset() {};
				bool	AddVst(BMessage*,int,int,bool g=false);
				int	CountVst(int);
				void 	getVstName(const char*,int,int);
				void	AddVstWindowInfo(BMessage *g,int,int);
				virtual void MessageReceived(BMessage *);
				void	CloseVst();
protected:
friend MyList;
				void	SetVstState(int who,bool);
				void	MoveVst(int from,int to);			
				void	AddVst(const char* n,int line,int);
				void	RemoveVst(int line,int pos);
				void	vstWindow(VSTFilterPlugin *vst,const char* name,int line,int,BMessage* msg=NULL);
private:		
			Juice*		succo;	
			BList*		win_vst[3];
			MyList*	list[3];
			MySource*	s;
};

#endif