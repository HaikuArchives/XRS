#include "VStringView.h"
#include "Font.h"

VStringView::VStringView(BRect r,char *txt):BView(r,"VStringView",B_FOLLOW_ALL_SIDES,B_WILL_DRAW)
{
	BFont tmp;
	GetFont(&tmp);
	tmp.SetRotation(90.0);	
	tmp.SetFace(B_BOLD_FACE);
	SetFont(&tmp,B_FONT_ROTATION|B_FONT_FACE);
	text=txt;
	SetViewColor(255,203,0);
	SetLowColor(255,203,0);
	spec=BPoint(0,0);
}
void
VStringView::Draw(BRect area)
{
	if(spec==BPoint(0,0))
	DrawString(text,BPoint(Bounds().Width()-6,Bounds().Height()-4));
	else
	DrawString(text,spec);
}
void
VStringView::SetStringPos(BPoint x)
{
	spec=x;
}