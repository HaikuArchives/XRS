#ifndef _MY_SCROLL_
#define _MY_SCROLL_

#include "ScrollView.h"
class MyList;

class MyScroll: public BScrollView
{
	public:
		MyScroll(MyList* l);
	
	virtual void		MouseMoved(	BPoint where,uint32 code,const BMessage *a_message);
	

		MyList*	m;

};

#endif