
#ifndef MATRIX_WINDOW_H
#define MATRIX_WINDOW_H
 
#include "Window.h"
#include "GlobalDef.h"

class DropView;
class XMatrix;
class XMName;

class Sequence;

class MatrixWindow:public BWindow
{



	 public:
					MatrixWindow(BWindow*);
					
			virtual void MessageReceived(BMessage*);
			virtual bool QuitRequested();
			virtual void FrameResized(float,float);
				int	nextPattern(int d);
				void	step();
				int	getStep();
				int	getPattern();
				int	getSelectedPattern();
				void	setSelectedPattern(int);
				void	Reset(Sequence*);
				void	setPosition(int);
				
				XMatrix*	getMatrix();
				
	private:
	
				XMatrix 	*the_m;
				XMName	*the_n;
				BScrollView *scroll;
				BScrollBar	*scroll_bar;
				int sel;		
				BWindow	*	target;
				//BView		*	targetKey;
				BMessage	*the_msg;
				Sequence*	seq;
				int y_count;
		
};

#endif