#ifndef _X_MNAME_
#define _X_MNAME_

#include "View.h"
class Sequence;
//class BList;

class XMName: public BView
{
	public:
			XMName(BRect);
			
		virtual void 	Draw(BRect);
		virtual void 	MouseDown(BPoint);
		virtual void	AttachedToWindow();	
		//virtual void	MessageReceived(BMessage* msg);
			
			 int	nextPattern(int);
			 int	getPattern();
			 void	step();
			 int	getStep();
			 void	Select(int);
			 
			 void	Reset(Sequence*);
			 void	AddItem(int,int);
			 int	getMaxPat();
			
			 void	setPosition(int);
	
	private:
			
			void	setMaxs(int,int);
		
			
			//BList*		names;
			Sequence*	sequence;
			
			int curPat;
			int sel;
			int y_count;
			int  MaxPat;
			bool	key_rel;
			
		
};			

#endif