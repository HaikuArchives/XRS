#include "XSlider.h"

XSlider::XSlider(BRect r):BView(r,"toolbar", B_FOLLOW_NONE,B_WILL_DRAW)
{
}
void
XSlider::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);
}