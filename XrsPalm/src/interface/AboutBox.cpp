#include "AboutBox.h"
#include "Alert.h"
#include "Box.h"
#include "Button.h"
#include "Window.h"
#include "WU_AppUtils.h"
#define PREFS_H 204
#define PREFS_L 400

#include "URLView.h"

AboutBox::AboutBox(bool mode):BWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE)
{
	//BView *w=new BView(BRect(0,0,192,204),"",B_FOLLOW_NONE,B_WILL_DRAW);
	BView *w=new BView(Bounds(),"",B_FOLLOW_NONE,B_WILL_DRAW);
	AddChild(w);
	//w->SetViewBitmap(GetAppBitmap(100));
	w->SetViewBitmap(::GetAppJpeg(102) );
	
	BRect r(Bounds());
	
	r.top=r.bottom+1;
	
	int k=20;
	if(mode) k=100;
		
	r.bottom+=k;
	ResizeBy(-1,k-1);
	
	BBox *but=new BBox(r,"",B_FOLLOW_ALL,B_WILL_DRAW);
	
	
	if(mode){
		BButton *ok=new BButton(BRect(130,60,200,94),"","Ok!",new BMessage(B_QUIT_REQUESTED));
		ok->SetFontSize(12);
		but->AddChild(ok);
		text=NULL;
	
	
	
	URLView *email = new URLView( BRect( 20, 45, 200, 60 ), "Author ", "Anzani Andrea <oxygenum@tiscali.it>", "mailto:oxygenum@tiscali.it" );
	email->SetHoverEnabled( true );
	email->AddAttribute( "META:name", "Anzani Andrea" );
	email->AddAttribute( "META:nickname", "xeD" );
	email->AddAttribute( "META:company", ":-)" );
	email->AddAttribute( "META:url", "" );
	email->AddAttribute( "META:state", "MI" );
	email->AddAttribute( "META:country", "Italy" );
	
	
	but->AddChild( new BStringView(BRect(10,30,200,45),"","Written by : ")); 
	but->AddChild( email );
	but->AddChild(new BStringView(BRect(10,80,200,95),"","Home Page : "));
	but->AddChild(new URLView(BRect( 20,95,200,110),"","utenti.tripod.it/Freaxed/XRS/","http://utenti.tripod.it/Freaxed/XRS/"));
	}
	 else
	{
		but->AddChild( text=new BStringView(BRect(0,0,r.Width(),20),"","Loading..")); 
	}
	AddChild(but);
	
	MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
}
void
AboutBox::setText(const char*t){
if(text!=NULL)	if(Lock())
			{
			 	text->SetText(t); 
			 	Unlock();
			 }
}

