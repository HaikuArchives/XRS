#ifndef _MY_SOURCE_
#define _MY_SOURCE_

#include "ListView.h"

class MySource: public BListView
{
	public:
			MySource(BRect);
	void 		Init();
	virtual bool	InitiateDrag(BPoint,int32,bool);
	float	h;
	float	w;

};

#endif