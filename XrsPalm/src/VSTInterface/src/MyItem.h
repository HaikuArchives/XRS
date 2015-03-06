#ifndef _MY_ITEM_
#define _MY_ITEM_

#include "InterfaceKit.h"

class MyItem: public BStringItem{

public:
		MyItem(const char*);
		void	ChangeStatus();
		bool	GetStatus(){ return active;};
		void	SetStatus(bool b){ active=b;}
	
	virtual	void		DrawItem(BView *owner,
							BRect bounds,
							bool complete = false) ;
	
	bool active;
	
};

#endif