/***************************************
/
/	SimpleJuice beta 02
/
/   Something like a DrumMachine.
/	Original idea by Andrea Anzani
/	
/	I hope to find someone that can help me in get
/	this app better!
/	Please write two lines to me.
/
/	andrea.anzani@lib.disco.unimib.it
/
*****************************************/

#ifndef DROP_VIEW_H
#define DROP_VIEW_H

#ifndef _STRING_VIEW_H
#include <StringView.h>
#endif




class DropView : public BStringView 
{
public:
				DropView(BRect frame, const char *name, const char *text,int16); 

virtual void 	MouseDown(BPoint);


int16	id;

	void Select();
	void Deselect();
	BMessage*	notify;

};

#endif 