/*

	VSTCopyrightWindow.cpp

	by Georges-Edouard Berenger
	© 2000, Steinberg Soft- und Hard GmbH, All Rights Reserved.

*/

#ifndef _VSTCOPYRIGHT_WINDOW_H_
#include "VSTCopyrightWindow.h"
#endif

#include "vstlogo.h"

#include <Button.h>
#include <Roster.h>
#include <AppFileInfo.h>
#include <Application.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Screen.h>
#include <Bitmap.h>

const char* kAboutWeb = "http://www.steinberg.net";

class URLView : public BView {

	public:
					URLView (BRect frame, const char* url);
					~URLView ();
	virtual	void	AttachedToWindow ();
	virtual	void	Draw (BRect updateRect);
	virtual	void	MouseDown (BPoint where);
	virtual void	OpenURL ();

	protected:
		char*		fUrl;
		BPoint		fLocation;
};

URLView::URLView(BRect frame, const char* url)
	: BView(frame, NULL, B_FOLLOW_NONE, B_WILL_DRAW)
{
	fUrl = strdup (url);
}

URLView::~URLView()
{
	free (fUrl);
}

void URLView::AttachedToWindow()
{
	rgb_color back = ui_color(B_PANEL_BACKGROUND_COLOR);
	SetViewColor (back);
	SetLowColor (back);
	SetHighColor (49, 61, 225);
	SetFont (be_plain_font);
	font_height height;
	GetFontHeight (&height);
	float width = StringWidth (fUrl);
	float r = width + 8;
	BRect frame = Frame ();
	BRect nframe = frame;
	nframe.right = nframe.left + r - 4;
	nframe.bottom = frame.top + height.ascent + height.descent + 2;
	ResizeTo (nframe.Width (), nframe.Height ());
	fLocation.x = ceil ((nframe.Width () - width) / 2) + 1;
	fLocation.y = floor ((nframe.Height () + height.ascent - height.descent) / 2);
}

void URLView::Draw (BRect /*updateRect*/)
{
	DrawString (fUrl, fLocation);
}

void URLView::MouseDown (BPoint /*where*/)
{
	BPoint	pos;
	uint32	buttons;
	BRect	bounds = Bounds ();
	bool	inside = true;
	InvertRect (bounds);
	do
	{
		snooze (5000);
		GetMouse (&pos, &buttons);
		bool now = bounds.Contains (pos);
		if (now != inside)
		{
			InvertRect (bounds);
			Flush ();
			inside = now;
		}
	} while (buttons != 0);
	if (inside)
	{
		OpenURL ();
		InvertRect (bounds);
	}
}

void URLView::OpenURL ()
{
	char*	argv[2];
	argv[0] = fUrl;
	argv[1] = 0;
	be_roster->Launch ("text/html", 1, argv);
}

class AboutView : public BView {

	public:
					AboutView(BRect frame, const char* name);
					~AboutView();
	virtual	void	AttachedToWindow();
	virtual	void	Draw(BRect updateRect);

	private:
		char*		fName;
		BFont		fFont;
		BBitmap*	fVSTlogo;
		rgb_color	fPanelColor;
		rgb_color	fDarkPanelColor;
};

VSTCopyrightWindow::VSTCopyrightWindow (const char* name) :BWindow(BRect (0, 0, 550, 200), B_EMPTY_STRING,
					B_MODAL_WINDOW, B_NOT_RESIZABLE+B_NOT_ZOOMABLE+B_NOT_MINIMIZABLE+B_NOT_CLOSABLE)
{
	BRect		rect;
	AboutView	*topview;

	// View de fond
	rect=Bounds();
	AddChild(topview = new AboutView(rect, name));
	BButton	*ok;
	topview->AddChild(ok = new BButton(BRect(rect.right-125, rect.bottom-46, rect.right-25, rect.bottom-22), NULL,
		"Thanks!", new BMessage(B_QUIT_REQUESTED), B_FOLLOW_LEFT));
	ok->MakeDefault(true);
	BRect	screen = BScreen ().Frame ();
	rect.left = (screen.right + screen.left - rect.right - rect.left) / 2;
	rect.top = (screen.top + screen.bottom - rect.top - rect.bottom) / 2;
	MoveTo (rect.left, rect.top);
	Show();
}

#define kTop 163
#define kStep 12
#define kMarge 20

AboutView::AboutView(BRect frame, const char * name) : BView(frame, NULL, B_FOLLOW_NONE, B_WILL_DRAW)
{
	fName = strdup (name);
	fVSTlogo = getVSTlogo ();
	float left = kMarge * 2 + fVSTlogo->Bounds ().Width ();
	URLView* urlview = new URLView (BRect (left - 2, kTop - 3 * kStep + 2, left - 2, 0), kAboutWeb);
	AddChild (urlview);
}

AboutView::~AboutView()
{
	if (fVSTlogo)
		delete fVSTlogo;
	free (fName);
}

void AboutView::AttachedToWindow()
{
	fPanelColor=ui_color(B_PANEL_BACKGROUND_COLOR);
	fDarkPanelColor=tint_color(fPanelColor, B_DARKEN_1_TINT);
	SetViewColor(fPanelColor);
	SetLowColor(fPanelColor);
}

void AboutView::Draw(BRect /*updateRect*/)
{
	BRect	r=Bounds();
	SetDrawingMode(B_OP_OVER);
	DrawBitmap(fVSTlogo, BPoint(kMarge, 105));
	SetDrawingMode(B_OP_COPY);
	r.bottom=80;
	SetHighColor(fDarkPanelColor);
	FillRect(r);
	SetHighColor (80, 0, 0);
	SetLowColor(fDarkPanelColor);
	if (fFont.SetFamilyAndFace ("Baskerville", B_BOLD_FACE | B_ITALIC_FACE) != B_OK)
		fFont.SetFamilyAndFace ("Baskerville BT", B_BOLD_FACE | B_ITALIC_FACE);
	fFont.SetSize(45);
	SetFont(&fFont, B_FONT_FAMILY_AND_STYLE | B_FONT_SIZE);
	DrawString(fName, BPoint((r.Width()-StringWidth(fName))/2, 50));
	fFont.SetFamilyAndFace ("Swis721 BT", B_REGULAR_FACE);
	fFont.SetSize(10);
	SetFont(&fFont, B_FONT_FAMILY_AND_STYLE | B_FONT_SIZE);
	SetHighColor(0, 0, 0);
	SetLowColor(fPanelColor);
	float left = kMarge * 2 + fVSTlogo->Bounds ().Width ();
	MovePenTo(left, kTop - 4 * kStep);
	DrawString("VST Plugin Technology by Steinberg.");
	MovePenTo(left, kTop - 3 * kStep);
	DrawString("© Steinberg Soft- und Hardware GmbH, 2000.");
	MovePenTo(left, kTop);
	DrawString("Written by Georges-Edouard Berenger.");
	MovePenTo(left, kTop + kStep);
	DrawString("Hamburg, ");
	DrawString(__DATE__);
	DrawString(", ");
	DrawString(__TIME__);
	DrawString(".");
}
