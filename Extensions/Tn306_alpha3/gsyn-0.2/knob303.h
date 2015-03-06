#ifndef __KNOB303_H
#define __KNOB303_H

#include "pot.h"
#include "seq.h"
#include "vco303.h"
#include "vcf303.h"
#include "vca303.h"
#include "deverb.h"
#include "View.h"
#include "TextControl.h"

class SpecialTrack;

class TB303Knobs: public BView
{
private:
	BPot *tune, *cutoff, *reso, *envmod, *decay, *accent, *delay, *feedbk, *dist;
	BStringView *label;
	
	BMessage	*info;	
	
	 SpecialTrack *seq;
	VCO303 *vco;
	VCF303 *vcf;
	VCA303 *vca;
	Deverb *dly;
	BTextControl*	name_label;
public:
	TB303Knobs(BRect r);
	void	Reset(SpecialTrack*);
	virtual ~TB303Knobs();
	
	void do_tune(float t);
	void do_cutoff(float c);
	void do_reso(float r);
	void do_envmod(float e);
	void do_decay(float d);
	//void do_accent(float a);
	void do_delay(float d);
	void do_feedbk(float f);
	void do_dist(float d);

protected:
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage*);
	BPot * MakeController(BRect r,const char* n1,const char* n2,int min,int max ,int cur,const rgb_color col,int32);
};

#endif

