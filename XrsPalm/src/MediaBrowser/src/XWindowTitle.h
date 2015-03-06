#include "View.h"

class XWindowTitle: public BView
{
	public:
		XWindowTitle(BRect r,char *txt);
	virtual void AttachedToWindow();
	virtual void MouseDown(BPoint);
	virtual void MouseUp(BPoint);
	virtual void MouseMoved(BPoint,uint32,const BMessage*); 	
	virtual void Draw(BRect);
	void SetStringPos(BPoint x);
	private:
		bool down;
		BPoint here;
		char	*text;
		BPoint	spec;
		float px,py;
};