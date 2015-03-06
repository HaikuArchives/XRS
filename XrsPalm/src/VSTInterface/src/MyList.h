#ifndef _MY_LIST_
#define _MY_LIST_

#include "ListView.h"

class MyList: public BListView
{
	public:
		MyList(BRect);
	void 	Init(float hx);
	virtual void		MouseMoved(	BPoint where,uint32 code,const BMessage *a_message);
	virtual void		MouseDown(BPoint);
	virtual void Draw(BRect);
	virtual void MessageReceived(BMessage*);
	virtual bool InitiateDrag(BPoint,int32,bool);
		
		float	h;
		float	w;
		float y;
		float yn;
		 char mea[20];
};

#endif