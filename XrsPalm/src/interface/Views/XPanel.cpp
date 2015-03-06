#include "XPanel.h"
#include "TranslationUtils.h"
#include "InterfaceKit.h"
#include"GraphicsDef.h"
#include "Xed_Utils.h"
#include "GfxMsg.h"
//#include "BankManager.h"
#include "Track.h"
#include "Song.h"
#include "stdio.h"
#include "TracksPanel.h"
#include "PatControl.h"
#include "ColorsDef.h"


XPanel::XPanel(BRect rect,BView* tv): BView(rect,"_xthis",B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP)
{
	SetViewColor(bkColor);
	
	//Play
	play_bt=new BPictureButton(BRect(PLAY_X_START,PLAY_Y_START,PLAY_X_START+PLAY_LX,PLAY_Y_START+PLAY_LY),"play",::MakePictureSkin("PlayOff",tv),::MakePictureSkin("PlayOn" ,tv),new BMessage(PLAY_START),B_TWO_STATE_BUTTON);
	AddChild(play_bt);
	play_bt->SetTarget(Parent());
	
	//CutOff
	sl_cut=new BSlider(BRect(CUTOFF_X_START,CUTOFF_Y_START,CUTOFF_X_START+CUTOFF_LX,CUTOFF_Y_START+CUTOFF_LY),"",NULL,NULL,0,MAX_CUT);
	sl_cut->SetModificationMessage(new BMessage(CUT_SET));
	AddChild(sl_cut);
	
	//Resonance
	sl_reso=new BSlider(BRect(RESO_X_START,RESO_Y_START,RESO_X_START+RESO_LX,RESO_Y_START+RESO_LY),"",NULL,NULL,0,MAX_RESO);
	sl_reso->SetModificationMessage(new BMessage(RESO_SET));
	//sl_reso->Archive();
	AddChild(sl_reso);
	
	//flter mode
	filter_on=new BPictureButton(BRect(FILTERON_X_START,FILTERON_Y_START,FILTERON_X_START+SBUTTON_W,FILTERON_Y_START+SBUTTON_H),"play",::MakePictureSkin("FilterOff" ,tv),::MakePictureSkin("FilterOn",tv),new BMessage(FILTER_MODE),B_TWO_STATE_BUTTON);
	AddChild(filter_on);
	filter_on->SetValue(false);
		
	//VST
	vst=new BPictureButton(BRect(VST_X_START,VST_Y_START,VST_X_START+SBUTTON_W,VST_Y_START+SBUTTON_H),"VST",::MakePictureSkin("FilterOff" ,tv),::MakePictureSkin("FilterOn",tv),new BMessage('VST'),B_ONE_STATE_BUTTON);
	AddChild(vst);
	vst->SetValue(false);
	
	// Meter
	all_bt=new BPictureButton(BRect(ALL_X_START,ALL_Y_START,ALL_X_START+ALL_LX,ALL_Y_START+ALL_LY),"all",::MakePictureSkin("AllOff",tv),::MakePictureSkin("AllOn" ,tv),new BMessage(PLAY_ALL_SET),B_TWO_STATE_BUTTON);
	AddChild(all_bt);
	all_bt->SetValue(false);
	all_bt->SetTarget(Parent());
	
		
	//Tempo
				
	tempo=new BSlider(BRect(TEMPO_X_START,TEMPO_Y_START,TEMPO_X_START+TEMPO_LX,TEMPO_Y_START+TEMPO_LY),"",NULL,new BMessage(TEMPO_SET),MIN_BPM,MAX_BPM);
	tempo->SetModificationMessage(new BMessage(TEMPO_MOV));
	tempo->SetTarget(Parent());
	AddChild(tempo);
	
	//tempo text;
	tempo_text=new PatControl(BRect(TTEXT_X_START,TTEXT_Y_START,TTEXT_X_START+TTEXT_LX,TTEXT_Y_START+TTEXT_LY),
				::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),20,300,120,TEMPO_FINE,::MakeBitmapSkin("PatBase"));
	AddChild(tempo_text);
	
	//curpat
	curpat=new PatControl(BRect(CURPAT_X_START,CURPAT_Y_START,CURPAT_X_START+TTEXT_LX,CURPAT_Y_START+TTEXT_LY),
				::MakePictureSkin("arrowup",tv),::MakePictureSkin("arrowup_sel",tv),::MakePictureSkin("arrowdown",tv),::MakePictureSkin("arrowdown_sel",tv),::MakePictureSkin("arrowdisabled",tv),1,1,1,CURPAT_MSG,::MakeBitmapSkin("PatBase"));
	AddChild(curpat);
	

	//edit
	edit=new BPictureButton(BRect(EDIT_X_START,EDIT_Y_START,EDIT_X_START+SBUTTON_W,EDIT_Y_START+SBUTTON_H),"edit",::MakePictureSkin("FilterOff",tv),::MakePictureSkin("FilterOn" ,tv),new BMessage(EDIT_MSG),B_ONE_STATE_BUTTON);
	AddChild(edit);
	edit->SetTarget(Parent());	
	
	key_rel=true;
	SetEventMask(B_KEYBOARD_EVENTS);
}

