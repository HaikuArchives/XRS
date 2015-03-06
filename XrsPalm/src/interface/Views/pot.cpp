/*
	SUper Pot By Anzani Andrea
	
	Questa versione include anche l'on-off tasto (lo swicth)
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WindowScreen.h"
#include "Application.h"

#include <Window.h>
#include "Xed_Utils.h"
#include "pot.h"

#include <SupportDefs.h>

const uint32 MSG_CHANGE_COLOR		= 'mCOL';
const uint32 MSG_CAPTURE_MOUSE		= 'mCPM';
const uint32 MSG_CHANGE_FOCUS		= 'mCFS';
const uint32 MSG_FILE_OPEN			= 'mFOP';
#define	SB_MSG	'sbmp'


static float rads(float degrees);



BPot::BPot(BRect frame, const char *name,
	const char *label, BMessage *message,BMessage *state,
	int32 minValue, int32 maxValue,BBitmap *p1,BBitmap *p2,
	uint32 resizingMode, uint32 flags)
	: BControl(frame, name, label, message, resizingMode, flags),
	m_fMinValue(minValue), m_fMaxValue(maxValue),
	m_fMinAngle(-150), m_fMaxAngle(150), m_fDragAngle(0),
	m_szMin(0), m_szMax(0),
	state_on(true),
	switch_msg(state)
{
	SetValue(minValue);
	SetMarked(false);
	
	namex=name;
	
	BRect b = Bounds();
	m_rectKnob.Set(0, 0, 20, 20);
	m_rectKnob.OffsetBy(b.left + (b.Width() - m_rectKnob.Width())/2,
		b.top + (b.Height() - m_rectKnob.Height())/2);
		
	rel_msg=new BMessage(SB_MSG);
	
	pad=p1;
	padoff=p2;
	tick=MakeBitmapSkin("tick");
	
}

BPot::~BPot()
{
	SetLimitLabels(0, 0);
}

void 
BPot::SetOn(bool val)
{
	state_on=val;
	Invalidate();
}
bool
BPot::isOn()
{
	return state_on;
}

void BPot::GetValueRange(int32 *min, int32 *max) const
{
	*min = m_fMinValue; *max = m_fMaxValue;	
}

void BPot::SetValueRange(int32 min, int32 max)
{
	assert(min != max);
	m_fMinValue = min; m_fMaxValue = max;
}

const char *BPot::MinLimitLabel() const
{
	return m_szMin;
}

const char *BPot::MaxLimitLabel() const
{
	return m_szMax;
}

void BPot::SetLimitLabels(const char *minLabel, const char *maxLabel)
{
	if (m_szMin) {
		free(m_szMin);
		m_szMin = 0;
	}
	if (m_szMax) {
		free(m_szMax);
		m_szMax = 0;
	}
	
	if (minLabel) m_szMin = strdup(minLabel);
	if (maxLabel) m_szMax = strdup(maxLabel);
}

void BPot::SetValue(int32 newValue)
{
	if (newValue < m_fMinValue) newValue = m_fMinValue;
	if (newValue > m_fMaxValue) newValue = m_fMaxValue;
	if (newValue != Value()) {
		BControl::SetValue(newValue);
		Invoke();
	}
}

float BPot::Angle() const
{
	return AngleForValue(Value());
}

float BPot::SetAngle(float newDegrees)
{
	SetValue(ValueForAngle(newDegrees));
	return Angle() - newDegrees;
}

int32 BPot::ValueForAngle(float degrees) const
{
	assert(m_fMinAngle != m_fMaxAngle);
	return (int32) floor(
		(m_fMinValue + (m_fMaxValue - m_fMinValue) *
			(degrees - m_fMinAngle) / (m_fMaxAngle - m_fMinAngle))
		+ 0.5);	
}

float BPot::AngleForValue(int32 value) const
{
	assert(m_fMinValue != m_fMaxValue);
	return (m_fMinAngle +
		((m_fMaxAngle - m_fMinAngle) *
			(value - m_fMinValue)) / (m_fMaxValue - m_fMinValue));
}

void BPot::SetMarked(bool marked)
{
	m_bMark = marked;
}

bool BPot::IsMarked() const
{
	return m_bMark;
}

void BPot::AttachedToWindow()
{
	if (Parent()) {
		SetViewColor(Parent()->ViewColor());
	}
}

void BPot::Draw(BRect updateRect)
{
	//DrawBoilerplate(updateRect);
	if(!state_on && padoff!=NULL) DrawBitmap(padoff);
		else 
		if(pad!=NULL) DrawBitmap(pad);
	//DrawLabels(updateRect);
	DrawKnob(updateRect);
}

void BPot::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MSG_CAPTURE_MOUSE:
		{
			int32 val;
			if (msg->FindInt32("be:value", &val) == B_OK) {
				SetMarked((bool)val);
				
			}
		}
	
	default:
		BControl::MessageReceived(msg);
		break;
	}
}

void BPot::DrawBoilerplate(BRect /*updateRect*/){}

