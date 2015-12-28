
#include "MainWindow.h"
#include "View.h"
#include "Bitmap.h"
#include "Xed_Utils.h"

#include "InterfaceKit.h"

#include "PictureButton.h"
#include "stdio.h"
#include "TranslationUtils.h"

#include "XPanel.h"
#include "Juice.h"

#include"GraphicsDef.h"
#include "GfxMsg.h"

#include "Application.h"

#include "Path.h"
#include "Entry.h"
#include "Directory.h"

//#include "BankManager.h"
//#include "ExternalManager.h"
#include "JFileManager.h"
#include "Configurator.h"

#include "Panels.h"
#include "TracksPanel.h"
#include "MatrixWindow.h"

#include "TickView.h"

BStringView*	info_txt;
BBox*	info_box;

/* the magic array ! */

char control_array[5][10];
char control_out[20];

#include "TrackManager.h"
#include "StereoMeter.h"
/* DIRECT CD */

//#define	TMP_MSG 'tmm'

StereoMeter* One;

MainWindow::MainWindow() :
	BWindow(BRect(WINDOW_X_POS,WINDOW_Y_POS,WINDOW_X_POS+WINDOW_XL,WINDOW_Y_POS+WINDOW_YL),"Main", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_WILL_ACCEPT_FIRST_CLICK)

