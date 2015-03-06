#ifndef _MY_DRAG_
#define _MY_DRAG_

#include "InterfaceKit.h"

class MyDrag: public BView{

public:
		MyDrag(BRect);
	
	virtual void Draw(BRect);
	virtual void MouseDown(BPoint);
//	virtual void MessageReceived(BMessage);
	


};

#endif