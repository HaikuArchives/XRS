
#include "PatControl.h"
#include "Xed_Utils.h"
#include "stdio.h"

#include "GfxMsg.h"



PatControl::PatControl(BRect rect,BPicture*picUp,BPicture* picUpSel,BPicture* picDown,BPicture* picDownSel,BPicture* dis,int m,int ma,int cur,int32 mx,BBitmap *base): BView(rect,"W",B_FOLLOW_NONE,B_WILL_DRAW)
{
	
		
	up=new BPictureButton(BRect(rect.Width()-8,0,rect.Width(),8),"arrow_up",picUp,picUpSel,new BMessage(INT_UP),B_ONE_STATE_BUTTON);
	down=new BPictureButton(BRect(rect.Width()-8,9,rect.Width(),18),"arrow_up",picDown,picDownSel,new BMessage(INT_DW),B_ONE_STATE_BUTTON);
	
	up->SetDisabledOn(dis);
	up->SetDisabledOff(dis);
	
	down->SetDisabledOn(dis);
	down->SetDisabledOff(dis);
	

	//text=new BStringView(BRect(0,0,rect.Width()-9,rect.Height()),"text"," ");
	//text->SetFont(be_fixed_font);
	//text->SetFontSize(12);
	min=m;
	max=ma;
	value=cur;
	//to_msg=mx;
	SetFont(be_fixed_font);
	SetFontSize(12);
	
	
	is_en=true;
	
	the_msg=new BMessage(mx);
	the_msg->AddInt16("pos",0);
				
	AddChild(up);
	AddChild(down);
	//AddChild(text);
	pad=base;
}
void
PatControl::Draw(BRect r)
{
	BView::Draw(r);
	//SetHighColor(255,0,0);
	if(pad!=NULL) DrawBitmap(pad);
	SetHighColor(0,0,0);
	SetLowColor(230,230,230);
	DrawString(str,BPoint(7,12));
}
void
PatControl::MessageReceived(BMessage* msg)
{

	int get;
	
	switch(msg->what)
	{
		case INT_UP:
		
		if(value+1<=max){
			setValue(++value);
			the_msg->ReplaceInt16("pos",getValue());
			Window()->PostMessage(the_msg,Parent());
		}
		break;
		
		case INT_DW:
		
		if(value-1>=min){
			setValue(--value);
			the_msg->ReplaceInt16("pos",getValue());
			Window()->PostMessage(the_msg,Parent());
			
		}
		
		case INT_DIR:
		
		get=msg->FindInt16("goto");
		
		if(get >=min && get <=max){
			setValue(get);
			the_msg->ReplaceInt16("pos",getValue());
			Window()->PostMessage(the_msg,Parent());
		}
		
		break;
		
		default:
			BView::MessageReceived(msg);
		break;
	}
}
int
PatControl::getValue()
{
	return value;
}
void
PatControl::setEnabled(bool val)
{
	up->SetEnabled(val);
	down->SetEnabled(val);	
	is_en=val;
}
bool
PatControl::isEnabled()
{
	return is_en;
}
void
PatControl::setValue(int val)
{
	value=val;
	sprintf(str,"%2d",value);
	Invalidate();	
}
void
PatControl::setMax(int val)
{
	max=val;
	if(max<getValue()) setValue(max);	
}
void
PatControl::AttachedToWindow()
{
	if(Parent())
	{
		if(pad!=NULL) SetViewColor(B_TRANSPARENT_COLOR);
		else SetViewColor(Parent()->ViewColor());
		up->SetTarget(this);
		down->SetTarget(this);
	}
	
	setValue(value);
	
	BView::AttachedToWindow();
}