void BPot::DrawLabels(BRect /*updateRect*/)
{
	BPoint o = KnobOrigin();
	float r = KnobRadius();
	font_height hFont;
	
	PushState();
	
	SetHighColor(100,100,100);
	SetPenSize(1);
	GetFontHeight(&hFont);
		
	float base = hFont.ascent + hFont.leading;
	BPoint inPt, outPt;
	
	inPt.x = o.x + (r+4)*sin(rads(m_fMinAngle));
	inPt.y = o.y - (r+4)*cos(rads(m_fMinAngle));
	outPt.x = inPt.x + 5*sin(rads(m_fMinAngle));
	outPt.y = inPt.y - 5*cos(rads(m_fMinAngle));
	StrokeLine(inPt, outPt);
	
	//if (m_szMin) {
//		float width = StringWidth(m_szMin);
//		
//		textPt.x += 2*sin(rads(m_fMinAngle)) - width;
//		textPt.y += -2*cos(rads(m_fMinAngle)) + base;
//		DrawString(m_szMin, textPt);
//	}
	
	
	SetHighColor(0,0,0);
	//SetLowColor(0,0,0);
	SetPenSize(1);
	GetFontHeight(&hFont);
	
	
	BPoint textPt = outPt;
	textPt.y += -2*cos(rads(m_fMinAngle)) + base;
	DrawString(namex, textPt);
	
	SetHighColor(100,100,100);
	SetPenSize(1);
	GetFontHeight(&hFont);
	
	inPt.x = o.x + (r+4)*sin(rads(m_fMaxAngle));
	inPt.y = o.y - (r+4)*cos(rads(m_fMaxAngle));
	outPt.x = inPt.x + 5*sin(rads(m_fMaxAngle));
	outPt.y = inPt.y - 5*cos(rads(m_fMaxAngle));
	StrokeLine(inPt, outPt);
	
	//if (m_szMax) {
//		BPoint textPt = outPt;
//		textPt.x += 2*sin(rads(m_fMaxAngle));
//		textPt.y += -2*cos(rads(m_fMaxAngle)) + base;
//		DrawString(m_szMax, textPt);
//	}
	PopState();
}

void BPot::DrawKnob(BRect updateRect)
{
	BRect calc = KnobRect();
	
	calc.InsetBy(-1,-1);
	
	if (updateRect.Intersects(calc)) {
	
		//SetHighColor(200,200,200);
		//FillEllipse(KnobRect());
		PushState();
		SetPenSize(1);
		SetHighColor(20,0,0);
		//StrokeEllipse(KnobRect());
		
		float r = KnobRadius()-3;
		BPoint o = KnobOrigin();
		
		BPoint p;
				
		p.x = o.x + r*sin(rads(Angle()));
		p.y = o.y - r*cos(rads(Angle()));
		
		SetHighColor(255,255,55);
		SetPenSize(1);
		StrokeLine(o, p);
		
		//DrawBitmap(tick,BPoint(p.x-2,p.y-2));
		
		PopState();
	}
}

void BPot::MouseDown(BPoint where)
{
	uint32 buttons;
	GetMouse(&where, &buttons);
	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		if (IsMarked() || Bounds().Contains(where)) {
			m_ptPrev = where;
			m_fDragAngle = Angle();
			SetTracking(true);
			SetMouseEventMask(B_POINTER_EVENTS,
				B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
		}
		
		mouse_start=where;
		ConvertToScreen(&mouse_start);
		be_app->HideCursor() ;
	}
	
	else
		if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		
		if(switch_msg!=NULL){
		
		state_on = !state_on;		
		
		Invalidate();
		Window()->PostMessage(switch_msg,Parent());}
		}
}

void BPot::MouseMoved(BPoint where, uint32 /* transit */,
	const BMessage* /* dragDropMsg */)
{
	if (IsTracking() && m_ptPrev != where) {
		float incr = CalcAngleIncr(m_ptPrev, where);
		incr += SetAngle(m_fDragAngle);
		if (incr != 0){
			DrawKnob(Bounds());
			
			}
		m_ptPrev = where;
		
		
	}
}

void BPot::MouseUp(BPoint where)
{
	if (IsTracking()) {
		if (m_ptPrev != where) {
			float incr = CalcAngleIncr(m_ptPrev, where);
			incr += SetAngle(m_fDragAngle);
			
			if (incr != 0)
				DrawKnob(Bounds());
				

				
				
		}
		SetTracking(false);
		//mouse_start
		//SetMousePosition
		set_mouse_position(mouse_start.x,mouse_start.y);
		be_app->ShowCursor() ;
		
	}
	
	Window()->PostMessage(rel_msg);
	
	
}

void
BPot::SetReleaseMessage(BMessage* x)
{
	rel_msg=x;
}

float BPot::CalcAngleIncr(BPoint prev, BPoint cur)
{
	float incr = GetDragAngle(prev, cur);
	m_fDragAngle += incr;
	if (m_fDragAngle > m_fMaxAngle) {
		m_fDragAngle = m_fMaxAngle;
	} else if (m_fDragAngle < m_fMinAngle) {
		m_fDragAngle = m_fMinAngle;
	} 
	
	return m_fDragAngle;
}

BRect BPot::KnobRect() const
{
	return m_rectKnob;
}

BPoint BPot::KnobOrigin() const
{
	BPoint o(m_rectKnob.left, m_rectKnob.top);
	o.x += m_rectKnob.Width() / 2;
	o.y += m_rectKnob.Height() / 2;
	return o;	
}

float BPot::KnobRadius() const
{
	return (m_rectKnob.Width() / 2);
}

float BPot::GetDragAngle(BPoint prev, BPoint cur)
{
	//return 2*(cur.x - prev.x);
	return -2*(cur.y - prev.y);
}



// Utility function -- convert degrees to radians
static float rads(float degrees)
{
	return (float)(degrees*M_PI/180);
}

void BPot::Stroke3DBox(BView* v, BRect r){}
