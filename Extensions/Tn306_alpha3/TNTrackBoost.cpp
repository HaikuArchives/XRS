#include "TNTrackBoost.h"
#include "Application.h"
#include "SpecialTrack.h"
#include "JSpecialTrack.h"
#include "filemaker_msg.h"
#include "Message.h"
#include "File.h"
// DINAMIC CAST!!!!!!!! //

TNTrackBoost::TNTrackBoost():TrackBoost()
{
	name[0]='T';
	name[1]='n';
	name[2]='3';
	name[3]='0';
	name[4]='6';
	name[5]='\0';
	
	id=1;

}

Track*
TNTrackBoost::getTrack()
{
	return (Track*)(new SpecialTrack());
}
JTrack*
TNTrackBoost::getJTrack(BRect r,int16 n,BView* tv)
{
	return (JTrack*)(new JSpecialTrack(r,n,tv));
}

bool
TNTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	bool ret=true;
	BMessage *msg;
	SpecialTrack*	st;	
	
	switch(va){
					
			case TRACK_SPECIFIC_DATA_START:
				
				st=(SpecialTrack*)cur;
				msg=new BMessage();
				msg->Unflatten(file);
				// debug .. vario..
				//printf("%s..\n",msg->FindString("TN_name"));	
				//printf("%f..\n",msg->FindFloat("TN_delay"));	
				//vcf
				st->vcf.Cutoff(msg->FindFloat("TN_cutoff"));
				st->vcf.Resonance(msg->FindFloat("TN_resonance"));
				st->vcf.Envmod(msg->FindFloat("TN_envmod"));
				st->vcf.Decay(msg->FindFloat("TN_decay"));
				//dly
				st->dly.Vol(msg->FindFloat("TN_delay"));
				st->dly.Distort(msg->FindFloat("TN_distortion"));
				st->dly.Feedback(msg->FindFloat("TN_feedback"));
				//seq
				st->Tune(msg->FindInt16("TN_tune"));
				//win
				st->winx=msg->FindFloat("TN_winx");
				st->winy=msg->FindFloat("TN_winy");							
				
			break;
			default:
				//printf("..%ld..",va);
				ret=false;
			break;
		}	
		
		return ret;
}
void
TNTrackBoost::Save(Track *cur,int t,BFile* file,int mea)
{
	//bool ap;
	if(cur==NULL) return;
	
	BMessage *msg;
	SpecialTrack*	st;
	
	st=(SpecialTrack*)cur;
		msg=new BMessage();
		msg->AddString("TN_name","TN306");
		// vcf 
		msg->AddFloat("TN_cutoff",st->vcf.getCutoff());
		msg->AddFloat("TN_resonance",st->vcf.getResonance());
		msg->AddFloat("TN_envmod",st->vcf.getEnvmod());
		msg->AddFloat("TN_decay",st->vcf.getDecay());		
		//dly
		msg->AddFloat("TN_delay",st->dly.getVol());
		msg->AddFloat("TN_feedback",st->dly.getFeedback());
		msg->AddFloat("TN_distortion",st->dly.getDistort());
		//seq
		msg->AddInt16("TN_tune",st->getTune());
		//win
		msg->AddFloat("TN_winx",st->winx);
		msg->AddFloat("TN_winy",st->winy);
						
		WriteFixed(TRACK_SPECIFIC_DATA_START,msg->FlattenedSize(),file);
		msg->Flatten(file);
}