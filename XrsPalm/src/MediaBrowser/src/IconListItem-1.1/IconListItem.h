//------------------------------------------------------------------------------
// IconListItem.h
//------------------------------------------------------------------------------
// A ListItem implementation that displays an icon and its label.
//
// IconListItem implementation Copyright (C) 1999 Fabien Fulhaber <fulhaber@evhr.net>
// Special thanks to Brendan Allen <darkmoon96@tcon.net> for his help.
// Thanks to NPC community (http://www.beroute.tzo.com/npc/).
// This code is free to use in any way so long as the credits above remain intact.
// This code carries no warranties or guarantees of any kind. Use at your own risk.
//------------------------------------------------------------------------------

#ifndef _ICON_LIST_ITEM_H_
#define _ICON_LIST_ITEM_H_

//------------------------------------------------------------------------------
// I N C L U D E S
//------------------------------------------------------------------------------
 
#include <ListItem.h>
#include <Bitmap.h> 
#include <Rect.h>
#include <Font.h>
#include <String.h>

//------------------------------------------------------------------------------
// D E C L A R A T I O N S
//------------------------------------------------------------------------------

class IconListItem : public BListItem
{ 
	public:
		IconListItem(BBitmap *mini_icon, char *text, int level, bool expanded); 

		virtual ~IconListItem(); 
		virtual void DrawItem(BView *owner, BRect frame, bool complete = false); 
		virtual void Update(BView *owner, const BFont *font);
	private:
		BRect bounds; 
		BBitmap *icon;
		BString label;
};

#endif
//------------------------------------------------------------ IconListMenu.h --