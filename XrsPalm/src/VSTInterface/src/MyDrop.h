#ifndef _MY_DROP_
#define _MY_DROP_

#include "InterfaceKit.h"

class MyDrop: public BView{

public:
		MyDrop(BRect);
	
	virtual void Draw(BRect);
	//virtual void MouseDown(BPoint);
	virtual void MessageReceived(BMessage*);
	virtual	void			MouseMoved(	BPoint where,
									uint32 code,
									const BMessage *a_message);
	
	int k;

};

#endif