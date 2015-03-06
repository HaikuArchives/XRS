//--------------------------------------------------------------------
//	
//	BitmapMenuItem.cpp
//
//	Written by: Owen Smith
//	
//--------------------------------------------------------------------

/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include "BitmapMenuItem.h"
#include "constants.h"
#include "Xed_Utils.h"
//====================================================================
//	BitmapMenuItem Implementation



//--------------------------------------------------------------------
//	BitmapMenuItem constructors, destructors, operators

BitmapMenuItem::BitmapMenuItem(const char* name, const char *bitmap,
	BMessage* message, char shortcut, uint32 modifiers)
	: BMenuItem(name, message, shortcut, modifiers)
{
		
	m_bitmap=::MakeBitmapSkin("PianoViewMenu");
}



//--------------------------------------------------------------------
//	BitmapMenuItem constructors, destructors, operators

void BitmapMenuItem::Draw(void)
{
	
	
	BMenu* menu = Menu();
	if (menu) {
		BRect itemFrame = Frame();
		BRect bitmapFrame = itemFrame;
		bitmapFrame.right=bitmapFrame.left+m_bitmap->Bounds().Width(); // account for 2-pixel margin
		bitmapFrame.bottom=bitmapFrame.top+m_bitmap->Bounds().Height();
		menu->SetDrawingMode(B_OP_COPY);
		//menu->SetHighColor(BKG_GREY);
		//menu->FillRect(itemFrame);
		
		menu->DrawBitmap(m_bitmap, bitmapFrame);
		menu->SetHighColor(0,0,0);
		menu->DrawString(" PianoView",BPoint(m_bitmap->Bounds().right+4,bitmapFrame.top+10));
		
		if (IsSelected()) {
			// a nonstandard but simple way to draw highlights
			menu->SetDrawingMode(B_OP_INVERT);
			menu->SetHighColor(0,0,0);
			menu->FillRect(itemFrame);
		}
	}
}

void BitmapMenuItem::GetContentSize(float* width, float* height)
{
	GetBitmapSize(width, height);
}



//--------------------------------------------------------------------
//	BitmapMenuItem accessors

void BitmapMenuItem::GetBitmapSize(float* width, float* height)
{
	BRect r = m_bitmap->Bounds();
	*width = r.Width() + 40; // 2-pixel boundary on either side
	*height = r.Height() + 10; // 2-pixel boundary on top/bottom
}
