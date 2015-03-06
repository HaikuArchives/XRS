#ifndef _X_MPOZ_
#define _X_MPOZ_

#include "View.h"
class Sequence;

class XMPoz: public BView
{
	public:
			XMPoz(BRect);
			
		virtual void 	Draw(BRect);
		
		virtual void	AttachedToWindow();	
			
			 int	nextPattern(int);
			 int	getPattern();
			
			
			
			 
			 void	Reset(Sequence*);
			
			 int	getMaxPat();
			
			 void	setMaxs(int,int);		
				
		
			void	setPosition(int);
			
			Sequence*	sequence;
			int curPat;
			
						
		
};			

#endif