{
	::CheckMimeType();
	/* MAGIC ARRAY DA RIPOSIZIONARE! */

	strcpy(control_array[0],"Volume");
	strcpy(control_array[1],"Pan");
	strcpy(control_array[2],"Cutoff");
	strcpy(control_array[3],"Resonance");

	/* */

	BMenu		*menuEdit;
	BMenu		*menuNew;
	BMenu		*edit_track,*edit_mea;
	BMenuItem		*edit_prefs;

	BMenuItem	*quit_item;

	BRect dummy(0, 0, 0, 0);
	menuBar = new BMenuBar(dummy, "Full Menu Bar");


	menuBar->AddItem(menuFile=new BMenu("File"));
	menuBar->AddItem(menuEdit=new BMenu("Edit"));

	//menuBar->AddItem(new BMenu("Extensions"));



	menuFile->AddItem(menuNew=new BMenu("New"));
	menuNew->AddItem(new BMenuItem("Default",new BMessage(MENU_NEW_DEFAULT)));
	menuNew->AddItem(new BMenuItem("Empty",new BMessage(MENU_NEW_EMPTY)));
	menuFile->AddItem(new BMenuItem("Open",new BMessage(MENU_OPEN)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(save_item=new BMenuItem("Save",new BMessage(MENU_SAVE)));
	menuFile->AddItem(saveas_item=new BMenuItem("Save As..",new BMessage(MENU_SAVEAS)));
	menuFile->AddItem(new BMenuItem("Export As WAVE",new BMessage(MENU_EXPORT)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(new BMenuItem("About",new BMessage(B_ABOUT_REQUESTED)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(quit_item=new BMenuItem("Quit",new BMessage(B_QUIT_REQUESTED)));	//Un po' brutale..



	menuFile->SetTargetForItems(be_app);
	menuNew->SetTargetForItems(be_app);
	quit_item->SetTarget(this);
	float menuY, menuX;

	//BMenu *add_menu;

	menuEdit->AddItem(edit_track=new BMenu("Track"));
	menuEdit->AddItem(edit_mea=new BMenu("Measure"));

	menuEdit->AddItem(new BMenuItem("Playlist",new BMessage(EDIT_MSG),'P'));
	menuEdit->AddSeparatorItem();
	//menuEdit->AddItem(new BMenuItem("Unload External Samples",new BMessage(MENU_UNLOAD)));
	//
	menuEdit->AddItem(new BMenuItem("Song Settings..",new BMessage(MENU_SETTINGS)));
	menuEdit->AddSeparatorItem();
	menuEdit->AddItem(edit_prefs=new BMenuItem("General Prefs..",new BMessage(MENU_PREFS)));
	//menuEdit->AddItem(edit_samples=new BMenu("External Samples"));

	//edit_track->AddItem(add_menu=new BMenu("Add"));
	//add_menu->AddItem(new BMenuItem("Sampler",new BMessage(ADD_TRACK),42));
	//add_menu->AddItem(new BMenuItem("Tn306",new BMessage(ADD_TN306),'B'));

	edit_track->AddItem(new BMenuItem("Remove",new BMessage(REMOVE_TRACK),'R'));
	edit_track->AddSeparatorItem();
	edit_track->AddItem(copy=new BMenuItem("Cut",new BMessage(MENU_CUT),'X'));
	edit_track->AddItem(cut=new BMenuItem("Copy",new BMessage(MENU_COPY),'C'));
	edit_track->AddItem(paste=new BMenuItem("Paste",new BMessage(MENU_PASTE),'V'));

	edit_mea->AddItem(mea_copy=new BMenuItem("Cut",new BMessage(MENU_MEA_CUT),'X',B_SHIFT_KEY));
	edit_mea->AddItem(mea_cut=new BMenuItem("Copy",new BMessage(MENU_MEA_COPY),'C',B_SHIFT_KEY));
	edit_mea->AddItem(mea_paste=new BMenuItem("Paste",new BMessage(MENU_MEA_PASTE),'V',B_SHIFT_KEY));


	paste->SetEnabled(false);
	mea_paste->SetEnabled(false);

	edit_track->SetTargetForItems(be_app);
	edit_mea->SetTargetForItems(be_app);


	//edit_samples->Addtem( new BMenuItem("Delete all from memory",NULL) ) ;


	menuBar->GetPreferredSize(&menuX,&menuY);


	BView *tv=new BView(BRect(-10,-10,0,0),"_temp",B_FOLLOW_NONE,B_WILL_DRAW);
	AddChild(tv);

	panel=new XPanel(BRect(0,0+menuY,WINDOW_XL-1,XPANEL_H+menuY-1),tv);

	panel->AddChild(One=new StereoMeter(BRect(20,50,50,80)));

	AddChild(panel);

	BView* fake=new BView( BRect(WINDOW_XL-1,0+menuY,WINDOW_XL,XPANEL_H+menuY),"_line!",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW);
	AddChild(fake);
	fake->SetViewBitmap(MakeBitmapSkin("Line"));


	tp=new TracksPanel(BRect(0,0+menuY+XPANEL_H,WINDOW_XL-12,WINDOW_YL-INFO_BAR_LY-1),tv);

	BScrollView	*scroll_view;
	scroll_view=new BScrollView("XRScroolView",tp,B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP,false,true,B_FANCY_BORDER);

	AddChild(scroll_view);


	AddChild(info_box=new BBox(BRect(0,WINDOW_YL-INFO_BAR_LY,WINDOW_XL,WINDOW_YL+menuY+1),"",B_FOLLOW_BOTTOM|B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW));
	info_box->SetViewColor(123,162,173);
	info_box->AddChild(info_txt=new BStringView(BRect(542,4,542+90,INFO_BAR_LY),"","",B_FOLLOW_BOTTOM|B_FOLLOW_RIGHT,B_WILL_DRAW));
	info_txt->SetFontSize(10);

	ticks=new TickView( BRect(172,0,542,16),0);
	info_box->AddChild(ticks);


	BBitmap	*tile=::MakeBitmapSkin("base");

	if(tile==NULL) printf("fa brutto!\n");
	panel->SetViewBitmap(tile);


	panel->Init();

	mw=new MatrixWindow(this);

	AddToSubset(mw);


	mw->Show();
	mw->Hide();

	succo=new Juice(mw);


	AddChild(menuBar);

	panel->MakeFocus(true);


	MoveTo(FloatKey("Xpos",WINDOW_X_POS),FloatKey("Ypos",WINDOW_Y_POS));
	ResizeTo(FloatKey("Xstop",WINDOW_XL),FloatKey("Ystop",WINDOW_YL));

	mw->MoveTo(FloatKey("playlist_Xpos",250),FloatKey("playlist_Ypos",30));
	mw->ResizeTo(FloatKey("playlist_Xstop",452),FloatKey("playlist_Ystop",320));

	RemoveShortcut('Q',0);
	float a,b,c,d;
	GetSizeLimits(&a,&b,&c,&d);
	SetSizeLimits(WINDOW_XL,b,panel->Frame().Height()+menuBar->Frame().Height()+20,d);


}
void
MainWindow::WindowActivated(bool active)
{
	if(panel==NULL) return;

	panel->MakeFocus(true);


	if(active)
		panel->SetEventMask(B_KEYBOARD_EVENTS);
		else
		panel->SetEventMask(0);



}
/*
BHandler*
MainWindow::getHandler()
{
	return (BHandler*)panel;
}*/
//MainWindow::~MainWindow()
void
MainWindow::Close()
{


}
bool
MainWindow::QuitRequested()
{
	status_t ask;

	ask=jfm->AskForClose(curSong);
	if(ask==B_OK)
	{
			PutFloatKey("Xpos",Frame().left);
			PutFloatKey("Ypos",Frame().top);
			PutFloatKey("Xstop",Frame().right-Frame().left);
			PutFloatKey("Ystop",Frame().bottom-Frame().top);

			PutFloatKey("playlist_Xpos",mw->Frame().left);
			PutFloatKey("playlist_Ypos",mw->Frame().top);
			PutFloatKey("playlist_Xstop",mw->Frame().right-mw->Frame().left);
			PutFloatKey("playlist_Ystop",mw->Frame().bottom-mw->Frame().top);



			if(mw->Lock()) mw->Quit();

			if(succo->Lock()) succo->Quit();

			be_app->PostMessage(B_QUIT_REQUESTED);

	return true;

	}
	else

	return false;
}
void
MainWindow::Saved()
{
	entry_ref ref;
	curSong->getEntry()->GetRef(&ref);

	SetTitle(ref.name);
	save_item->SetEnabled(false);

}
void
MainWindow::Init(JFileManager* jf,TrackManager*t)
{
	jfm=jf;
	tm=t;

	tp->Init(tm);
	//MakeBanksMenu();
	succo->Init(curSong,this,0);
	jfm->Init(succo,tm);
	if(Lock())
	{
		BMenu* x=tm->getMenu();
		x->SetTargetForItems(this);
		menuBar->AddItem(x);

		SetKeyMenuBar(menuBar);
		Unlock();
	}
}
/*
void
MainWindow::RefreshPopUp(int h)
{
	printf("SingleBank %d\n",h);
	if(h<0) tp->ChangeBank();
	else
		tp->SingleBank(h);
}*/
void
MainWindow::Reset(Song* s,bool juicereset=true)
{


	if(Lock())
	{

		panel->Reset(s,tp);
		tp->Reset(s,0);
		if(s->getEntry()!=NULL) {

				entry_ref t;
				s->getEntry()->GetRef(&t);
				SetTitle(t.name);
		} else
		SetTitle("XRS v1.2");
		save_item->SetEnabled(false);

		/*if(s->getBKM()!=NULL)
			menuBanks->FindItem(s->getBKM()->the_ref.name)->SetMarked(true);*/
		//else
		/*	{
				none_bank->SetMarked(true);
			}
		*/
		if(juicereset) succo->setSong(s);
		mw->Reset(s->getSequence());

		ticks->SetNumberNotes(s->getNumberNotes());
		curSong=s;
	Unlock();
	}
}

void
MainWindow::MessageReceived(BMessage* message)
{
	entry_ref ref;
	int k;
	status_t err;
	bool playing;

	if(message->what>1000 && message->what<2000)
	{
		curSong->setModified(true);
		save_item->SetEnabled(true);
		if(message->what>1900)
		{
			sprintf(control_out,"%10s : %3ld",control_array[message->what-1901],message->FindInt32("be:value"));
			info_txt->SetText(control_out);
		}
	}

	switch(message->what)
	{

	case MENU_SETTINGS:

		SongSettings*	ss;

		ss=new SongSettings();
		ss->Init(curSong);

		ss->Show();


	break;
	case ASYNC:
	//printf("Meter L %f\n",message->FindFloat("value"));
	PostMessage(message,One);
	break;
	/*case MENU_UNLOAD:
		err=AskRemove("Remove all External Samples?");
		if(err==B_OK)
		{
			curSong->getEXTM()->Empty();
			for(int y=0;y<curSong->getNumberTrack();y++)
			{
				if(curSong->getTrackAt(y)->getType()==EXT_SAMPLE_TYPE)
					curSong->getTrackAt(y)->setType(UNSET_TYPE);
			}
			RefreshPopUp(-1);

		}
	break;*/
	case PLAY_START:

		succo->Start(panel->isAllPat(),panel->getCurrentPattern());
		menuBar->SetEnabled(!succo->isPlay());
		if(!succo->isPlay()) {
			ticks->SetTick(-1,0,0 );
			mw->setPosition(0);
		}

		//PostMessage(message,panel);
		info_txt->SetText("");
		break;
	case PLAY_ALL_SET:

		if(!panel->isAllPat()) succo->setPosition(panel->getCurrentPattern());
		succo->setPatternMode(panel->isAllPat());
		info_txt->SetText("");
		curSong->all=panel->isAllPat();
		break;


	case CURPAT_MSG:
	//case TOTPAT_MSG:
		mw->PostMessage(message);
		succo->setPosition(panel->getCurrentPattern());

	break;

	case FALSEPAT_MSG:

		PostMessage(message,panel);
		succo->setPosition(mw->getSelectedPattern());

	break;

	/*case	MENU_BANK:

		playing=succo->isPlay();
		if(playing)	succo->Pause();

		if(message->FindRef("bank", &ref) == B_OK){

			if(curSong->getBKM()!=NULL)
				if( ref==curSong->getBKM()->the_ref) return;
			curSong->ChangeManager(new BankManager(ref.name));
			tp->ChangeBank();

			}
		break;

		if(playing) succo->Pause();

	case	NONE_BANK:

			playing=succo->isPlay();
			if(playing)	succo->Pause();
				curSong->RemoveBKM();
				tp->ChangeBank();
			if(playing) 	succo->Pause();
	break;*/
	case  MENU_EXPORT:
		//jfm->ExportWave();
		(new ExportPanel(panel->getCurrentPattern()+1,curSong->getNumberMeasure(),jfm))->Show();
		break;
	case TEMPO_SET:
		panel->SetTimeText(message->FindInt32("be:value"));
		PostMessage(message,succo);
		break;
	case TEMPO_MOV:
		succo->PostMessage(message);
		panel->SetTimeText(message->FindInt32("be:value"));
		PostMessage(message,succo);
		break;
	//case EXT_SAMPLE:
//		jfm->LoadSample(message);
//	break;
	case CK_MESSAGE:
		PostMessage(message,panel);
	break;
	case TRACK_SET:
		PostMessage(message,tp);
		save_item->SetEnabled(true);
	break;

#ifdef ENABLE_VST
	case 'VST':
		succo->ShowVstManager();

	break;
#endif

	case 'Mtr':
		setTickValue(message->FindInt16("beat"),message->FindInt16("pat"));
	break;
	case MENU_PREFS:
		(new PrefsPanel())->Show();

	break;
	case MENU_COPY:
	case MENU_CUT:
		paste->SetEnabled(true);

	case MENU_PASTE:
		tp->setPattern(panel->getCurrentPattern());
	break;

	case MENU_MEA_COPY:
	case MENU_MEA_CUT:
		mea_paste->SetEnabled(true);

	case MENU_MEA_PASTE:
		tp->setPattern(panel->getCurrentPattern());
	break;

	case ADD_TRACK:
		curSong->AddTrack(tm->getTrack(message->FindInt16("id")));
		tp->AddTrack(curSong->getNumberTrack()-1);
	break;

	/*case ADD_TN306:
		//
		curSong->AddTrack(tm->getTrack(1));
		tp->AddTrack(curSong->getNumberTrack()-1);
	break;*/

	case REMOVE_TRACK:

		//Need confirmation! //
		if(AskRemove("Remove selected Track ?")==B_OK){

		Track* trk=tp->getCurrentTrack();
		int pos=curSong->getIndexOfTrack(trk);
		curSong->RemoveTrack(trk);
		tp->RemoveTrackAt(pos);
		tp->setPattern(panel->getCurrentPattern()); //UI Refresh!

		}
	break;

	case EDIT_MSG:
		if(mw->IsHidden()) mw->Show();
			else mw->Hide();
	break;

	//case LOST_FOCUS:
//		panel->MakeFocus(true);
//		panel->SetEventMask(B_KEYBOARD_EVENTS);
//		printf("Activationz..\n");
//	break;

	case ADD_PATTERN:

		{
		int16 z=mw->getSelectedPattern();
		curSong->AddMeasure();
		panel->ResetMeasureCount();
		mw->Reset(curSong->getSequence());
		mw->setSelectedPattern(z);
		}//printf("Add_pattern\n");
	break;
	case REMOVE_PATTERN:

	if(!succo->isPlay()){

		//window_feel wf=mw->Feel();

		//mw->SetFeel(B_NORMAL_WINDOW_FEEL);

		//RemoveFromSubset(mw);

		//if(AskRemove("Remove selected Measure ?")!=B_OK) return;

		//mw->SetFeel(wf);
		//AddToSubset(mw);
		int16 f=message->FindInt16("measure");
		err=curSong->RemoveMeasure(f);

		if(err==B_OK){
		panel->ResetMeasureCount();
		mw->Reset(curSong->getSequence());

		if( f < curSong->getNumberMeasure() )
			{
			 mw->setSelectedPattern(f);
			// printf("< remove_pattern %d\n",f);
			}
		else
			{
			// printf("> remove_pattern %d\n",f);
			 mw->setSelectedPattern(curSong->getNumberMeasure()-1);
			}
		}
		}//printf("remove_pattern %d\n",message->FindInt16("measure"));
	break;
	default:

		BWindow::MessageReceived(message);
		break;
	}
}

status_t
MainWindow::AskRemove(const char *txt)
{
	BAlert *sam;
	int32 sam_say;

	sam=new BAlert("Remove ",txt,"Remove","No");
	sam_say=sam->Go();

	if(sam_say==0) return B_OK;

			else

			   return B_ERROR;
}

int
MainWindow::getCurrentPattern()
{
	return panel->getCurrentPattern();
}
Track*
MainWindow::getCurrentTrack()
{
	return tp->getCurrentTrack();
}
void
MainWindow::setTickValue(float val,int pat)
{
	ticks->SetTick((int16)val,pat,panel->getCurrentPattern());
}
