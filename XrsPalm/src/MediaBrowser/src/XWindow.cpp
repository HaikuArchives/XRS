
#include "XWindow.h"
#include "Application.h"
#define	X_WINDOW_ZOOM	'xwzc'


//XWindowTitle *title;

XWindow::XWindow(BRect r,const char *txt) :
	BWindow(r,txt, B_BORDERED_WINDOW,B_WILL_ACCEPT_FIRST_CLICK)
{
	
	ResizeBy(21,0);
	MoveBy(-21,0);
	AddChild(title=new XWindowTitle(BRect(0,0,20,Bounds().Height()),(char*)txt));
	
}

bool
XWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
void
XWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case X_WINDOW_ZOOM:
			if(Bounds().Width()==19)
			ResizeTo(680-450,Bounds().Height());
			else
			ResizeTo(19,Bounds().Height());
		break;
		default:
		BWindow::MessageReceived(message);
	}
	
}