bool
XPanel::isAllPat()
{
	return all_bt->Value();
}
status_t
XPanel::Init()
{
	tempo->SetViewColor(123,162,173,255);
	sl_cut->SetViewColor(123,162,173,255);
	sl_reso->SetViewColor(123,162,173,255);
	
		 
	 return B_OK;
}


int
XPanel::getCurrentPattern()
{
	return curPattern;
}
Pattern*
XPanel::currentPattern()
{
	return curSong->getTrackAt(0)->getPatternAt(curPattern);
}

void
XPanel::ResetMeasureCount()
{
	curpat->setMax(curSong->getNumberMeasure());
}
void
XPanel::Reset(Song* s,TracksPanel* f)
{
	curSong=s;
	
	curPattern=0;
	tp=f;
		
	tempo->SetValue(curSong->getTempo());
	SetTimeText(curSong->getTempo());
	
	curpat->setValue(1);
	
	all_bt->SetValue(curSong->all);
	
	
	sl_cut->SetValue(curSong->getCutoff());
	sl_reso->SetValue(curSong->getResonance());
	
	filter_on->SetValue(curSong->isFilterOn());
	
	
	sl_cut->SetTarget(this);
	sl_reso->SetTarget(this);
	filter_on->SetTarget(this);	
	vst->SetTarget(this);
	//curpat
	
	ResetMeasureCount();
}


void
XPanel::MessageReceived(BMessage* message)
{
	//int id;
	BMessage *info;
	
	if(message->what>1000 && message->what<2000)
	{
		curSong->setModified(true);
		if(message->what>1900) Window()->PostMessage(message);
		else Window()->PostMessage(new BMessage(GENERIC));
	}
		
	switch(message->what)
	{

	
	
	case CURPAT_MSG:
		curPattern=curpat->getValue()-1;
		tp->setPattern(getCurrentPattern());
		Window()->PostMessage(message);
		
	break;
	
	case FALSEPAT_MSG:
	
		curpat->setValue(message->FindInt16("pos"));
		curPattern=curpat->getValue()-1;
		tp->setPattern(getCurrentPattern());
	break;
	/*case TOTPAT_MSG:
	
		curpat->setMax(totpat->getValue());
		curSong->setNumberMeasure(totpat->getValue());
		curPattern=curpat->getValue()-1;
		tp->setPattern(getCurrentPattern());
		Window()->PostMessage(message);
	
	break;*/
	
	case TEMPO_FINE:
	
	tempo->SetValue(tempo_text->getValue());
	info=new BMessage(TEMPO_MOV);
	info->AddInt32("be:value",tempo_text->getValue());
	Window()->PostMessage(info);
	
	break;
	
	
	
	case PLAY_START:
	
		play_bt->SetValue(!play_bt->Value());
		
		break;
	
	case	FILTER_MODE:
		
		curSong->setFilterIsOn(filter_on->Value());
	break;
	case CUT_SET:
		curSong->setCutoff(message->FindInt32("be:value"));
	break;
	case RESO_SET:
		curSong->setResonance(message->FindInt32("be:value"));
	break;
	default:
		
		BView::MessageReceived(message);
		
		break;
	}
} 
void
XPanel::MouseDown(BPoint)
{
	if(!Window()->IsActive()) Window()->Activate(true);
}
void
XPanel::KeyUp(const char *key,int32)
{
	key_rel=true;
}
void
XPanel::KeyDown(const char *key,int32 z)
{
	
	
	if(!key_rel) return;
	key_rel=false;
	BMessage *msg ;
	//bool b;
	
		
	switch(key[0])
	{
		case B_ESCAPE:
		Window()->Activate(true);
		break;
		case B_SPACE:
			
			Window()->PostMessage(new BMessage(PLAY_START));
			play_bt->SetValue(!play_bt->Value());
		
			break;	
		
		case B_FUNCTION_KEY:
		
		msg= Window()->CurrentMessage();
		
		if(msg){
			
			int key2=msg->FindInt32("key"); //,&key2);
			if(key2>=2 && key2<=9){
				
				key2--;
				
											
				msg=new BMessage(INT_DIR);
				msg->AddInt16("goto",key2);
				
				Window()->PostMessage(msg,curpat);
			}	
		}
		break;	
		
		case 'A':
		case 'a':
			Window()->PostMessage(new BMessage(PLAY_ALL_SET));
			all_bt->SetValue(!all_bt->Value());
		break;
		case 'M':
		case 'm' :
			tp->muteTrack(tp->getCurrentTrack());
		break;
		
		case 'S':
		case 's' :
			
			tp->soloTrack(tp->getCurrentTrack());
		break;
		case B_PAGE_UP:
		Window()->PostMessage(new BMessage(INT_UP),curpat);
		break;
		case B_PAGE_DOWN:
		Window()->PostMessage(new BMessage(INT_DW),curpat);
		break;
		default:break;
		
	}
	
	if(key[0]>'0' && key[0]<='9'){
	
		int id=key[0]-'1';
		if(id<curSong->getNumberTrack())
		tp->SelectTrack(id);
	
	}
	
		
}
void
XPanel::SetTimeText(int bmp)
{
	tempo_text->setValue(bmp);
	
}