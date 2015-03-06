#ifndef _X_MATRIX_
#define _X_MATRIX_

#include "View.h"
class Sequence;

class XMatrix: public BView
{
	public:
			XMatrix(BRect,BView*);
			
		virtual void 	Draw(BRect);
		virtual void 	MouseDown(BPoint);
		virtual void	AttachedToWindow();	
			
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
		
			
			
			Sequence*	sequence;
			
			int curPat;
			int sel;
			
			int  MaxPat;
			bool	key_rel;
			BView	*the_n;
			int y_count;
};			

#endif