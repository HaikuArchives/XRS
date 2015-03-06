
#include "Window.h"
#include "Alert.h"

#define PREFS_H 250
#define PREFS_L 400

#include "Panels.h"
#include "TabView.h"
#include "StringView.h"
#include "Box.h"
#include "Button.h"
#include "FilePanel.h"
#include "Application.h"
#include "Path.h"
#include "StatusBar.h"
#include "RadioButton.h"
#include "PatControl.h"
#include "Xed_Utils.h"
#include "GlobalDef.h"
#include "Configurator.h"
#include "JFileManager.h"
#include "GfxMsg.h"
#include "Song.h"
#include "TextView.h"

BStringView *tw;

PrefsPanel::PrefsPanel():BWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE)
{
	
	/*BTabView *b=new BTabView(BRect(0,0,PREFS_L,PREFS_H-40),"l",B_WIDTH_AS_USUAL,B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	BTab	*t=new BTab();
	
	BBox *vista=new BView(BRect(20,20,20,40),"",B_FOLLOW_ALL,B_WILL_DRAW);
	
	b->AddTab(vista,t);
	
	vista->AddChild(new BStringView(BRect(20,20,2000,40),"","/boot/home/d/",B_FOLLOW_NONE,B_WILL_DRAW));
	
	//b->AddTab(new BStringView(BRect(20,50,2000,40),"","/boot/home/d/",B_FOLLOW_NONE,B_WILL_DRAW),t);
	
	
	t->SetLabel("VST");
	
	AddChild(b);*/
	
	BBox *b=new BBox(BRect(0,0,PREFS_L,PREFS_H-40),"l",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	
	b->AddChild(new BStringView(BRect(20,20,380,40),"","VST Folder :",B_FOLLOW_NONE,B_WILL_DRAW));
	
	//BStringView *tw;
	
	b->AddChild(tw=new BStringView(BRect(40,50,300,70),"","/boot/",B_FOLLOW_NONE,B_WILL_DRAW));
	
	BEntry e;
	BPath p;
	status_t err=Ref("vstfolder",&rif,"/boot/xeD/");
	
	if(err==B_OK){
	e.SetTo(&rif);
	e.GetPath(&p);
	tw->SetText(p.Path());}
	else
		tw->SetText("/notyetset/vst/");
		
	tw->SetLowColor(255,255,255);
	
	b->AddChild(new BButton(BRect(300,50,330,70),"","set",new BMessage('Set')));	
	
	
	b->AddChild(new BStringView(BRect(20,100,380,140),"","After saving , You must restart XRS to apply this setting",B_FOLLOW_NONE,B_WILL_DRAW));
	
	AddChild(b);
	
	BBox *but=new BBox(BRect(0,PREFS_H-39,PREFS_L,PREFS_H),"",B_FOLLOW_ALL,B_WILL_DRAW);
	
	BButton *save=new BButton(BRect(PREFS_L/2,5,PREFS_L/2+PREFS_L/4,34),"","Save",new BMessage('Save'));
	BButton *cancel=new BButton(BRect(PREFS_L/2+PREFS_L/4,5,PREFS_L/2+PREFS_L/2,34),"","Cancel",new BMessage('Canc'));
	
	but->AddChild(cancel);
	but->AddChild(save);
	AddChild(but);
	MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	
}

void
PrefsPanel::MessageReceived(BMessage *message)
{
	BMessenger *m;
	BEntry e;
	BPath p;
	
	switch(message->what)
	{
		case 'Set':
			m=new BMessenger(NULL,this);
			openpanel=new BFilePanel(B_OPEN_PANEL,m,&rif,B_DIRECTORY_NODE,false,NULL,NULL,true,true);
			openpanel->Show();
		break;
		case 'Canc':
		
			this->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Save':
						
			ReplaceRef("vstfolder",&rif);
			this->PostMessage(B_QUIT_REQUESTED);
			
		break;
		case B_REFS_RECEIVED:
		
			if(message->FindRef("refs",&rif)==B_OK)
			{
				e.SetTo(&rif);
				e.GetPath(&p);
				tw->SetText(p.Path());
			}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

ExportPanel::ExportPanel(int k,int m,JFileManager *j) : BWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE)
{
	jfm=j;
	
	BBox *b=new BBox(BRect(0,0,PREFS_L,PREFS_H-40),"l",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	
	b->AddChild(new BStringView(BRect(20,20,380,40),"","Export Mode :",B_FOLLOW_NONE,B_WILL_DRAW));
	
	b->AddChild(single=new BRadioButton(BRect(40,50,200,70),"w","Single Measure",NULL));
	
	single->SetValue(true);
	
	b->AddChild(new BRadioButton(BRect(40,80,300,110),"!","All the Song",NULL));
	
	BView *tv=new BView(BRect(0,0,10,10),"_temp",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	AddChild(tv);
	
	mea=new PatControl(BRect(200,50,239,67),
				::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),1,m,k,NULL,::MakeBitmapSkin("PatBase"));
	b->AddChild(mea);
	
	//b->AddChild(new BStatusBar(BRect(40,50,300,70),"Export Status:",NULL));
	
	
	b->AddChild(new BStringView(BRect(20,100,380,140),"","-",B_FOLLOW_NONE,B_WILL_DRAW));
	
	AddChild(b);
	
	BBox *but=new BBox(BRect(0,PREFS_H-39,PREFS_L,PREFS_H),"",B_FOLLOW_ALL,B_WILL_DRAW);
	
	BButton *save=new BButton(BRect(PREFS_L/2,5,PREFS_L/2+PREFS_L/4,34),"","Export",new BMessage('Expt'));
	BButton *cancel=new BButton(BRect(PREFS_L/2+PREFS_L/4,5,PREFS_L/2+PREFS_L/2,34),"","Cancel",new BMessage('Canc'));
	
	but->AddChild(cancel);
	but->AddChild(save);
	AddChild(but);
	MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
}

void
ExportPanel::MessageReceived(BMessage *message)
{
	BMessenger *m;
	BEntry e;
	BPath p;
	
	switch(message->what)
	{
		case 'Canc':
		
			this->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Expt':
			jfm->ExportWave(!single->Value(),mea->getValue()-1);
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	}
}

SongSettings::SongSettings():BWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE)
{
	
	
		
	BBox *b=new BBox(BRect(0,0,PREFS_L,PREFS_H-40),"l",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	
	b->AddChild(new BStringView(BRect(20,20,200,40),"","Song description:",B_FOLLOW_NONE,B_WILL_DRAW));
	b->AddChild(cb=new BCheckBox(BRect(230,20,340,40),"","PopUp on load",NULL));
	//BStringView *tw;
	
	b->AddChild(tw=new BTextView(BRect(40,50,340,140),"",BRect(0,0,260,90),B_FOLLOW_NONE,B_WILL_DRAW));
	
	BView *tv=new BView(BRect(0,0,10,10),"_temp",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	AddChild(tv);
	
	b->AddChild(new BStringView(BRect(20,155,229,180),"","Number of notes in each measure :",B_FOLLOW_NONE,B_WILL_DRAW));
	
	
	mea=new PatControl(BRect(230,160,269,177),
				::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),4,32,16,NULL,::MakeBitmapSkin("PatBase"));
	b->AddChild(mea);
	
	// first beat each:
	
	b->AddChild(new BStringView(BRect(20,175,229,200),"","first beat each :",B_FOLLOW_NONE,B_WILL_DRAW));
	
	
	beat=new PatControl(BRect(230,180,269,197),
				::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),2,8,4,NULL,::MakeBitmapSkin("PatBase"));
	b->AddChild(beat);
	
	
		
	tw->SetLowColor(255,255,255);
	
	tw->SetStylable(true);
		
	//b->AddChild(new BStringView(BRect(20,100,380,140),"","After saving , You must restart XRS to apply this setting",B_FOLLOW_NONE,B_WILL_DRAW));
	
	AddChild(b);
	
	BBox *but=new BBox(BRect(0,PREFS_H-39,PREFS_L,PREFS_H),"",B_FOLLOW_ALL,B_WILL_DRAW);
	
	BButton *save=new BButton(BRect(PREFS_L/2,5,PREFS_L/2+PREFS_L/4,34),"","Ok",new BMessage('Ok'));
	BButton *cancel=new BButton(BRect(PREFS_L/2+PREFS_L/4,5,PREFS_L/2+PREFS_L/2,34),"","Cancel",new BMessage('Canc'));
	
	but->AddChild(cancel);
	but->AddChild(save);
	AddChild(but);
	MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	
}

void
SongSettings::MessageReceived(BMessage *message)
{
	
	
	switch(message->what)
	{
		
		case 'Canc':
		
			this->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Ok':
						
			curSong->setDescription(tw->Text());
			curSong->setNumberNotes(mea->getValue());
			curSong->popupdesc=(bool)cb->Value();
			be_app->PostMessage(new BMessage(SONG_RESET));
			this->PostMessage(B_QUIT_REQUESTED);
			
		break;
		
		default:
			BWindow::MessageReceived(message);
		break;
	}
}
void
SongSettings::Init(Song* s)
{
	curSong=s;
	tw->SetText(curSong->getDescription());
	cb->SetValue(curSong->popupdesc);
	mea->setValue(curSong->getNumberNotes());
	
}