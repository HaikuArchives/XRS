#ifndef _X_MATRIX_
#define _X_MATRIX_

#include "View.h"

class Sequence;
class XMPoz;


class XMatrix: public BView
{
	public:
			XMatrix(BRect,BView*,XMPoz*);
			
		virtual void 	Draw(BRect);
		virtual void 	MouseDown(BPoint);
		virtual void	AttachedToWindow();	
		virtual void	ScrollTo(BPoint);	
			 int	nextPattern(int);
			 int	getPattern();
			 void	step();
			 int	getStep();
			 void	Select(int);
			 
			 void	Reset(Sequence*);
			 void	AddItem(int,int);
			 int	getMaxPat();
			
			 void	setPosition(int);
			 void	setMaxs(int,int);
	private:
			
			
		
			
			
			Sequence*	sequence;
			
			int curPat;
			int sel;
			int y_pos;
			int x_pos;
			int y_count;
			BView*		the_n;
			XMPoz*	the_s;
			
			int  MaxPat;
			bool	key_rel;
			
		
};			

#endif